from pathlib import Path
import subprocess
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from check_headers import check
from run_chapter import run

if __name__ == "__main__":
    check("counting")
    run("counting")
    subprocess.run([sys.executable, "-X", "utf8", str(
        Path(__file__).with_name("formula_checks.py"))], check=True)
