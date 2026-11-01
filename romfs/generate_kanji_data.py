#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Genera kanji_data.c / kanji_data.h desde completekanji.txt consultando Japonés Básico.

Instalación: python -m pip install requests beautifulsoup4
Uso:         python generate_kanji_data.py
"""
import argparse
import html
import re
import sys
import time
import unicodedata
from dataclasses import dataclass, field
from pathlib import Path
from urllib.parse import quote

import requests
from bs4 import BeautifulSoup, Tag
from requests.adapters import HTTPAdapter
from urllib3.util.retry import Retry

BASE_URL = "https://japonesbasico.com/kanji/"
JLPT_MAP = {"N5": 5, "N4": 4, "N3": 3, "N2": 2, "N1": 1}
USER_AGENT = ("Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
              "AppleWebKit/537.36 Chrome/130 Safari/537.36")


@dataclass
class Word:
    kanji: str
    hiragana: str
    meaning: str


@dataclass
class KanjiData:
    character: str
    unicode_value: int = 0
    grade: str = ""
    jlpt: int = 0
    radical: str = ""
    stroke_count: int = 0
    meanings: list[str] = field(default_factory=list)
    kun: list[str] = field(default_factory=list)
    on: list[str] = field(default_factory=list)
    common_words: list[Word] = field(default_factory=list)
    uncommon_words: list[Word] = field(default_factory=list)


def clean(value):
    return re.sub(r"\s+", " ", html.unescape(value or "")).strip()


def unique(values):
    result, seen = [], set()
    for value in values:
        value = clean(value)
        if value and value not in seen:
            seen.add(value)
            result.append(value)
    return result


def session_with_retries():
    s = requests.Session()
    s.headers.update({"User-Agent": USER_AGENT, "Accept-Language": "es-ES,es;q=0.9"})
    retry = Retry(total=4, connect=4, read=4, backoff_factor=1,
                  status_forcelist=(429, 500, 502, 503, 504),
                  allowed_methods=frozenset(["GET"]))
    adapter = HTTPAdapter(max_retries=retry)
    s.mount("https://", adapter)
    s.mount("http://", adapter)
    return s


def label_value(soup, label):
    for bold in soup.find_all(["b", "strong"]):
        found = clean(bold.get_text(" ", strip=True)).rstrip(":").lower()
        if found == label.lower() and isinstance(bold.parent, Tag):
            parent_text = clean(bold.parent.get_text(" ", strip=True))
            label_text = clean(bold.get_text(" ", strip=True))
            return clean(parent_text[len(label_text):]).lstrip(":").strip()
    return ""


def readings(value):
    if not value:
        return []
    # La web presenta primero kana y luego su romanización.
    return unique(re.split(r"[,、，]\s*", value.split("/")[0]))


def extract_radical(soup):
    for bold in soup.find_all(["b", "strong"]):
        if "radical clásico" in clean(bold.get_text(" ", strip=True)).lower():
            text = clean(bold.parent.get_text(" ", strip=True))
            text = re.sub(r"^Radical clásico\s*:\s*", "", text, flags=re.I)
            return text.split("(")[0].strip()
    return ""


def extract_words(soup, heading_fragment):
    heading = None
    for candidate in soup.find_all(["h2", "h3", "h4"]):
        if heading_fragment.lower() in clean(candidate.get_text(" ", strip=True)).lower():
            heading = candidate
            break
    if heading is None:
        return []

    words = []
    for node in heading.find_all_next(["h2", "h3", "h4", "p"]):
        if node.name in ("h2", "h3", "h4"):
            break

        text = clean(node.get_text(" ", strip=True))
        match = re.search(r"\(([^()]*)\)", text)
        if not match:
            continue

        written = clean(text[:match.start()])
        hiragana = clean(match.group(1).split("/")[0])
        if not re.search(r"[\u3040-\u309f]", hiragana):
            continue

        meaning = clean(text[match.end():])
        # Quitar la romanización inicial (gen, iu, yū, etc.).
        meaning = re.sub(r"^[^ ]*[A-Za-zāīūēō'-]+[^ ]*\s*", "", meaning)
        meaning = re.sub(r"\s*◆\s*", " / ", meaning)
        meaning = re.sub(r"^[\s·\-]+", "", meaning).strip()
        if written and meaning:
            words.append(Word(written, hiragana, meaning))

    result, seen = [], set()
    for word in words:
        key = (word.kanji, word.hiragana, word.meaning)
        if key not in seen:
            seen.add(key)
            result.append(word)
    return result


def parse_page(character, page_html):
    soup = BeautifulSoup(page_html, "html.parser")
    data = KanjiData(character=character)

    unihan = soup.find("a", href=re.compile("GetUnihanData"))
    if unihan:
        match = re.search(r"([0-9A-Fa-f]{4,6})", unihan.get_text(" ", strip=True))
        if match:
            data.unicode_value = int(match.group(1), 16)
    if not data.unicode_value:
        data.unicode_value = ord(character[0])

    meta = soup.find("meta", attrs={"name": re.compile("^description$", re.I)})
    if meta and meta.get("content"):
        meaning = clean(meta["content"])
        meaning = re.sub(r"^El kanji\s+.+?\s+significa\s+", "", meaning, flags=re.I)
        data.meanings = unique(re.split(r"[,;／/]\s*", meaning))
    if not data.meanings:
        h2 = soup.find("h2")
        if h2:
            data.meanings = [clean(h2.get_text(" ", strip=True))]

    data.kun = readings(label_value(soup, "Lecturas japonesas"))
    data.on = readings(label_value(soup, "Lecturas chinas"))

    grade = label_value(soup, "Grado")
    m = re.match(r"([0-9]+(?:k)?)", grade, re.I)
    data.grade = m.group(1) if m else ""

    jlpt = re.search(r"\bN[1-5]\b", label_value(soup, "JLPT").upper())
    data.jlpt = JLPT_MAP.get(jlpt.group(0), 0) if jlpt else 0

    strokes = re.search(r"\d+", label_value(soup, "Trazos"))
    data.stroke_count = int(strokes.group(0)) if strokes else 0
    data.radical = extract_radical(soup)
    data.common_words = extract_words(soup, "Palabras comunes")
    data.uncommon_words = (extract_words(soup, "Palabras no comunes")
                           or extract_words(soup, "Palabras menos comunes"))
    return data


# ---------------------------------------------------------------- C output

def cstr(value):
    value = value.replace("\\", "\\\\").replace('"', '\\"')
    value = value.replace("\n", "\\n").replace("\r", "\\r").replace("\t", "\\t")
    value = value.replace("?", "\\?")  # evita trigrafos
    return '"' + value + '"'


def render_c(entries):
    out = ["/* Generado automáticamente. No editar manualmente. */",
           '#include "kanji_data.h"', ""]

    # Arrays estáticos por kanji (sin límites fijos de tamaño).
    refs = []
    for i, x in enumerate(entries):
        ref = {}
        for key, values in (("m", x.meanings), ("k", x.kun), ("o", x.on)):
            if values:
                name = f"k{i}_{key}"
                out.append(f"static const char *const {name}[] = {{ "
                           + ", ".join(cstr(v) for v in values) + " };")
                ref[key] = (name, len(values))
            else:
                ref[key] = ("NULL", 0)
        for key, words in (("c", x.common_words), ("u", x.uncommon_words)):
            if words:
                name = f"k{i}_{key}"
                out.append(f"static const KanjiWord {name}[] = {{")
                for w in words:
                    out.append("    { " + ", ".join(
                        cstr(t) for t in (w.kanji, w.hiragana, w.meaning)) + " },")
                out.append("};")
                ref[key] = (name, len(words))
            else:
                ref[key] = ("NULL", 0)
        refs.append(ref)

    out += ["", f"const KanjiEntryTable kanji_data[{len(entries)}] = {{"]
    for x, r in zip(entries, refs):
        out.append(
            f"    {{ 0x{x.unicode_value:04X}, {cstr(x.grade)}, {x.jlpt}, "
            f"{x.stroke_count}, {cstr(x.radical)},")
        out.append(f"      {r['m'][0]}, {r['m'][1]}, {r['k'][0]}, {r['k'][1]}, "
                   f"{r['o'][0]}, {r['o'][1]},")
        out.append(f"      {r['c'][0]}, {r['c'][1]}, {r['u'][0]}, {r['u'][1]} }},")
    out += ["};", ""]
    return "\n".join(out)


def render_header(total):
    return """#ifndef KANJI_DATA_H
