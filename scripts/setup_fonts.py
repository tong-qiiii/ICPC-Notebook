"""Prepare the reference layout's fonts for local and GitHub Actions builds.

Source: https://github.com/notofonts/noto-cjk/tree/Serif2.003/Serif
License: https://github.com/notofonts/noto-cjk/blob/Serif2.003/Serif/LICENSE
Cover: https://github.com/lxgw/LxgwWenKai/releases/tag/v1.522
License: https://github.com/lxgw/LxgwWenKai/blob/v1.522/OFL.txt
"""

import hashlib
from pathlib import Path
import shutil
import urllib.request


FONT_DIR = Path(__file__).resolve().parents[1] / ".cache" / "notebook-fonts"
BASE_URL = (
    "https://raw.githubusercontent.com/notofonts/noto-cjk/Serif2.003/"
    "Serif/OTF/SimplifiedChinese/"
)
FONTS = {
    "NotoSerifCJKsc-Regular.otf":
        "2a2eae2628df83556c54018c41e20fa532c1b862c5256ae8b3f23feb918d12ca",
    "NotoSerifCJKsc-Bold.otf":
        "8af07d4b6c2e82bcc72a30e066eaf295f11b9424f4aad2eaa9fe0e9c3b38fc73",
    "LXGWWenKai-Regular.ttf":
        "39ad71264b588165b469e35e6afb162a378dacd1f95348160240ba9038ac3009",
}
FONT_URLS = {
    "LXGWWenKai-Regular.ttf": (
        "https://github.com/lxgw/LxgwWenKai/releases/download/"
        "v1.522/LXGWWenKai-Regular.ttf"
    ),
}


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare_fonts(directory=FONT_DIR):
    directory.mkdir(parents=True, exist_ok=True)
    for name, expected in FONTS.items():
        target = directory / name
        if target.is_file() and digest(target) == expected:
            print(f"Verified: {name}")
            continue

        partial = directory / (name + ".part")
        print(f"Downloading: {name}", flush=True)
        try:
            request = urllib.request.Request(
                FONT_URLS.get(name, BASE_URL + name),
                headers={"User-Agent": "ICPC-Notebook"},
            )
            with urllib.request.urlopen(request, timeout=60) as response:
                with partial.open("wb") as output:
                    shutil.copyfileobj(response, output)
            if digest(partial) != expected:
                raise RuntimeError(f"SHA-256 mismatch: {name}")
            partial.replace(target)
            print(f"Verified: {name}")
        finally:
            partial.unlink(missing_ok=True)
    print(f"Font directory: {directory}")


if __name__ == "__main__":
    try:
        prepare_fonts()
    except (OSError, RuntimeError) as error:
        raise SystemExit(f"Font setup failed: {error}") from error
