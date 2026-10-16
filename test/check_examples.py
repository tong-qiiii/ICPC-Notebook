"""Compile and run expanded chapter examples; optionally select topics."""
from concurrent.futures import ThreadPoolExecutor
import argparse
from pathlib import Path
import json
import os
import re
import shutil
import tempfile
from check_headers import ROOT, execute


def expand(paths):
    chunks = []
    for path in paths:
        source = (ROOT / "code" / path).read_text(encoding="utf8")
        source = re.sub(r'(?m)^#pragma once\n|^#include "[^"]+"\n', "", source)
        chunks.append(source)
    return "\n".join(chunks)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("topics", nargs="*", help="Only run these code/<topic> examples")
    args = parser.parse_args()
    compiler = shutil.which(os.environ.get("CXX", "g++"))
    if not compiler:
        raise SystemExit("Compiler not found; set CXX")
    base = re.search(r"```cpp\n(.*?)\n```", (ROOT / "STYLE.md").read_text(encoding="utf8"), re.S)[1]
    examples = []
    for chapter in sorted((ROOT / "sections").glob("*.typ")):
        source = chapter.read_text(encoding="utf8")
        matches = list(re.finditer(r'#source_code\(\((.*?)\)\)', source))
        if not matches:
            continue
        previous = 0
        for match in matches:
            calls = re.findall(r"```cpp\n(.*?)\n```", source[previous:match.start()], re.S)
            if len(calls) != 1:
                raise RuntimeError("Expected one minimal example: " + chapter.name)
            paths = re.findall(r'"([^"]+)"', match[1])
            examples.append((paths[-1], expand(paths), calls[0]))
            previous = match.end()
    headers = {h.relative_to(ROOT / "code").as_posix() for h in (ROOT / "code").glob("*/*.hpp")
               if h.parent.name not in ["basic", "graph", "tree"]}
    if {name for name, _, _ in examples} != headers or len(examples) != len(headers):
        raise RuntimeError("Chapter/source/example coverage mismatch")
    if args.topics:
        known = {Path(name).parent.name for name in headers}
        if not set(args.topics) <= known:
            raise RuntimeError("Unknown topics: " + ", ".join(sorted(set(args.topics) - known)))
        examples = [item for item in examples if Path(item[0]).parent.name in args.topics]
    flags = ["-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror=c++20-extensions",
             "-Werror=c++23-extensions", "-D_GLIBCXX_ASSERTIONS", "-ftrapv"]
    with tempfile.TemporaryDirectory(prefix="icpc_examples_") as temporary:
        work = Path(temporary)

        def one(i, name, source, example):
            path = work / (str(i) + ".cpp")
            path.write_text(base + "\n" + source + "\nint main()\n{\n" + example + "\n}\n", encoding="utf8")
            exe = path.with_suffix(".exe")
            execute([compiler, *flags, str(path), "-o", str(exe)])
            execute([str(exe)])
            return name

        with ThreadPoolExecutor(max_workers=2) as pool:
            jobs = [(name, pool.submit(one, i, name, source, example))
                    for i, (name, source, example) in enumerate(examples)]
            failed = []
            for i, (name, job) in enumerate(jobs, 1):
                try:
                    job.result()
                except Exception as error:
                    failed.append(name)
                    print(f"FAIL {name}: {error}", flush=True)
                if i % 20 == 0 or i == len(examples):
                    print(f"PDF examples: {i}/{len(examples)} checked", flush=True)
            if failed:
                raise RuntimeError("Failed PDF examples: " + ", ".join(failed))
    print(f"PASS ALL {len(examples)} EXPANDED PDF EXAMPLES", flush=True)


if __name__ == "__main__":
    main()
