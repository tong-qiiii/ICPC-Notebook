"""Compile each selected notebook header independently, including the personal base."""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
import argparse
import json
import os
import re
import shutil
import subprocess
import tempfile
import time

ROOT = Path(__file__).resolve().parents[1]


def execute(command, timeout=120):
    for attempt in range(5):
        try:
            result = subprocess.run(command, capture_output=True, text=True,
                                    encoding="utf8", errors="replace", timeout=timeout)
            break
        except PermissionError as error:
            if os.name != "nt" or attempt == 4:
                raise RuntimeError(f"Could not execute {command[0]}: {error}") from error
            time.sleep(0.2 * (attempt + 1))
    if result.returncode and os.name == "nt" and "CreateProcess:" in result.stderr:
        time.sleep(0.5)
        result = subprocess.run(command, capture_output=True, text=True,
                                encoding="utf8", errors="replace", timeout=timeout)
    if result.returncode:
        raise RuntimeError(result.stdout + result.stderr)
    return result.stdout


def check(topic):
    compiler = shutil.which(os.environ.get("CXX", "g++"))
    if not compiler:
        raise RuntimeError("C++ compiler not found; set CXX")
    headers = sorted((ROOT / "code" / topic).glob("*.hpp"))
    base = re.search(r"```cpp\n(.*?)\n```", (ROOT / "STYLE.md").read_text(
        encoding="utf8"), re.S)[1]
    flags = ["-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror=c++20-extensions",
             "-Werror=c++23-extensions", "-D_GLIBCXX_ASSERTIONS",
             "-I", str(ROOT)]
    failures = []
    with tempfile.TemporaryDirectory(prefix="icpc_headers_") as temporary:
        work = Path(temporary)

        def compile_header(header):
            errors = []
            for mode, prefix in [("standalone", ""), ("personal_base", base)]:
                source = work / (header.stem + "_" + mode + ".cpp")
                source.write_text(prefix + (f'\n#include "code/{topic}/{header.name}"\n'
                                  * 2) + "int main() {}\n", encoding="utf8")
                try:
                    execute([compiler, *flags, "-fsyntax-only", str(source)])
                except RuntimeError as error:
                    errors.append(f"{header.name} [{mode}]\n{error}")
            first = work / (header.stem + "_standalone.cpp")
            second = work / (header.stem + "_second.cpp")
            second.write_text(f'#include "code/{topic}/{header.name}"\n', encoding="utf8")
            try:
                execute([compiler, *flags, str(first), str(second), "-o",
                         str(work / (header.stem + ".exe"))])
            except RuntimeError as error:
                errors.append(f"{header.name} [multiple translation units]\n{error}")
            return errors

        with ThreadPoolExecutor(max_workers=2) as pool:
            for i, result in enumerate(pool.map(compile_header, headers), 1):
                failures.extend(result)
                if i % 10 == 0 or i == len(headers):
                    print(f"{topic}: {i}/{len(headers)} headers checked", flush=True)
        for error in failures:
            print(error, flush=True)
    if failures:
        raise RuntimeError(f"{len(failures)} compilation checks failed")
    print(f"PASS {topic}: {len(headers)} independent headers, both modes", flush=True)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("topics", nargs="+")
    for selected in parser.parse_args().topics:
        check(selected)
