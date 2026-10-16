"""Run every chapter suite; a failed chapter never gets reported as passing."""
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
TOPICS = ["basic", "graph", "tree", "data_structure", "interval_greedy",
          "math", "geometry", "poly", "string", "dp", "probability", "counting"]


def main():
    failed = []
    for topic in TOPICS:
        print(f"\n=== {topic} ===", flush=True)
        result = subprocess.run([sys.executable, "-X", "utf8", str(
            ROOT / "test" / topic / "run_tests.py")], cwd=ROOT)
        if result.returncode:
            failed.append(topic)
        print(f"{'FAIL' if result.returncode else 'PASS'} CHAPTER {topic}", flush=True)
    if failed:
        raise SystemExit("Failed chapters: " + ", ".join(failed))
    for script in ["check_manifest.py", "check_examples.py"]:
        subprocess.run([sys.executable, "-X", "utf8", str(ROOT / "test" / script)],
                       cwd=ROOT, check=True)
    print(f"PASS ALL {len(TOPICS)} CHAPTERS", flush=True)


if __name__ == "__main__":
    main()
