"""Divide docs/port/xbox360-todo.md en hub + 3 archivos por subsistemas.
Cumple la regla 16 de AGENTS.md (no perder contenido).

Uso: python docs/port/evidence/split_todo.py [--apply]
Sin --apply: solo muestra los rangos.
Con --apply: reescribe los 4 archivos.
"""
import argparse, pathlib, sys

ROOT = pathlib.Path(__file__).resolve().parents[3]
DOC = ROOT / "docs" / "port" / "xbox360-todo.md"
HUB = DOC
SUB_1_2 = ROOT / "docs" / "port" / "xbox360-todo-subsistemas-1-2.md"
SUB_3_4 = ROOT / "docs" / "port" / "xbox360-todo-subsistemas-3-4.md"
SUB_5_7 = ROOT / "docs" / "port" / "xbox360-todo-subsistemas-5-7.md"

# Line ranges (1-based, inclusive), from the structural map in PLAN.md session 12.
# Section 1 spans lines 14-427 (subsistemas 1-7).
SUB_1_2_RANGE = (14, 82)    # Subsistema 1 (16-45) + Subsistema 2 (46-82), under "## 1."
SUB_3_4_RANGE = (83, 250)   # Subsistema 3 (83-229) + Subsistema 4 (230-250)
SUB_5_7_RANGE = (251, 427)  # Subsistema 5 (251-345) + 6 (346-371) + 7 (372-427)
HUB_HEAD = (1, 13)         # Title + Resumen General
HUB_TAIL = (428, 821)      # Section 2, 2.1, 3, 4, 5

HUB_INTRO_INSERT_AFTER_HEAD = """\
## Indice de archivos de subsistemas (seccion 1)

La seccion 1 de este documento (Llamadas a Hardware PS2 por Subsistema)
se ha dividido en 3 archivos siguiendo la regla 16 de AGENTS.md (no
recortar para ajustarse a un tamano, dividir por subsistema sin perder
contenido). El hub (`xbox360-todo.md`) conserva el resumen, las
secciones 2 (inventario de `#ifdef PORT`), 3 (stubs de `port/src/`),
4 (riesgos abiertos) y 5 (conclusiones). El contenido de la seccion 1
vive en:

- [xbox360-todo-subsistemas-1-2.md](./xbox360-todo-subsistemas-1-2.md):
  subsistemas 1 (Archivos / Memoria / CDVD) y 2 (Input).
- [xbox360-todo-subsistemas-3-4.md](./xbox360-todo-subsistemas-3-4.md):
  subsistemas 3 (Renderer) y 4 (Sonido).
- [xbox360-todo-subsistemas-5-7.md](./xbox360-todo-subsistemas-5-7.md):
  subsistemas 5 (Memory Card), 6 (Peliculas) y 7 (Logica restante).

Para reconstruir la seccion 1 original, concatenar los tres archivos
en orden. Los enlaces relativos al repo (`../../src/...`, `../../port/...`)
siguen siendo validos desde cualquiera de los cuatro archivos porque
todos viven en `docs/port/`.

---

"""


def split():
    text = DOC.read_text(encoding="utf-8")
    lines = text.split("\n")
    # Sanity: 1-based indexing.
    assert len(lines) >= HUB_TAIL[1], f"file shorter than expected: {len(lines)}"
    return text, lines


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--apply", action="store_true")
    args = ap.parse_args()

    text, lines = split()
    n_total = len(lines)

    def slice_lines(rng):
        s, e = rng
        return "\n".join(lines[s - 1:e]) + "\n"

    # Build the hub: head + index insert + tail.
    head = slice_lines(HUB_HEAD)
    tail = slice_lines(HUB_TAIL)
    hub_text = head + HUB_INTRO_INSERT_AFTER_HEAD + tail

    sub_1_2_text = (
        "# Llamadas a Hardware PS2: Subsistemas 1-2 (Archivos/Memoria e Input)\n\n"
        "Parte de [xbox360-todo.md](./xbox360-todo.md) (hub). Ver el indice de\n"
        "subsistemas en la cabecera del hub. Esta seccion cubre los\n"
        "**subsistemas 1 y 2** de la regla 3 de AGENTS.md (orden de\n"
        "implementacion en fase 1: archivos/memoria, input, renderer, sonido,\n"
        "memory card, peliculas, logica restante).\n\n"
        "---\n\n"
        "## 1. Llamadas a Hardware PS2 por Subsistema (Regla 3 de AGENTS.md)\n\n"
        + slice_lines(SUB_1_2_RANGE)
    )
    sub_3_4_text = (
        "# Llamadas a Hardware PS2: Subsistemas 3-4 (Renderer y Sonido)\n\n"
        "Parte de [xbox360-todo.md](./xbox360-todo.md) (hub). Ver el indice de\n"
        "subsistemas en la cabecera del hub. Esta seccion cubre los\n"
        "**subsistemas 3 y 4** de la regla 3 de AGENTS.md.\n\n"
        "---\n\n"
        "## 1. Llamadas a Hardware PS2 por Subsistema (Regla 3 de AGENTS.md)\n\n"
        + slice_lines(SUB_3_4_RANGE)
    )
    sub_5_7_text = (
        "# Llamadas a Hardware PS2: Subsistemas 5-7 (Memory Card, Peliculas, Logica restante)\n\n"
        "Parte de [xbox360-todo.md](./xbox360-todo.md) (hub). Ver el indice de\n"
        "subsistemas en la cabecera del hub. Esta seccion cubre los\n"
        "**subsistemas 5, 6 y 7** de la regla 3 de AGENTS.md.\n\n"
        "---\n\n"
        "## 1. Llamadas a Hardware PS2 por Subsistema (Regla 3 de AGENTS.md)\n\n"
        + slice_lines(SUB_5_7_RANGE)
    )

    sizes = {
        "hub": len(hub_text),
        "sub_1_2": len(sub_1_2_text),
        "sub_3_4": len(sub_3_4_text),
        "sub_5_7": len(sub_5_7_text),
    }
    total_new = sum(sizes.values())
    total_old = len(text)
    print(f"Original: {total_old} chars, {n_total} lines")
    for k, v in sizes.items():
        print(f"  {k}: {v} chars")
    print(f"Suma: {total_new} chars (delta {total_new - total_old})")

    if not args.apply:
        return

    HUB.write_text(hub_text, encoding="utf-8", newline="")
    SUB_1_2.write_text(sub_1_2_text, encoding="utf-8", newline="")
    SUB_3_4.write_text(sub_3_4_text, encoding="utf-8", newline="")
    SUB_5_7.write_text(sub_5_7_text, encoding="utf-8", newline="")
    print("Written 4 files.")


if __name__ == "__main__":
    main()
