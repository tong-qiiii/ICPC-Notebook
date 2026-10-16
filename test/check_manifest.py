"""Audit immutable raw backups, source coverage and unique chapter imports."""
from pathlib import Path
import hashlib
import json
import re

ROOT = Path(__file__).resolve().parents[1]


def main():
    manifest = json.loads((ROOT / "test/migration_manifest.json").read_text(encoding="utf8"))
    main_source = (ROOT / "main.typ").read_text(encoding="utf8")
    includes = re.findall(r'(?m)^#include "sections/([^"]+)"', main_source)
    chapters = list((ROOT / "sections").glob("*.typ"))
    assert len(includes) == len(set(includes)) == len(chapters)
    assert set(includes) == {p.name for p in chapters}
    headers = set()
    for chapter in manifest:
        path = ROOT / chapter["raw"]
        assert hashlib.sha256(path.read_bytes()).hexdigest() == chapter["raw_sha256"], path
        original = path.read_text(encoding="utf8")
        headings = re.findall(r"(?m)^## (.+)", original)
        assert headings == [item["heading"] for item in chapter["items"]], path
        for item in chapter["items"]:
            for name in item["templates"]:
                assert name not in headers, name
                headers.add(name)
                assert (ROOT / name).is_file(), name
                template = Path(name)
                assert (ROOT / "test" / template.parent.name /
                        (template.stem + "_test.cpp")).is_file(), name
    actual = {p.relative_to(ROOT).as_posix() for topic in manifest for p in
              (ROOT / "code" / topic["topic"]).glob("*.hpp")}
    assert headers == actual, headers.symmetric_difference(actual)
    print(f"PASS manifest: {len(chapters)} chapters, {sum(len(c['items']) for c in manifest)} raw items, "
          f"{len(headers)} new templates, unchanged raw hashes", flush=True)


if __name__ == "__main__":
    main()
