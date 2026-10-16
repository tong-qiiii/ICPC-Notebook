"""Run a chapter's C++17 tests and compile the exact code shown in its PDF."""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
import json
import os
import re
import shutil
import sys
import tempfile
from check_headers import ROOT, execute, check


def run(topic):
    compiler = shutil.which(os.environ.get("CXX", "g++"))
    if not compiler:
        raise RuntimeError("C++ compiler not found; set CXX")
    base = re.search(r"```cpp\n(.*?)\n```", (ROOT / "STYLE.md").read_text(encoding="utf8"), re.S)[1]
    flags = ["-std=c++17", "-O2", "-Wall", "-Wextra", "-D_GLIBCXX_ASSERTIONS", "-ftrapv", "-I", str(ROOT)]
    units = sorted((ROOT / "test" / topic).glob("*_test.cpp"))
    if not units:
        raise RuntimeError("No runtime tests for " + topic)
    with tempfile.TemporaryDirectory(prefix="icpc_" + topic + "_") as temporary:
        work = Path(temporary)
        personal = work / "personal.hpp"
        personal.write_text(base, encoding="utf8")

        def one(source):
            exe = work / (source.stem + ".exe")
            execute([compiler, *flags, "-include", str(personal), str(source), "-o", str(exe)])
            return execute([str(exe)])

        failures = []
        with ThreadPoolExecutor(max_workers=2) as pool:
            futures = [(p, pool.submit(one, p)) for p in units]
            for source, future in futures:
                try:
                    print(future.result(), end="", flush=True)
                except Exception as error:
                    failures.append(source.name)
                    print(f"FAIL {source.name}: {error}", flush=True)
        if failures:
            raise RuntimeError("Failed tests: " + ", ".join(failures))
    print(f"PASS {topic}: {len(units)} runtime test units", flush=True)


if __name__ == "__main__":
    for topic in sys.argv[1:]:
        run(topic)
