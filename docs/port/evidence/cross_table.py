"""Genera la tabla cruzada categoria x mecanismo a partir de classified_calls.tsv.

Cumple PLAN.md sesion 4: 'Tabla cruzada categoria x mecanismo, en TSV.
Explicar las 70 filas de categoria "stub/impl en port/src" frente a
las 107 de mecanismo "stub de enlace"'.

Uso: python docs/port/evidence/cross_table.py
Salida: docs/port/evidence/cross_table.tsv y resumen en stdout.
"""
import collections, csv, pathlib

ROOT = pathlib.Path(__file__).resolve().parents[3]
INPUT = ROOT / "docs/port" / "evidence" / "classified_calls.tsv"
OUTPUT = ROOT / "docs/port" / "evidence" / "cross_table.tsv"


def main():
    counts = collections.defaultdict(lambda: collections.Counter())
    totals_cat = collections.Counter()
    totals_mech = collections.Counter()
    n = 0
    with INPUT.open(encoding="utf-8") as f:
        rdr = csv.DictReader(f, delimiter="\t")
        for row in rdr:
            cat = row["categoria"]
            mech = row["mecanismo_reemplazo"]
            counts[cat][mech] += 1
            totals_cat[cat] += 1
            totals_mech[mech] += 1
            n += 1
    cats = sorted(counts)
    mechs = sorted({m for c in counts.values() for m in c})

    with OUTPUT.open("w", encoding="utf-8") as out:
        out.write("Tabla cruzada: categoria x mecanismo (de classified_calls.tsv)\n")
        out.write(f"Total filas: {n}\n\n")
        out.write("categoria\t" + "\t".join(mechs) + "\ttotal\n")
        for c in cats:
            row = [counts[c].get(m, 0) for m in mechs]
            out.write(c + "\t" + "\t".join(str(x) for x in row) + "\t" + str(sum(row)) + "\n")
        out.write("total\t" + "\t".join(str(totals_mech[m]) for m in mechs) + "\t" + str(n) + "\n\n")
        out.write("Nota sobre la asimetria 70 vs 107:\n")
        out.write("  - 70 = categoria 'Stub / implementacion en port/src': son las DEFINICIONES\n")
        out.write("    de stubs en port/src/ (filas cuyo archivo es port/src/*.c).\n")
        out.write("  - 107 = mecanismo 'stub de enlace (port/src)': cubre TODAS las filas\n")
        out.write("    cuyo simbolo tiene un stub en port/src/ como reemplazo,\n")
        out.write("    lo que incluye: las 41 definiciones de stubs en port/src/ que\n")
        out.write("    coinciden con la categoria 'Stub / implementacion' (mecanismo\n")
        out.write("    = stub de enlace), 65 llamadas reales a esos stubs desde src/\n")
        out.write("    (categoria = 'Llamada real a funcion en src', mecanismo =\n")
        out.write("    stub de enlace), y 1 directiva #define (categoria = 'Directiva\n")
        out.write("    #define', mecanismo = stub de enlace). Total: 41 + 65 + 1 = 107.\n")

    print(f"Wrote {OUTPUT.relative_to(ROOT)}")
    print(f"\nTotales por categoria:")
    for c in cats:
        print(f"  {c}: {totals_cat[c]}")
    print(f"\nTotales por mecanismo:")
    for m in mechs:
        print(f"  {m}: {totals_mech[m]}")
    print(f"\nTotal: {n}")


if __name__ == "__main__":
    main()
