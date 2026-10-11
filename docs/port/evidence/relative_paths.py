"""Convierte las URLs `file:///c:/Users/.../Xbox360BT3/<resto>` en `docs/port/*.md`
a rutas relativas al repo. Cumple la regla 12 de AGENTS.md.

Uso: python -m docs.port.evidence.relative_paths [--apply]
Sin --apply: solo cuenta y muestra ejemplos (modo dry-run).
Con --apply: reescribe los archivos.

Evidencia: docs/port/evidence/relative-paths.txt.
"""
import argparse, os, pathlib, re, sys

ROOT = pathlib.Path(__file__).resolve().parents[3]  # docs/port/evidence/<script> -> repo root
DOCS_PORT = ROOT / "docs" / "port"
PREFIX = "file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/"

# Anything that may follow a `file:///` URL: closing paren, end of line, end of attribute.
# We match the URL up to the first character that cannot be part of it (`)`, `"`, whitespace, `]`).
URL_RE = re.compile(r"file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/[^\s)\"\]<>]*")


def relpath_from(md_file: pathlib.Path, target_in_repo: str) -> str:
    """Path of `target_in_repo` (relative to repo) from the directory of `md_file`."""
    target = (ROOT / target_in_repo).resolve()
    md_dir = md_file.parent.resolve()
    rel = os.path.relpath(target, md_dir).replace(os.sep, "/")
    # Normalize same-directory links to "./name" for portability across renderers.
    if not rel.startswith("."):
        rel = "./" + rel
    return rel


def convert_text(md_file: pathlib.Path, text: str, apply: bool, evidence: list) -> str:
    out = []
    last = 0
    count = 0
    for m in URL_RE.finditer(text):
        url = m.group(0)
        # Strip an optional #Ln or #Ln-Lm anchor for the target lookup, but keep it in the output.
        anchor = ""
        target = url
        if "#" in target:
            target, anchor = target.split("#", 1)
            anchor = "#" + anchor
        # target is the repo-relative path (after PREFIX).
        repo_rel = target[len(PREFIX):]
        rel = relpath_from(md_file, repo_rel)
        new = rel + anchor
        if apply:
            out.append(text[last:m.start()])
            out.append(new)
        count += 1
        evidence.append(f"{md_file.relative_to(ROOT)}: {url} -> {new}")
        last = m.end()
    if apply:
        out.append(text[last:])
        return "".join(out), count
    return text, count


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--apply", action="store_true",
                    help="Rewrite the files in place. Default: dry-run (count only).")
    args = ap.parse_args()

    targets = sorted(DOCS_PORT.glob("*.md"))
    total = 0
    evidence = []
    for f in targets:
        text = f.read_text(encoding="utf-8")
        new_text, n = convert_text(f, text, args.apply, evidence)
        total += n
        if args.apply and n > 0:
            f.write_text(new_text, encoding="utf-8", newline="")
        print(f"{f.relative_to(ROOT)}: {n} URL(s) {'rewritten' if args.apply else 'found'}")

    print(f"TOTAL: {total} URL(s) {'rewritten' if args.apply else 'found'}")
    ev = ROOT / "docs" / "port" / "evidence" / "relative-paths.txt"
    if args.apply:
        ev.write_text("\n".join(evidence) + ("\n" if evidence else ""), encoding="utf-8")
        print(f"Evidence: {ev.relative_to(ROOT)} ({len(evidence)} lines)")
    else:
        # Always write a dry-run evidence, so the count is reproducible.
        ev.write_text("# Dry-run: relative-paths.py (no --apply)\n\n"
                      + f"Total URLs found: {total}\n\n"
                      + "First 10 examples:\n"
                      + "\n".join(evidence[:10]) + "\n", encoding="utf-8")
        print(f"Evidence (dry-run): {ev.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
