#!/usr/bin/env python3
"""
bcfnt_check_char.py

Comprueba si un caracter Unicode (codepoint) existe dentro de una fuente
BCFNT (formato CFNT usado en Nintendo 3DS).

Estructura del formato (según documentación de la comunidad homebrew 3DS):
    Header CFNT
        magic: 4 bytes  ("CFNT" o "CFNU" en versiones region-locked)
        endian: 2 bytes (0xFEFF = little endian)
        header_size: 2 bytes
        version: 4 bytes
        file_size: 4 bytes
        n_blocks: 4 bytes

    Cada bloque tiene un sub-header:
        magic: 4 bytes  ("FINF", "TGLP", "CWDH", "CMAP", etc.)
        size: 4 bytes  (incluye estos 8 bytes)

    Nos interesa la cadena de bloques CMAP, enlazada mediante el campo
    next_cmap_offset dentro de cada CMAP. Cada CMAP mapea un rango de
    codepoints Unicode a indices de glifo, en 3 posibles formatos:
        mapping_method 0 = DIRECT  (rango contiguo -> index_offset + (cp - code_begin))
        mapping_method 1 = TABLE   (tabla de uint16 indices para el rango)
        mapping_method 2 = SCAN    (lista de pares codepoint -> glyph_index)

Uso:
    python3 bcfnt_check_char.py archivo.bcfnt "A"
    python3 bcfnt_check_char.py archivo.bcfnt 0x0041
    python3 bcfnt_check_char.py archivo.bcfnt --all "Hola Mundo"
"""

import struct
import sys
import argparse

NO_GLYPH = 0xFFFF


class BCFNTParseError(Exception):
    pass


def read_cfnt_header(data):
    if len(data) < 16:
        raise BCFNTParseError("Archivo demasiado pequeño para ser un BCFNT valido")

    magic = data[0:4]
    if magic not in (b"CFNT", b"CFNU"):
        raise BCFNTParseError(f"Magic invalido: {magic!r} (se esperaba CFNT/CFNU)")

    endian = struct.unpack_from("<H", data, 4)[0]
    if endian == 0xFFFE:
        raise BCFNTParseError("Archivo en big endian; este script asume little endian")

    header_size = struct.unpack_from("<H", data, 6)[0]
    version = struct.unpack_from("<I", data, 8)[0]
    file_size = struct.unpack_from("<I", data, 12)[0]
    n_blocks = struct.unpack_from("<I", data, 16)[0] if len(data) >= 20 else None

    return {
        "magic": magic,
        "header_size": header_size,
        "version": version,
        "file_size": file_size,
        "n_blocks": n_blocks,
    }


def normalize_block_start(data, ptr, expected_magic):
    """
    Los offsets guardados en este formato a veces apuntan al inicio del
    bloque (al magic) y a veces al CUERPO del bloque (8 bytes despues del
    magic+size, saltandose el header comun). Esta funcion detecta cual de
    las dos convenciones aplica y devuelve siempre la posicion del magic.
    """
    if ptr is None or ptr <= 0 or ptr + 4 > len(data):
        return None

    # Caso 1: ptr ya apunta al magic
    if data[ptr:ptr + 4] == expected_magic:
        return ptr

    # Caso 2: ptr apunta al cuerpo (magic esta 8 bytes antes)
    if ptr - 8 >= 0 and data[ptr - 8:ptr - 4] == expected_magic:
        return ptr - 8

    return None


def find_finf_block(data):
    """Busca el bloque FINF, que contiene el offset al primer CMAP."""
    off = data.find(b"FINF")
    if off == -1:
        raise BCFNTParseError("No se encontro el bloque FINF")
    # FINF layout (offsets relativos al inicio del bloque, incluyendo magic+size):
    # 0x00 magic "FINF"
    # 0x04 size
    # 0x08 font_type
    # 0x09 line_feed
    # 0x0A alter_char_index (u16)
    # 0x0C default_width (3 bytes)
    # 0x0F encoding
    # 0x10 tglp_offset (u32)
    # 0x14 cwdh_offset (u32)
    # 0x18 cmap_offset (u32)
    raw_cmap_offset = struct.unpack_from("<I", data, off + 0x18)[0]
    encoding = data[off + 0x0F]

    cmap_block_start = normalize_block_start(data, raw_cmap_offset, b"CMAP")
    if cmap_block_start is None:
        raise BCFNTParseError(
            f"No se pudo ubicar el primer bloque CMAP (raw_offset={raw_cmap_offset:#x}). "
            "El layout de FINF puede diferir en esta version del formato."
        )
    return cmap_block_start, encoding


