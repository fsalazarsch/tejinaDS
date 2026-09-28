import json
import urllib.request
import urllib.parse
import time
import os
import asyncio
from googletrans import Translator

# Nombre del archivo que contiene los códigos Unicode de tus kanjis
WHITELIST_FILE = "whitelist.txt"

# Inicializar el traductor de forma asíncrona
translator = Translator()

def cargar_unicodes_desde_whitelist():
    unicodes = []
    if not os.path.exists(WHITELIST_FILE):
        print(f"Error: No se encontró el archivo '{WHITELIST_FILE}' en esta carpeta.")
        return []
    
    with open(WHITELIST_FILE, "r", encoding="utf-8") as f:
        for linea in f:
            linea_limpia = linea.strip()
            if not linea_limpia:
                continue
            try:
                if linea_limpia.lower().startswith("0x"):
                    code = int(linea_limpia, 16)
                else:
                    code = int(linea_limpia, 16)
                unicodes.append(code)
            except ValueError:
                continue
    return unicodes

async def traducir_a_espanol(texto_ingles):
    try:
        # En la versión estable de googletrans se requiere usar 'await'
        traducido = await translator.translate(texto_ingles, src='en', dest='es')
        return traducido.text
    except Exception as e:
        return texto_ingles

async def obtener_vocabulario_jisho(kanji_char):
    url_kanji = urllib.parse.quote(kanji_char)
    url = f"https://jisho.org{url_kanji}"
    
    try:
        req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
        # Operación síncrona de red delegada al motor básico
        with urllib.request.urlopen(req, timeout=10) as response:
            data = json.loads(response.read().decode())
            
            ejemplos = []
            for item in data.get('data', []):
                japanese_info = item.get('japanese', [{}]) if item.get('japanese') else {}
                word = japanese_info.get('word')
                reading = japanese_info.get('reading')
                
                senses = item.get('senses', [{}]) if item.get('senses') else {}
                english_definitions = senses.get('english_definitions', [])
                definition_en = ", ".join(english_definitions) if english_definitions else ""
                
                if word and reading and definition_en and (kanji_char in word):
                    print(f"  > Traduciendo significado de '{word}': {definition_en[:30]}...")
                    # Esperar la respuesta asíncrona de la traducción
                    definition_es = await traducir_a_espanol(definition_en)
                    definition_es = definition_es.replace('"', '\\"')
                    ejemplos.append((word, reading, definition_es))
                
                if len(ejemplos) >= 9:
                    break
            
            return ejemplos
    except Exception as e:
        print(f"Error con el kanji {kanji_char}: {e}")
        return []

async def generar_archivo_c():
    kanji_unicodes = cargar_unicodes_desde_whitelist()
    if not kanji_unicodes:
        print("No se encontraron códigos válidos para procesar.")
        return
        
    print(f"Se cargaron {len(kanji_unicodes)} kanjis desde '{WHITELIST_FILE}'. Iniciando extracción...")
    
    with open("vocabulario_espanol.c", "w", encoding="utf-8") as f:
        f.write("#include <stdio.h>\n\n")
        f.write("typedef struct {\n")
        f.write("    const char *palabra;\n")
        f.write("    const char *lectura;\n")
        f.write("    const char *significado_es;\n")
        f.write("} VocabuloEjemplo;\n\n")
        
        f.write("typedef struct {\n")
        f.write("    unsigned int unicode; // Identificador numérico hexadecimal\n")
        f.write("    int cantidad_ejemplos;\n")
        f.write("    VocabuloEjemplo ejemplos[9]; // Matriz fija de 9 espacios explícita\n")
        f.write("} KanjiVocabEntry;\n\n")
        
        f.write("static const KanjiVocabEntry kanji_vocab_table[] = {\n")
        
        for code in kanji_unicodes:
            kanji_char = chr(code)
            hex_str = f"0x{code:04X}"
            print(f"\nProcesando kanji: {kanji_char} ({hex_str})")
            
            lista_ejemplos = await obtener_vocabulario_jisho(kanji_char)
            cant = len(lista_ejemplos)
            
            f.write("    {\n")
            f.write(f'        {hex_str}, {cant}, {{\n')
            
            for i in range(9):
                if i < cant:
                    word, reading, definition = lista_ejemplos[i]
                    f.write(f'            {{"{word}", "{reading}", "{definition}"}}')
                else:
                    f.write('            {NULL, NULL, NULL}')
                
                if i < 8:
                    f.write(",\n")
                else:
                    f.write("\n")
            
            f.write("        }\n")
            f.write("    },\n")
            
            # Un segundo de pausa para proteger la tasa de peticiones (Rate Limit)
            await asyncio.sleep(1.0)
            
        f.write("};\n")
        
    print("\n¡Archivo 'vocabulario_espanol.c' generado con éxito!")

if __name__ == "__main__":
    # Lanzar el bucle de eventos asíncronos nativo de Python
    asyncio.run(generar_archivo_c())
