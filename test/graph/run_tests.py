"""Compile graph headers and PDF code with the exact personal base from STYLE.md."""
from pathlib import Path
import os
import re
import shutil
import subprocess
import tempfile


def run(command, cwd):
    result = subprocess.run(command, cwd=cwd, text=True, encoding="utf-8",
                            errors="replace", capture_output=True, timeout=120)
    if result.returncode:
        print(result.stdout, end="")
        print(result.stderr, end="")
        raise RuntimeError(f"Failed: {' '.join(map(str, command))}")
    return result.stdout


def main():
    root = Path(__file__).resolve().parents[2]
    compiler = shutil.which(os.environ.get("CXX", "g++"))
    if not compiler:
        raise SystemExit("Compiler not found; set CXX or add g++ to PATH")
    directory = root / "code/graph"
    headers = sorted(directory.glob("*.hpp"))
    style = (root / "STYLE.md").read_text(encoding="utf-8")
    base = re.search(r"```cpp\n(.*?)\n```", style, re.DOTALL).group(1)
    chapter = (root / "sections/02_graph.typ").read_text(encoding="utf-8")
    chunks = re.split(r"(?m)^== ", chapter)[1:]
    examples = []
    for chunk in chunks:
        match = re.search(r'\n#graph_code\("([^"]+)"(?:,\s*deps:\s*\((.*?)\))?\)', chunk)
        example = re.search(r"```cpp\n(.*?)\n```", chunk, re.DOTALL)
        if not match or not example:
            raise RuntimeError("Missing calling example or source: " + chunk.splitlines()[0])
        dependencies = re.findall(r'"([^"]+)"', match.group(2) or "")
        actual = re.findall(r'(?m)^#include "([^"]+)"', (directory / match.group(1)).read_text(encoding="utf-8"))
        if actual != dependencies:
            raise RuntimeError("Typst must expand every local dependency: " + match.group(1))
        source = "\n".join((directory / name).read_text(encoding="utf-8")
                           for name in [*dependencies, match.group(1)])
        source = re.sub(r'(?m)^#pragma once\n|^#include "[^"]+"\n', "", source)
        examples.append((match.group(1), source, example.group(1)))
    expected = {h.stem for h in headers}
    if {Path(name).stem for name, _, _ in examples} != expected or len(examples) != len(headers):
        raise RuntimeError("Chapter/header coverage is not one to one")
    for header in headers:
        source = header.read_text(encoding="utf-8")
        if re.search(r'(?:\)|else|struct\s+\w+)[ \t]*\{', source):
            raise RuntimeError("Opening scope brace must use its own line: " + header.name)
        if re.search(r'using ll\s*=', source):
            raise RuntimeError("ll alias conflicts with the personal macro: " + header.name)
    flags = ["-O2", "-Wall", "-Wextra", "-Werror", "-D_GLIBCXX_ASSERTIONS",
             "-ftrapv", "-I", str(root)]
    suffix = ".exe" if os.name == "nt" else ""
    with tempfile.TemporaryDirectory(prefix="icpc_graph_") as temporary:
        work = Path(temporary)
        base_path = work / "personal_base.hpp"
        base_path.write_text(base + "\n", encoding="utf-8")
        for mode, standard, extra in [("standalone", "-std=c++17", []),
                                      ("personal_base", "-std=gnu++17", ["-include", str(base_path)])]:
            print(f"[{mode}] independent headers ({len(headers)})", flush=True)
            for header in headers:
                source = work / "header.cpp"
                source.write_text(f'#include "code/graph/{header.name}"\n' * 2
                                  + "int main() {}\n", encoding="utf-8")
                run([compiler, standard, *flags, *extra, "-fsyntax-only", str(source)], work)
            print(f"[{mode}] samples, boundaries, repeated calls and brute references", flush=True)
            covered = set()
            for source in sorted((root / "test/graph").glob("*_test.cpp")):
                exe = work / (source.stem + suffix)
                run([compiler, standard, *flags, *extra, str(source), "-o", str(exe)], work)
                output = run([str(exe)], work)
                print(output, end="", flush=True)
                covered.update(re.findall(r"(?m)^PASS (\w+)$", output))
            if covered != expected:
                raise RuntimeError("Runtime coverage mismatch: " + repr(expected - covered))
        print("[PDF] compile and run all expanded templates/calling examples", flush=True)
        for i, (name, source, example) in enumerate(examples, 1):
            program = work / "copied.cpp"
            program.write_text(base + "\n" + source + "\nint main()\n{\n"
                               + example + "\n}\n", encoding="utf-8")
            exe = work / ("copied" + suffix)
            run([compiler, "-std=gnu++17", *flags, "-Wno-unused-variable",
                 "-Wno-unused-but-set-variable", str(program), "-o", str(exe)], work)
            run([str(exe)], work)
            if i % 5 == 0 or i == len(examples):
                print(f"[PDF] {i}/{len(examples)} passed", flush=True)
        print("[headers] multiple translation unit link", flush=True)
        includes = "".join(f'#include "code/graph/{h.name}"\n' for h in headers)
        first, second = work / "first.cpp", work / "second.cpp"
        first.write_text(includes + "int other();\nint main() { return other() != 1; }\n", encoding="utf-8")
        second.write_text(includes + "int other() { SCC s(1); s.work(); return s.cnt; }\n", encoding="utf-8")
        exe = work / ("linked" + suffix)
        run([compiler, "-std=c++17", *flags, str(first), str(second), "-o", str(exe)], work)
        run([str(exe)], work)
    print(f"PASS: {len(headers)} templates, 2 modes, {len(examples)} copied examples and multi-file link", flush=True)


if __name__ == "__main__":
    main()