def iter_cmap_blocks(data, first_cmap_block_start, debug=False):
    """Recorre la lista enlazada de bloques CMAP. 'offset' en cada dict
    siempre es la posicion del magic 'CMAP' del bloque (ya normalizada)."""
    block_start = first_cmap_block_start
    seen = set()
    while block_start is not None and block_start not in seen:
        seen.add(block_start)
        if block_start + 20 > len(data):
            break
        magic = data[block_start:block_start + 4]
        if magic != b"CMAP":
            break

        code_begin = struct.unpack_from("<H", data, block_start + 8)[0]
        code_end = struct.unpack_from("<H", data, block_start + 10)[0]
        mapping_method = struct.unpack_from("<H", data, block_start + 12)[0]
        # 2 bytes de padding en block_start + 14
        raw_next = struct.unpack_from("<I", data, block_start + 16)[0]

        if debug:
            print(f"  [CMAP @ {block_start:#x}] rango U+{code_begin:04X}-U+{code_end:04X} "
                  f"metodo={mapping_method} raw_next={raw_next:#x}")

        yield {
            "offset": block_start,
            "code_begin": code_begin,
            "code_end": code_end,
            "mapping_method": mapping_method,
        }

        if raw_next == 0:
            break
        block_start = normalize_block_start(data, raw_next, b"CMAP")


def lookup_glyph_index(data, cmap, codepoint):
    """Dado un bloque CMAP ya identificado como candidato, resuelve el glyph index."""
    method = cmap["mapping_method"]
    body_off = cmap["offset"] + 20  # tras el header comun de 20 bytes

    if method == 0:  # DIRECT
        index_offset = struct.unpack_from("<H", data, body_off)[0]
        if cmap["code_begin"] <= codepoint <= cmap["code_end"]:
            return index_offset + (codepoint - cmap["code_begin"])
        return None

    elif method == 1:  # TABLE
        if not (cmap["code_begin"] <= codepoint <= cmap["code_end"]):
            return None
        idx_in_table = codepoint - cmap["code_begin"]
        pos = body_off + idx_in_table * 2
        if pos + 2 > len(data):
            return None
        glyph_index = struct.unpack_from("<H", data, pos)[0]
        return None if glyph_index == NO_GLYPH else glyph_index

    elif method == 2:  # SCAN
        n_entries = cmap["code_end"] - cmap["code_begin"] + 1
        pos = body_off
        for _ in range(n_entries):
            if pos + 4 > len(data):
                break
            cp = struct.unpack_from("<H", data, pos)[0]
            gi = struct.unpack_from("<H", data, pos + 2)[0]
            if cp == codepoint:
                return None if gi == NO_GLYPH else gi
            pos += 4
        return None

    return None


def char_exists_in_bcfnt(data, codepoint, debug=False):
    """Devuelve el glyph_index si el codepoint existe en la fuente, o None si no."""
    cmap_block_start, _encoding = find_finf_block(data)
    for cmap in iter_cmap_blocks(data, cmap_block_start, debug=debug):
        if cmap["code_begin"] <= codepoint <= cmap["code_end"]:
            gi = lookup_glyph_index(data, cmap, codepoint)
            if gi is not None:
                return gi
    return None


def parse_char_arg(s):
    """Permite pasar el caracter como 'A', '0x0041' o '41' (hex)."""
    s = s.strip()
    if s.lower().startswith("0x"):
        return int(s, 16)
    if len(s) == 1:
        return ord(s)
    # si no es un solo char, intenta interpretarlo como hex
    try:
        return int(s, 16)
    except ValueError:
        raise argparse.ArgumentTypeError(
            f"No se pudo interpretar '{s}' como caracter o codepoint hex"
        )


def main():
    parser = argparse.ArgumentParser(description="Busca un caracter Unicode dentro de un archivo BCFNT")
    parser.add_argument("bcfnt_path", help="Ruta al archivo .bcfnt")
    parser.add_argument("char", help="Caracter (ej: 'A'), codepoint hex (ej: 0x0041), o texto si usas --all")
    parser.add_argument("--all", action="store_true", help="Trata 'char' como una cadena y revisa cada caracter")
    parser.add_argument("--debug", action="store_true", help="Muestra la cadena de bloques CMAP encontrada")
    args = parser.parse_args()

    with open(args.bcfnt_path, "rb") as f:
        data = f.read()

    try:
        header = read_cfnt_header(data)
    except BCFNTParseError as e:
        print(f"Error leyendo el archivo: {e}")
        sys.exit(1)

    print(f"Archivo: {args.bcfnt_path}")
    print(f"  magic: {header['magic'].decode(errors='replace')}  version: {header['version']:#x}  tamano: {header['file_size']} bytes")
    print()

    if args.all:
        targets = [(c, ord(c)) for c in args.char]
    else:
        cp = parse_char_arg(args.char)
        targets = [(chr(cp) if 0 <= cp <= 0x10FFFF else "?", cp)]

    if args.debug:
        print("Cadena de bloques CMAP:")

    for ch, cp in targets:
        try:
            glyph_index = char_exists_in_bcfnt(data, cp, debug=args.debug)
        except BCFNTParseError as e:
            print(f"'{ch}' (U+{cp:04X}): ERROR -> {e}")
            continue

        if glyph_index is not None:
            print(f"'{ch}' (U+{cp:04X}): SI esta en la fuente -> glyph_index = {glyph_index}")
        else:
            print(f"'{ch}' (U+{cp:04X}): NO esta en la fuente")


if __name__ == "__main__":
    main()