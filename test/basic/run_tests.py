"""C++17 checks with GCC; generated files stay in a temporary directory."""
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile


def run(command, cwd):
    subprocess.run(command, cwd=cwd, check=True, timeout=120)


def main():
    root = Path(__file__).resolve().parents[2]
    compiler = shutil.which(os.environ.get("CXX", "g++"))
    if compiler is None:
        raise SystemExit("C++17 compiler not found; set CXX or add g++ to PATH")
    headers = sorted((root / "code/basic").glob("*.hpp"),
                     key=lambda h: (h.name != "int128_io.hpp", h.name))
    chapter = (root / "sections/01_tools.typ").read_text(encoding="utf-8")
    examples = re.findall(r"```cpp\n(.*?)\n```", chapter, re.DOTALL)
    references = re.findall(r'#raw\(read\("([^"]+)"\)', chapter)
    if len(examples) != len(headers) or len(references) != len(headers):
        raise SystemExit("Expected one calling example and source reference per basic header")
    for reference in references:
        if not (root / "sections" / reference).is_file():
            raise SystemExit(f"Missing source reference: {reference}")
    if {h.resolve() for h in headers} != {
            (root / "sections" / ref).resolve() for ref in references}:
        raise SystemExit("Basic chapter references do not cover the basic headers")
    tests = sorted(p for p in (root / "test/basic").glob("*_test.cpp")
                   if p.name != "tools_test.cpp")
    flags = ["-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror",
             "-D_GLIBCXX_ASSERTIONS", "-ftrapv", "-I", str(root)]
    with tempfile.TemporaryDirectory(prefix="icpc_basic_") as temporary:
        work = Path(temporary)
        aux = work / "other.cpp"
        aux.write_text(
            "".join(f'#include "code/basic/{h.name}"\n' for h in headers)
            + "std::mt19937_64* other_rng() { return &rng; }\n"
            + "std::uint64_t other_hash_seed() { return CustomHash::seed(); }\n",
            encoding="utf-8",
        )
        for name, extra in [("submission", []), ("local", ["-DLOCAL"])]:
            directory = work / name
            directory.mkdir()
            print(f"[{name}] independent header compilation", flush=True)
            for header in headers:
                source = directory / "header.cpp"
                source.write_text(
                    f'#include "code/basic/{header.name}"\n' * 2 + "int main() {}\n",
                    encoding="utf-8",
                )
                run([compiler, *flags, *extra, "-fsyntax-only", str(source)], directory)
            print(f"[{name}] chapter calling examples", flush=True)
            for example in examples:
                source = directory / "example.cpp"
                source.write_text(
                    "#include <bits/stdc++.h>\n"
                    + "".join(f'#include "code/basic/{h.name}"\n' for h in headers)
                    + "int main() {\n" + example + "\nreturn 0;\n}\n",
                    encoding="utf-8",
                )
                run([compiler, *flags, *extra, "-Wno-unused-variable",
                     "-Wno-unused-but-set-variable", "-Wno-unused-parameter",
                     "-fsyntax-only", str(source)], directory)
            executable = directory / ("tools_test.exe" if os.name == "nt" else "tools_test")
            print(f"[{name}] multi-file link and boundary/random/process tests", flush=True)
            run([compiler, *flags, *extra, str(root / "test/basic/tools_test.cpp"),
                 str(aux), "-o", str(executable)], directory)
            run([str(executable)], directory)
            for test in tests:
                executable = directory / (test.stem + ".exe")
                run([compiler, *flags, *extra, str(test), "-o", str(executable)], directory)
                run([str(executable)], directory)
        base = re.search(r"```cpp\n(.*?)\n```", (root / "STYLE.md").read_text(
            encoding="utf-8"), re.DOTALL)[1]
        personal = work / "personal.hpp"
        personal.write_text(base + "\n", encoding="utf-8")
        print("[personal base] added tests and copied chapter examples", flush=True)
        for test in tests:
            executable = work / (test.stem + ".exe")
            run([compiler, *flags, "-include", str(personal), str(test),
                 "-o", str(executable)], work)
            run([str(executable)], work)
        tested_headers = {p.name.removesuffix("_test.cpp") + ".hpp" for p in tests}
        for example, reference in zip(examples, references):
            header = root / "sections" / reference
            if header.name not in tested_headers:
                continue
            source = work / "copied_example.cpp"
            # Match the PDF: paste the source, with no repository header dependency.
            content = header.read_text(encoding="utf-8").replace("#pragma once\n", "")
            source.write_text(base + "\n" + content + "\nint main()\n{\n"
                              + example + "\n}\n", encoding="utf-8")
            executable = work / "copied_example.exe"
            run([compiler, *flags, str(source), "-o", str(executable)], work)
            run([str(executable)], work)
    print(f"PASS: {len(headers)} headers/examples, 2 modes, multi-file linking; "
          f"{len(tests)} additional tests and copied examples with personal base", flush=True)


if __name__ == "__main__":
    main()
