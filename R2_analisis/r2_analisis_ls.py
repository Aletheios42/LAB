#!/usr/bin/env python3
import r2pipe
import json

# Abrir el binario con r2pipe
r2 = r2pipe.open("/bin/ls")

# Realizar análisis automático completo
r2.cmd("aaa")  # Analiza funciones, referencias, variables, etc.

# Obtener lista de funciones
functions = json.loads(r2.cmd("aflj"))  # 'aflj' devuelve funciones en formato JSON

# Extraer información relevante de cada función
print("=== Funciones principales en /bin/ls ===")
for func in functions:
    # Filtrar solo funciones no-lib para reducir el ruido
    if not func["name"].startswith("sym.imp"):
        print(f"Función: {func['name']} en 0x{func['offset']:x} ({func['size']} bytes)")

# Analizar función principal (main)
print("\n=== Análisis de la función main ===")
r2.cmd("s main")  # Moverse a la función principal
main_disasm = r2.cmd("pdf")  # Disassemble función completa
# Extraer primeras 10 líneas para mostrar ejemplo
main_preview = "\n".join(main_disasm.split("\n")[:10])
print(main_preview)

# Buscar cadenas interesantes
print("\n=== Cadenas relevantes ===")
strings = json.loads(r2.cmd("izj"))  # 'izj' devuelve strings en formato JSON
for s in strings[:10]:  # Limitando a 10 strings para el ejemplo
    if len(s["string"]) > 5:  # Filtrar strings muy cortas
        print(f"0x{s['vaddr']:x}: {s['string']}")

# Analizar llamadas a funciones externas (e.g., libc)
print("\n=== Llamadas a funciones externas ===")
imports = json.loads(r2.cmd("iij"))  # 'iij' devuelve imports en formato JSON
if imports:
    # Mostrar qué claves están disponibles en el primer elemento
    print(f"Claves disponibles en importaciones: {list(imports[0].keys())}")
    for imp in imports[:10]:  # Limitando a 10 imports para el ejemplo
        print(f"{imp['name']} (ordinal: {imp['ordinal']})")

# Identificar secciones de memoria
print("\n=== Secciones de memoria ===")
sections = json.loads(r2.cmd("iSj"))  # 'iSj' devuelve secciones en formato JSON
for sec in sections:
    print(
        f"{sec['name']}: 0x{sec['vaddr']:x} - 0x{sec['vaddr']+sec['vsize']:x} ({sec['vsize']} bytes)"
    )

# Análisis de seguridad básico
print("\n=== Análisis de seguridad ===")
sec_info = r2.cmd("i~^canary,nx,pic")  # Buscar información de protecciones
print(sec_info)

# Análisis de grafos de llamadas
print("\n=== Grafo de llamadas desde main ===")
calls = r2.cmd("agc main")  # Generar grafo de llamadas desde main
print(calls[:500] + "..." if len(calls) > 500 else calls)  # Mostrar primeras 500 chars

# Análisis de referencias cruzadas
print("\n=== Referencias a funciones importantes ===")
xrefs_to_main = r2.cmd("axt main")  # Referencias a main
print("Referencias a main:")
print(xrefs_to_main)

# Análisis de funciones más grandes (potencialmente importantes)
print("\n=== Funciones más grandes (Top 5) ===")
largest_funcs = sorted(functions, key=lambda x: x["size"], reverse=True)[:5]
for func in largest_funcs:
    print(f"{func['name']}: {func['size']} bytes en 0x{func['offset']:x}")

# Análisis de bloques básicos en main
print("\n=== Bloques básicos en main ===")
blocks = r2.cmd("afbj @ main")
try:
    blocks_json = json.loads(blocks)
    print(f"La función main tiene {len(blocks_json)} bloques básicos")
    if blocks_json:
        print(
            f"Primer bloque: 0x{blocks_json[0]['addr']:x} ({blocks_json[0]['size']} bytes)"
        )
except json.JSONDecodeError:
    print("No se pudo decodificar la información de bloques básicos")

# Cerrar la sesión de r2
r2.quit()

