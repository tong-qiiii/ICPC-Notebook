"""Exercise native numbering in disposable copies of the complete Notebook.

This is a regression test, not a renumbering step. Compilation never runs it.
Set TYPST to the CLI path if needed; --pdf-check additionally needs PyMuPDF.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import re
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
INCLUDE = re.compile(r'^#include "sections/([^"\n]+)"', re.M)
LABEL = re.compile(r' <((?:chapter|algo)-[a-z0-9_-]+)>\s*$')
EXPRESSION = '''query(heading).filter(h => h.has("label")).map(h => (
  id: repr(h.label), level: h.level,
  number: counter(heading).at(h.location()),
  pattern: h.numbering, outlined: h.outlined,
  page: h.location().page(), body: h.body,
))'''
TARGETS = {"LCA": "algo-tree-lca", "MINT": "algo-math-modint",
           "INTERVAL": "algo-dp-interval-merge"}


def sections(main):
    return INCLUDE.findall(main)


def algorithms(source):
    """Keep complete algorithm blocks together, including source references."""
    lines = source.splitlines(keepends=True)
    starts = [i for i, line in enumerate(lines) if line.startswith("== ")]
    if not starts:
        raise AssertionError("Chapter has no algorithms")
    prefix = "".join(lines[:starts[0]])
    blocks = ["".join(lines[start:end]) for start, end in
              zip(starts, starts[1:] + [len(lines)])]
    return prefix, blocks


def edit_blocks(sources, filename, edit):
    prefix, blocks = algorithms(sources[filename])
    edit(blocks)
    sources[filename] = prefix + "".join(blocks)


def remove_algorithm(blocks, identity):
    index = next(i for i, block in enumerate(blocks)
                 if f'<{identity}>' in block.splitlines()[0])
    blocks.pop(index)


def expected_headings(main, sources):
    """Derive expected positions from markup, never from numeric filenames."""
    expected = []
    labels = set()
    imports = sections(main)
    assert len(imports) == len(set(imports))
    assert set(imports) == set(sources)
    for chapter, filename in enumerate(imports, 1):
        source = sources[filename]
        assert "counter(heading)" not in source, filename
        assert "#heading(" not in source, "Use the uniform native heading syntax"
        index = 0
        chapter_seen = False
        fence = False
        for line in source.splitlines():
            if line.startswith("```"):
                fence = not fence
            if fence or not line.startswith("="):
                continue
            level = len(line) - len(line.lstrip("="))
            assert level <= 4, "Use levels 1..4; details are unnumbered"
            if level > 2:
                continue
            match = LABEL.search(line)
            assert match, f"Missing stable label: {filename}: {line}"
            identity = match[1]
            assert identity not in labels, f"Duplicate label: {identity}"
            labels.add(identity)
            title = line[level + 1:match.start()].strip()
            title_prefix = re.sub(r'^#text\("', '', title)
            # Names such as "128 位整数" and "01 Trie" are not display IDs.
            assert not re.match(r'^\d+\.\d+(?:\.\d+)*\s', title_prefix), line
            if level == 1:
                assert not chapter_seen, filename
                assert identity.startswith("chapter-")
                chapter_seen = True
                number = [chapter]
            else:
                assert chapter_seen, filename
                assert identity.startswith("algo-")
                index += 1
                number = [chapter, index]
            expected.append((identity, level, number))
    return expected


def flatten(content):
    if isinstance(content, str):
        return content
    if not isinstance(content, dict):
        return ""
    if content.get("func") in ["text", "raw"]:
        return content["text"]
    if content.get("func") == "space":
        return " "
    if "children" in content:
        return "".join(flatten(c) for c in content["children"])
    return flatten(content.get("body"))


def normalized(text):
    return re.sub(r'[\s\u2060\u200b\ufeff]+', '', text)


def check_printed_outline(document, numbered):
    """Check visible TOC text and page numbers, in addition to PDF bookmarks."""
    text = normalized(''.join(
        document[i].get_text(clip=(0, 0, document[i].rect.width, 816))
        for i in range(1, numbered[0]["page"] - 1)))
    entries = []
    cursor = 0
    for heading in numbered:
        title = normalized('.'.join(map(str, heading["number"])) + ' ' +
                           flatten(heading["body"]))
        start = text.find(title, cursor)
        assert start >= cursor, ("Printed TOC title/order", title)
        end = start + len(title)
        entries.append((start, end, heading))
        cursor = end
    for i, (_, end, heading) in enumerate(entries):
        limit = entries[i + 1][0] if i + 1 < len(entries) else len(text)
        page = re.fullmatch(r'\.+(\d+)', text[end:limit])
        # Cover is unnumbered; printed page 1 is the first TOC page.
        assert page and int(page[1]) == heading["page"] - 1, (
            "Printed TOC target page", heading["id"], text[end:limit])


def run(command):
    result = subprocess.run(command, capture_output=True, text=True,
                            encoding="utf8", errors="replace", timeout=240)
    if result.returncode:
        raise RuntimeError(result.stderr + result.stdout)
    return result.stdout


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--pdf-check", action="store_true")
    parser.add_argument("--output-dir", type=Path,
                        help="Optionally retain test PDFs and query results")
    args = parser.parse_args()
    compiler = shutil.which(os.environ.get("TYPST", "typst"))
    if not compiler:
        raise SystemExit("Typst not found; set TYPST or add it to PATH")
    if args.pdf_check:
        try:
            import pymupdf
        except ImportError:
            raise SystemExit("--pdf-check needs PyMuPDF: python -m pip install pymupdf")
    if args.output_dir:
        args.output_dir.mkdir(parents=True, exist_ok=True)
    original_main = (ROOT / "main.typ").read_text(encoding="utf8")
    originals = {p.name: p.read_text(encoding="utf8") for p in
                 (ROOT / "sections").glob("*.typ")}
    protected = [ROOT / "main.typ", *sorted((ROOT / "sections").glob("*.typ")),
                 *sorted((ROOT / "raw").glob("*")),
                 *sorted((ROOT / "code").rglob("*.hpp"))]
    before = {p: hashlib.sha256(p.read_bytes()).hexdigest() for p in protected if p.is_file()}
    expected_headings(original_main, originals)
    results = []
    try:
        for scenario in ["baseline", "insert_start", "insert_middle", "swap",
                         "delete", "move_across_chapters", "reorder_chapters",
                         "rename_section", "auxiliary_headings", "deleted_reference"]:
            sources = originals.copy()
            entry = original_main
            tree = "03_tree.typ"
            if scenario.startswith("insert_"):
                place = 0 if scenario == "insert_start" else 4
                edit_blocks(sources, tree, lambda b: b.insert(place,
                    "== 临时编号测试 <algo-test-insert>\n\n仅用于测试。\n\n"))
            elif scenario == "swap":
                def swap(blocks):
                    blocks[1], blocks[4] = blocks[4], blocks[1]
                edit_blocks(sources, tree, swap)
            elif scenario == "delete":
                edit_blocks(sources, tree, lambda b: remove_algorithm(b, "algo-tree-hld"))
            elif scenario == "move_across_chapters":
                moved = []
                edit_blocks(sources, "10_dp.typ", lambda b: moved.append(b.pop(1)))
                edit_blocks(sources, "06_math.typ", lambda b: b.append(moved[0]))
            elif scenario == "reorder_chapters":
                lines = entry.splitlines(keepends=True)
                a = next(i for i, s in enumerate(lines) if s == '#include "sections/03_tree.typ"\n')
                b = next(i for i, s in enumerate(lines) if s == '#include "sections/06_math.typ"\n')
                lines[a], lines[b] = lines[b], lines[a]
                entry = "".join(lines)
            elif scenario == "rename_section":
                sources["math_reference.typ"] = sources.pop("06_math.typ")
                entry = entry.replace('sections/06_math.typ', 'sections/math_reference.typ')
            elif scenario == "auxiliary_headings":
                edit_blocks(sources, tree, lambda b: b.__setitem__(0, b[0] +
                    "=== 临时分类 <detail-test-category>\n\n分类说明。\n\n"
                    "==== 临时接口 <detail-test-interface>\n\n接口说明。\n\n"))
            elif scenario == "deleted_reference":
                edit_blocks(sources, tree, lambda b: remove_algorithm(b, "algo-tree-lca"))

            expected = expected_headings(entry, sources)
            entry += "\n\n" + "\n\n".join(
                f'#block[REF{name}BEGIN #ref(<{target}>, supplement: none) REF{name}END]'
                for name, target in TARGETS.items()) + "\n"
            with tempfile.TemporaryDirectory(prefix="icpc_numbering_") as temporary:
                work = Path(temporary)
                (work / "sections").mkdir()
                (work / "main.typ").write_text(entry, encoding="utf8")
                shutil.copy2(ROOT / "STYLE.md", work / "STYLE.md")
                shutil.copytree(ROOT / "code", work / "code")
                for filename, source in sources.items():
                    (work / "sections" / filename).write_text(source, encoding="utf8")
                command = [compiler, "eval", EXPRESSION, "--root", str(work),
                           "--in", str(work / "main.typ")]
                if scenario == "deleted_reference":
                    try:
                        run(command)
                    except RuntimeError as error:
                        message = str(error).lower()
                        assert "algo-tree-lca" in message and "does not exist" in message, message
                        print("PASS deleted_reference: dangling reference fails clearly", flush=True)
                        results.append({"scenario": scenario, "result": "expected error"})
                        continue
                    raise AssertionError("Deleting a referenced algorithm must fail compilation")
                actual = json.loads(run(command))
                numbered = [h for h in actual if h["level"] <= 2]
                observed = [(h["id"].strip("<>"), h["level"], h["number"]) for h in numbered]
                assert observed == expected, (scenario, observed, expected)
                assert all(h["outlined"] and h["pattern"] == "1.1" for h in numbered)
                if scenario == "auxiliary_headings":
                    details = [h for h in actual if h["id"].startswith("<detail-test-")]
                    assert len(details) == 2
                    assert all(h["pattern"] is None and not h["outlined"] for h in details)
                if args.pdf_check:
                    path = work / "notebook.pdf"
                    run([compiler, "compile", "--root", str(work), str(work / "main.typ"), str(path)])
                    document = pymupdf.open(path)
                    toc = document.get_toc()
                    assert len(toc) == len(numbered), (scenario, "outline entries")
                    check_printed_outline(document, numbered)
                    for heading, outline in zip(numbered, toc):
                        title = '.'.join(map(str, heading["number"])) + ' ' + flatten(heading["body"])
                        assert outline[0] == heading["level"]
                        assert normalized(outline[1]) == normalized(title), (scenario, title, outline)
                        assert outline[2] == heading["page"]
                        assert normalized(title) in normalized(document[heading["page"] - 1].get_text()), (scenario, "body", title)
                    all_text = normalized(''.join(page.get_text() for page in document))
                    by_id = {h["id"].strip("<>"): h for h in numbered}
                    for name, target in TARGETS.items():
                        number = '.'.join(map(str, by_id[target]["number"]))
                        assert f'REF{name}BEGIN{number}REF{name}END' in all_text, (scenario, target, number)
                    if args.output_dir:
                        shutil.copy2(path, args.output_dir / (scenario + ".pdf"))
                    document.close()
                if args.output_dir:
                    (args.output_dir / (scenario + ".json")).write_text(
                        json.dumps(actual, ensure_ascii=False, indent=2), encoding="utf8")
                results.append({"scenario": scenario, "numbered_headings": len(numbered),
                                "pdf_checked": args.pdf_check, "result": "PASS"})
                print(f"PASS {scenario}: {len(numbered)} headings, references" +
                      (", PDF outline/body" if args.pdf_check else ""), flush=True)
    finally:
        assert all(hashlib.sha256(p.read_bytes()).hexdigest() == v for p, v in before.items()), \
            "The numbering test changed protected workspace files"
    if args.output_dir:
        (args.output_dir / "results.json").write_text(json.dumps(results, indent=2), encoding="utf8")
    print("PASS NATIVE NUMBERING: all scenarios; workspace unchanged", flush=True)


if __name__ == "__main__":
    main()
