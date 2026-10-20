"""Check the common-model additions without rebuilding a complete PDF."""
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import json
import os
import re
import shutil
import sys
import tempfile
from check_headers import ROOT, execute
from notebook_sources import audit, examples


def main():
    entries = json.loads((ROOT / 'test/additions_manifest.json').read_text(encoding='utf8'))
    audit()
    headers = [f for e in entries for f in e['templates']]
    assert len(set(headers)) == len(headers)
    for e in entries:
        section = (ROOT / e['section']).read_text(encoding='utf8')
        assert section.count('<' + e['label'] + '>') >= 1
        assert re.search(r'(?m)^== .* <' + re.escape(e['label']) + r'>$', section)
        for f in e['templates'] + e['tests']:
            assert (ROOT / f).is_file(), f
    sys.path.insert(0, str(ROOT / 'test/typst'))
    from numbering_test import expected_headings
    sections = {p.name: p.read_text(encoding='utf8') for p in (ROOT / 'sections').glob('*.typ')}
    expected = expected_headings((ROOT / 'main.typ').read_text(encoding='utf8'), sections)
    print(f'PASS additions inventory: {len(headers)} sources and labels; '
          f'{len(expected)} numbered headings', flush=True)
    if '--static-only' in sys.argv:
        return
    compiler = shutil.which(os.environ.get('CXX', 'g++'))
    if not compiler:
        raise RuntimeError('C++ compiler missing')
    base = re.search(r'```cpp\n(.*?)\n```', (ROOT / 'STYLE.md').read_text(encoding='utf8'), re.S)[1]
    flags = ['-std=c++17', '-O2', '-Wall', '-Wextra', '-D_GLIBCXX_ASSERTIONS',
             '-ftrapv', '-Werror=c++20-extensions', '-Werror=c++23-extensions', '-I', str(ROOT)]
    selected = {h.removeprefix('code/') for h in headers}
    copied = [x for x in examples() if x[0] in selected]
    assert len(copied) == len(headers)
    tests = [f for e in entries for f in e['tests']]
    tests += ['test/graph/flow_matching_test.cpp', 'test/graph/connectivity_test.cpp',
              'test/math/crt_test.cpp', 'test/math/multi_sieve_test.cpp']
    with tempfile.TemporaryDirectory(prefix='icpc_additions_') as temporary:
        work = Path(temporary)
        personal = work / 'personal.hpp'
        personal.write_text(base, encoding='utf8')

        def header_check(h):
            stem = Path(h).stem
            for mode, prefix in [('plain', ''), ('personal', base)]:
                p = work / (stem + '_' + mode + '.cpp')
                p.write_text(prefix + f'\n#include "{h}"\n#include "{h}"\nint main() {{}}\n', encoding='utf8')
                execute([compiler, *flags, '-fsyntax-only', str(p)])
            other = work / (stem + '_other.cpp')
            other.write_text(f'#include "{h}"\n', encoding='utf8')
            execute([compiler, *flags, str(work / (stem + '_plain.cpp')), str(other),
                     '-o', str(work / (stem + '_link.exe'))])

        with ThreadPoolExecutor(max_workers=2) as pool:
            list(pool.map(header_check, headers))
        print(f'PASS {len(headers)} headers: standalone, personal macros, duplicate include, multiple TUs', flush=True)

        def test_run(f):
            out = work / (Path(f).stem + '.exe')
            execute([compiler, *flags, '-include', str(personal), str(ROOT / f), '-o', str(out)])
            return execute([str(out)], timeout=180)

        with ThreadPoolExecutor(max_workers=2) as pool:
            for result in pool.map(test_run, tests):
                print(result, end='', flush=True)
        print(f'PASS {len(tests)} correctness units', flush=True)

        def example_run(item):
            name, source, example = item
            p = work / (Path(name).stem + '_example.cpp')
            p.write_text(base + '\n' + source + '\nint main()\n{\n' + example + '\n}\n', encoding='utf8')
            out = p.with_suffix('.exe')
            execute([compiler, *flags, str(p), '-o', str(out)])
            execute([str(out)])

        with ThreadPoolExecutor(max_workers=2) as pool:
            list(pool.map(example_run, copied))
        print(f'PASS {len(copied)} copied PDF examples, dependencies included exactly once', flush=True)


if __name__ == '__main__':
    main()
