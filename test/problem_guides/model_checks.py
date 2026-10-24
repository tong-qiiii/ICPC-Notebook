"""Run chapter 15-18 model checks plus existing-interface C++17 tests.

These checks are not full contest submissions or general correctness proofs.
No full PDF build, network access or performance benchmarking is performed.
"""
from pathlib import Path
import json
import os
import re
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]


def inventory():
    all_text = '\n'.join(p.read_text(encoding='utf8') for p in (ROOT / 'sections').glob('*.typ'))
    labels = re.findall(r'^=+ .* <([a-z0-9_-]+)>$', all_text, re.M)
    assert len(labels) == len(set(labels)), 'Duplicate stable heading label'
    topics = ['15_tree_problems', '16_graph_problems', '17_string_problems', '18_offline_problems']
    main = (ROOT / 'main.typ').read_text(encoding='utf8')
    manifest = json.loads((ROOT / 'test/migration_manifest.json').read_text(encoding='utf8'))
    for name in topics:
        path = f'sections/{name}.typ'
        assert f'#include "{path}"' in main
        assert any(c.get('section') == path for c in manifest)
        text = (ROOT / path).read_text(encoding='utf8')
        blocks = re.split(r'^== ', text, flags=re.M)[1:]
        assert len(blocks) == 8, name
        assert not re.search(r'^=+ \d+\.\d+', text, re.M)
        assert 'source_code(' not in text and 'read(' not in text
        for block in blocks:
            assert '<algo-' in block.splitlines()[0]
            assert all(word in block for word in ('例', '记忆', 'O(')), block[:50]
        for ref in re.findall(r'#ref\(<([^>]+)>', text):
            assert ref in labels, ref
    text = (ROOT / 'sections/14_segment_tree_problems.typ').read_text(encoding='utf8')
    assert len(re.findall(r'^== ', text, re.M)) == 34
    assert len(re.findall(r'<algo-seg-model-[^>]+>', text)) == 10
    print('PASS inventory: 4 chapters x 8 models and 10 segment-tree additions; imports and stable links', flush=True)


def cpp_interfaces():
    compiler = shutil.which(os.environ.get('CXX', 'g++'))
    if not compiler:
        raise RuntimeError('C++ compiler unavailable; set CXX')
    with tempfile.TemporaryDirectory(prefix='icpc_guide_tests_') as tmp:
        exe = Path(tmp) / ('interfaces.exe' if os.name == 'nt' else 'interfaces')
        subprocess.run([compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                        '-D_GLIBCXX_ASSERTIONS', '-ftrapv', '-I', str(ROOT),
                        str(Path(__file__).with_name('interfaces_test.cpp')), '-o', str(exe)],
                       check=True, timeout=120)
        subprocess.run([str(exe)], check=True, timeout=60)


def main():
    from tree_checks import run as tree
    from graph_checks import run as graph
    from string_checks import run as string
    from offline_checks import run as offline
    inventory()
    for check in (tree, graph, string, offline):
        check()
    cpp_interfaces()
    print('PASS all guide checks; full-constraint submissions and unimplemented extensions remain unverified.', flush=True)


if __name__ == '__main__':
    main()