#define KANJI_DATA_H

#define TOTAL_KANJI %d

typedef struct {
    const char *kanji;
    const char *hiragana;
    const char *meaning;
} KanjiWord;

typedef struct {
    unsigned int unicode;
    const char *grade;          /* Ej.: "9k", "1", "6" ("" = sin dato) */
    signed char jlpt;           /* 5=N5 ... 1=N1; 0=sin dato */
    unsigned char stroke_count;
    const char *radical;

    const char *const *meanings;
    unsigned short meanings_len;
    const char *const *kun;
    unsigned short kun_len;
    const char *const *on;
    unsigned short on_len;

    const KanjiWord *common_words;
    unsigned short common_words_len;
    const KanjiWord *uncommon_words;
    unsigned short uncommon_words_len;
} KanjiEntryTable;

extern const KanjiEntryTable kanji_data[TOTAL_KANJI];

#endif
""" % total


def read_kanji(path):
    result, seen = [], set()
    for line in path.read_text(encoding="utf-8-sig").splitlines():
        for char in line.strip():
            if char.isspace() or unicodedata.category(char).startswith("P"):
                continue
            if char not in seen:
                seen.add(char)
                result.append(char)
    return result


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--input", default="completekanji.txt")
    ap.add_argument("--output", default="kanji_data.c")
    ap.add_argument("--header", default="kanji_data.h")
    ap.add_argument("--cache", default=".kanji_cache")
    ap.add_argument("--delay", type=float, default=0.7)
    args = ap.parse_args()

    source = Path(args.input)
    if not source.is_file():
        print(f"No existe el archivo: {source}", file=sys.stderr)
        return 1
    kanji = read_kanji(source)
    if not kanji:
        print("No se encontraron kanji en el archivo.", file=sys.stderr)
        return 1

    cache = Path(args.cache)
    cache.mkdir(parents=True, exist_ok=True)
    http = session_with_retries()
    entries, failures = [], []

    for i, char in enumerate(kanji, 1):
        cache_file = cache / f"{ord(char):X}.html"
        try:
            if cache_file.exists():
                page = cache_file.read_text(encoding="utf-8")
            else:
                url = BASE_URL + quote(char, safe="")
                response = http.get(url, timeout=30)
                response.raise_for_status()
                response.encoding = response.apparent_encoding or "utf-8"
                page = response.text
                cache_file.write_text(page, encoding="utf-8")
                time.sleep(max(0, args.delay))

            item = parse_page(char, page)
            entries.append(item)
            print(f"[{i}/{len(kanji)}] {char} U+{item.unicode_value:04X} "
                  f"kun:{len(item.kun)} on:{len(item.on)} "
                  f"comunes:{len(item.common_words)} no_comunes:{len(item.uncommon_words)}")
        except Exception as exc:
            print(f"[{i}/{len(kanji)}] ERROR {char}: {exc}", file=sys.stderr)
            failures.append(char)
            entries.append(KanjiData(character=char, unicode_value=ord(char)))

    Path(args.output).write_text(render_c(entries), encoding="utf-8")
    Path(args.header).write_text(render_header(len(entries)), encoding="utf-8")
    print(f"\nArchivos generados: {args.output}, {args.header}")
    if failures:
        print(f"Kanji con error: {', '.join(failures)}")
        print("Vuelve a ejecutar el script: las páginas descargadas quedan en caché.")
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())