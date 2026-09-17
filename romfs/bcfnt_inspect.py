#!/usr/bin/env python3
"""
bcfnt_inspect.py

Paso 1 antes de escribir un inyector de simbolos: vuelca la estructura
completa de un BCFNT (FINF, TGLP, CWDH, cadena de CMAP) para conocer los
offsets, formato de textura, capacidad de glifos y rangos ya usados.

Uso:
    python3 bcfnt_inspect.py archivo.bcfnt
"""

import struct
import sys

SHEET_FORMATS = {
    0: "RGBA8", 1: "RGB8", 2: "RGBA5551", 3: "RGB565",
    4: "RGBA4", 5: "LA8", 6: "HILO8", 7: "L8",
    8: "A8", 9: "LA4", 10: "L4", 11: "A4", 12: "ETC1", 13: "ETC1A4",
}

MAPPING_METHODS = {0: "DIRECT", 1: "TABLE", 2: "SCAN"}


def normalize_block_start(data, ptr, expected_magic):
    if ptr is None or ptr <= 0 or ptr + 4 > len(data):
        return None
    if data[ptr:ptr + 4] == expected_magic:
        return ptr
    if ptr - 8 >= 0 and data[ptr - 8:ptr - 4] == expected_magic:
        return ptr - 8
    return None


def main():
    if len(sys.argv) != 2:
        print("Uso: python3 bcfnt_inspect.py archivo.bcfnt")
        sys.exit(1)

    with open(sys.argv[1], "rb") as f:
        data = f.read()

    if data[0:4] not in (b"CFNT", b"CFNU"):
        print("No parece un BCFNT valido (magic distinto de CFNT/CFNU)")
        sys.exit(1)

    header_size = struct.unpack_from("<H", data, 6)[0]
    version = struct.unpack_from("<I", data, 8)[0]
    file_size = struct.unpack_from("<I", data, 12)[0]
    n_blocks = struct.unpack_from("<I", data, 16)[0]

    print(f"=== HEADER CFNT ===")
    print(f"header_size={header_size:#x}  version={version:#x}  "
          f"file_size={file_size} (real: {len(data)})  n_blocks={n_blocks}")

    finf_off = data.find(b"FINF")
    if finf_off == -1:
        print("No se encontro FINF"); sys.exit(1)

    font_type = data[finf_off + 8]
    line_feed = data[finf_off + 9]
    alter_char_index = struct.unpack_from("<H", data, finf_off + 0xA)[0]
    encoding = data[finf_off + 0xF]
    raw_tglp = struct.unpack_from("<I", data, finf_off + 0x10)[0]
    raw_cwdh = struct.unpack_from("<I", data, finf_off + 0x14)[0]
    raw_cmap = struct.unpack_from("<I", data, finf_off + 0x18)[0]

    print(f"\n=== FINF @ {finf_off:#x} ===")
    print(f"font_type={font_type} line_feed={line_feed} "
          f"alter_char_index={alter_char_index:#x} encoding={encoding}")
    print(f"raw_tglp_offset={raw_tglp:#x}  raw_cwdh_offset={raw_cwdh:#x}  raw_cmap_offset={raw_cmap:#x}")

    # --- TGLP ---
    tglp_start = normalize_block_start(data, raw_tglp, b"TGLP")
    print(f"\n=== TGLP @ {tglp_start if tglp_start is None else hex(tglp_start)} ===")
    if tglp_start is not None:
        cell_width = data[tglp_start + 0x08]
        cell_height = data[tglp_start + 0x09]
        baseline_pos = data[tglp_start + 0x0A]
        max_char_width = data[tglp_start + 0x0B]
        sheet_size = struct.unpack_from("<I", data, tglp_start + 0x0C)[0]
        num_sheets = struct.unpack_from("<H", data, tglp_start + 0x10)[0]
        sheet_format = struct.unpack_from("<H", data, tglp_start + 0x12)[0]
        num_cols = struct.unpack_from("<H", data, tglp_start + 0x14)[0]
        num_rows = struct.unpack_from("<H", data, tglp_start + 0x16)[0]
        sheet_width = struct.unpack_from("<H", data, tglp_start + 0x18)[0]
        sheet_height = struct.unpack_from("<H", data, tglp_start + 0x1A)[0]
        sheet_data_offset = struct.unpack_from("<I", data, tglp_start + 0x1C)[0]

        glyphs_per_sheet = num_rows * num_cols
        fmt_name = SHEET_FORMATS.get(sheet_format, f"desconocido({sheet_format})")

        print(f"cell={cell_width}x{cell_height}  baseline_pos={baseline_pos}  "
              f"max_char_width={max_char_width}")
        print(f"sheet_size={sheet_size} bytes/hoja  num_sheets={num_sheets}  formato={fmt_name}")
        print(f"cols={num_cols} rows={num_rows} -> {glyphs_per_sheet} glifos/hoja  "
              f"capacidad total = {glyphs_per_sheet * num_sheets}")
        print(f"sheet_dims={sheet_width}x{sheet_height}  sheet_data_offset={sheet_data_offset:#x} "
              f"(absoluto, apunta directo a los pixeles crudos)")
        print(f"tamano esperado de datos de pixeles: sheet_size*num_sheets = {sheet_size * num_sheets} bytes")
    else:
        print("No se pudo ubicar (revisar layout de FINF)")

    # --- CWDH chain ---
    print(f"\n=== CWDH (cadena) ===")
    cwdh_start = normalize_block_start(data, raw_cwdh, b"CWDH")
    idx = 0
    while cwdh_start is not None:
        start_index = struct.unpack_from("<H", data, cwdh_start + 8)[0]
        end_index = struct.unpack_from("<H", data, cwdh_start + 0xA)[0]
        raw_next = struct.unpack_from("<I", data, cwdh_start + 0xC)[0]
        print(f"  bloque {idx} @ {cwdh_start:#x}: glifos {start_index}-{end_index} "
              f"({end_index - start_index + 1} entradas)  raw_next={raw_next:#x}")
        if raw_next == 0:
            break
        cwdh_start = normalize_block_start(data, raw_next, b"CWDH")
        idx += 1

    # --- CMAP chain ---
    print(f"\n=== CMAP (cadena) ===")
    cmap_start = normalize_block_start(data, raw_cmap, b"CMAP")
    idx = 0
    seen = set()
    while cmap_start is not None and cmap_start not in seen:
        seen.add(cmap_start)
        code_begin = struct.unpack_from("<H", data, cmap_start + 8)[0]
        code_end = struct.unpack_from("<H", data, cmap_start + 0xA)[0]
        method = struct.unpack_from("<H", data, cmap_start + 0xC)[0]
        raw_next = struct.unpack_from("<I", data, cmap_start + 0x10)[0]
        method_name = MAPPING_METHODS.get(method, f"desconocido({method})")
        print(f"  bloque {idx} @ {cmap_start:#x}: U+{code_begin:04X}-U+{code_end:04X} "
              f"metodo={method_name}  raw_next={raw_next:#x}")
        if raw_next == 0:
            break
        cmap_start = normalize_block_start(data, raw_next, b"CMAP")
        idx += 1

    print(f"\nTamano total del archivo: {len(data)} bytes")
    print("Espacio libre al final tras el ultimo bloque detectado: revisar manualmente si hace falta.")


if __name__ == "__main__":
    main()