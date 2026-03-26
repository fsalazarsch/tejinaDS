#!/usr/bin/env python3
"""
Genera kanji_data.h a partir de kanjivg.xml y whitelist_clean.txt
Uso: python3 generate_kanji_data.py
"""

import xml.etree.ElementTree as ET
import sys

WHITELIST_PATH = 'whitelist_clean.txt'
XML_PATH       = 'kanjivg.xml'
OUTPUT_PATH    = 'kanji_data.h'

# ─── Cargar whitelist ─────────────────────────────────────────────────────────

with open(WHITELIST_PATH, 'r') as f:
    whitelist = [line.strip().lower() for line in f if line.strip()]

# Convertir 0x3042 → 03042
whitelist_ids = set(format(int(c, 16), '05x') for c in whitelist)

print(f"Whitelist: {len(whitelist_ids)} caracteres")

# ─── Parsear XML ──────────────────────────────────────────────────────────────

print("Cargando XML...")
tree = ET.parse(XML_PATH)
root = tree.getroot()

# Recopilar todos los kanji con sus strokes
# estructura: { '03042': ['path1', 'path2', ...], ... }
kanji_paths = {}

for elem in root.iter():
    eid = elem.get('id', '')
    # Solo nodos raíz de kanji: kvg:XXXXX (exactamente 9 chars)
    if not (eid.startswith('kvg:') and len(eid) == 9):
        continue

    kid = eid[4:]  # '03042'
    if kid not in whitelist_ids:
        continue

    # Extraer strokes en orden (kvg:XXXXX-s1, s2, ...)
    stroke_map = {}
    for child in elem.iter():
        cid = child.get('id', '')
        if cid.startswith(f'kvg:{kid}-s'):
            try:
                idx = int(cid.split('-s')[-1])
                d   = child.get('d', '')
                if d:
                    stroke_map[idx] = d
            except ValueError:
                pass

    if stroke_map:
        paths = [stroke_map[i] for i in sorted(stroke_map.keys())]
        kanji_paths[kid] = paths

print(f"Kanji encontrados: {len(kanji_paths)}")

# ─── Generar nombre de función desde codepoint ───────────────────────────────

def func_name(kid):
    code = int(kid, 16)
    char = chr(code)
    # Determinar bloque
    if 0x3040 <= code <= 0x309F:
        prefix = 'hira'
    elif 0x30A0 <= code <= 0x30FF:
        prefix = 'kata'
    else:
        prefix = 'kanji'
    return f'{prefix}_{kid}'

# ─── Escribir header ─────────────────────────────────────────────────────────

print(f"Generando {OUTPUT_PATH}...")

with open(OUTPUT_PATH, 'w', encoding='utf-8') as f:

    f.write('#ifndef KANJI_DATA_H\n')
    f.write('#define KANJI_DATA_H\n\n')
    f.write('#include "kanji_stroke.h"\n\n')
    f.write(f'// Auto-generado por generate_kanji_data.py\n')
    f.write(f'// Total de caracteres: {len(kanji_paths)}\n\n')

    # Tabla de lookup: array de punteros a función load
    # Se define al final, aquí solo adelantamos la idea

    entries = []  # (kid, fname, num_strokes)

    for kid in sorted(kanji_paths.keys()):
        paths = kanji_paths[kid]
        fname = func_name(kid)
        code  = int(kid, 16)
        char  = chr(code)
        n     = len(paths)

        entries.append((kid, fname, n, code))

        # Paths como array de strings
        f.write(f'// U+{kid.upper()} {char}\n')
        f.write(f'static const char* _{fname}_paths[] = {{\n')
        for path in paths:
            # Escapar backslashes y comillas por si acaso
            safe = path.replace('\\', '\\\\').replace('"', '\\"')
            f.write(f'    "{safe}",\n')
        f.write(f'}};\n\n')

        # Función loader
        f.write(f'static inline void kanji_load_{fname}(Kanji *k) {{\n')
        f.write(f'    k->num_strokes = {n};\n')
        f.write(f'    for (int i = 0; i < {n}; i++)\n')
        f.write(f'        parse_svg_path(_{fname}_paths[i], &k->strokes[i]);\n')
        f.write(f'}}\n\n')

    # ─── Tabla de lookup por codepoint ───────────────────────────────────────
    f.write('// ─── Lookup por codepoint Unicode ──────────────────────────────\n\n')
    f.write('typedef void (*kanji_loader_fn)(Kanji *);\n\n')

    f.write('typedef struct {\n')
    f.write('    unsigned int codepoint;\n')
    f.write('    kanji_loader_fn load;\n')
    f.write('} KanjiEntry;\n\n')

    f.write(f'static const KanjiEntry kanji_table[] = {{\n')
    for kid, fname, n, code in entries:
        f.write(f'    {{ 0x{kid.upper()}, kanji_load_{fname} }},\n')
    f.write('};\n\n')

    f.write(f'#define KANJI_TABLE_SIZE {len(entries)}\n\n')

    # Función de búsqueda binaria
    f.write('// Busca un carácter por codepoint y carga sus strokes\n')
    f.write('// Retorna 1 si encontrado, 0 si no\n')
    f.write('static inline int kanji_load(unsigned int codepoint, Kanji *k) {\n')
    f.write('    int lo = 0, hi = KANJI_TABLE_SIZE - 1;\n')
    f.write('    while (lo <= hi) {\n')
    f.write('        int mid = (lo + hi) / 2;\n')
    f.write('        if (kanji_table[mid].codepoint == codepoint) {\n')
    f.write('            kanji_table[mid].load(k);\n')
    f.write('            return 1;\n')
    f.write('        } else if (kanji_table[mid].codepoint < codepoint) {\n')
    f.write('            lo = mid + 1;\n')
    f.write('        } else {\n')
    f.write('            hi = mid - 1;\n')
    f.write('        }\n')
    f.write('    }\n')
    f.write('    return 0;\n')
    f.write('}\n\n')

    f.write('#endif // KANJI_DATA_H\n')

print(f"Listo: {OUTPUT_PATH}")
print(f"Uso en C: kanji_load(0x3042, &k);  // carga あ")
