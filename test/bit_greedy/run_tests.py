"""Check the bit-greedy templates, independent brute oracles and PDF copy examples."""
from pathlib import Path
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]


def main():
    text = (ROOT / "sections/19_bit_greedy.typ").read_text(encoding="utf8")
    blocks = re.split(r"(?m)^== ", text)[1:]
    if len(blocks) != 16:
        raise RuntimeError("Expected all 16 bit models")
    for block in blocks:
        if "简证" not in block or "记忆" not in block or "```cpp" not in block:
            raise RuntimeError("Missing proof, reminder or example: " + block.splitlines()[0])
    for script in ("run_chapter.py", "check_headers.py", "check_examples.py"):
        subprocess.run([sys.executable, str(ROOT / "test" / script), "bit_greedy"],
                       cwd=ROOT, check=True)
    print("PASS bit_greedy: 16 proof/model entries, C++17 runtime/header/copy checks", flush=True)


if __name__ == "__main__":
    main()
