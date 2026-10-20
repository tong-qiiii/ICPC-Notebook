"""Check the contest Bash scripts; real sanitizer cases require GNU/Linux."""
from pathlib import Path
import ast
import os
import re
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[2]
SCRIPTS = [ROOT / 'code/basic' / name for name in ('run', 'debug')]
FLAGS = {
    'run': ['-std=c++17', '-O2', '-Wall', '-Wextra', '-Wshadow'],
    'debug': ['-std=c++17', '-Og', '-g', '-Wall', '-fsanitize=address,undefined',
              '-D_GLIBCXX_DEBUG', '-fno-omit-frame-pointer'],
}


def find_bash():
    candidates = [os.environ.get('BASH')]
    if os.name == 'nt':
        git = shutil.which('git')
        if git:
            candidates.append(str(Path(git).resolve().parents[1] / 'bin/bash.exe'))
        candidates += ['D:/develop/Git/bin/bash.exe', 'C:/Program Files/Git/bin/bash.exe']
    candidates.append(shutil.which('bash'))
    for candidate in candidates:
        if not candidate or not Path(candidate).is_file():
            continue
        try:
            p = subprocess.run([candidate, '--noprofile', '--norc', '-c', 'printf ready'],
                               capture_output=True, timeout=10)
            if p.returncode == 0 and p.stdout == b'ready':
                return candidate
        except (OSError, subprocess.TimeoutExpired):
            pass
    raise RuntimeError('Working Bash not found; set BASH to its executable path')


def call(bash, script, stem, cwd, env):
    return subprocess.run([bash, '--noprofile', '--norc', './' + script, stem],
                          cwd=cwd, env=env, capture_output=True, text=True,
                          encoding='utf8', errors='replace', timeout=40)


def mock_checks(bash, work):
    (work / 'bin').mkdir()
    compiler = work / 'bin/g++'
    compiler.write_text('''#!/bin/bash
printf '%s\\n' "$@" > compiler.args
printf '%s\\n' "${ASAN_OPTIONS-}" "${UBSAN_OPTIONS-}" > compiler.env
if [ "${MOCK_COMPILE_FAIL-0}" = 1 ]; then exit 9; fi
cp mock_program "${@: -1}" || exit 10
chmod +x "${@: -1}"
''', encoding='utf8', newline='\n')
    compiler.chmod(0o755)
    (work / 'mock_program').write_text('''#!/bin/bash
touch ran
if [ "${MOCK_RUNTIME_FAIL-0}" = 1 ]; then echo failed >&2; exit 7; fi
read -r value
printf 'answer=%s\\n' "$value"
echo diagnostic >&2
''', encoding='utf8', newline='\n')
    env = os.environ.copy()
    env['PATH'] = str(work / 'bin') + os.pathsep + env.get('PATH', '')
    env.pop('ASAN_OPTIONS', None)
    env.pop('UBSAN_OPTIONS', None)
    stem = 'case one'
    (work / (stem + '.cpp')).write_text('int main() {}\n', encoding='utf8')
    for script in SCRIPTS:
        name = script.name
        input_file, output_file = work / (stem + '.in'), work / (stem + '.out')
        input_file.write_text('42\n', encoding='utf8')
        p = call(bash, name, stem, work, env)
        assert p.returncode == 0, p.stderr
        assert not p.stdout and 'diagnostic' in p.stderr
        assert output_file.read_text(encoding='utf8') == 'answer=42\n'
        args = (work / 'compiler.args').read_text(encoding='utf8').splitlines()
        assert args == FLAGS[name] + [stem + '.cpp', '-o', stem], args
        if name == 'debug':
            assert (work / 'compiler.env').read_text(encoding='utf8').splitlines() == [
                'handle_abort=1', 'print_stacktrace=1:halt_on_error=1']
        (work / 'ran').unlink()
        output_file.write_text('old output\n', encoding='utf8')
        p = call(bash, name, stem, work, dict(env, MOCK_COMPILE_FAIL='1'))
        assert p.returncode == 9 and not (work / 'ran').exists()
        assert output_file.read_text(encoding='utf8') == 'old output\n'
        input_file.unlink()
        p = call(bash, name, stem, work, env)
        assert p.returncode != 0 and not (work / 'ran').exists()
        input_file.write_text('42\n', encoding='utf8')
        p = call(bash, name, stem, work, dict(env, MOCK_RUNTIME_FAIL='1'))
        assert p.returncode == 7 and 'failed' in p.stderr
    print('PASS Bash: syntax, exact flags/environment, spaces, redirection, compile/runtime/input failures')


def linux_checks(bash, work):
    if not sys.platform.startswith('linux'):
        print('SKIP real GNU/Linux C++/sanitizer execution: current host is not Linux')
        return
    if not shutil.which('g++'):
        raise RuntimeError('g++ required for real Linux checks')
    env = os.environ.copy()
    source = work / 'a.cpp'
    input_file, output_file = work / 'a.in', work / 'a.out'
    source.write_text('#include <iostream>\nint main(){int x;std::cin>>x;std::cout<<x+1<<"\\n";}\n', encoding='utf8')
    input_file.write_text('41\n', encoding='utf8')
    for script in ('run', 'debug'):
        p = call(bash, script, 'a', work, env)
        assert p.returncode == 0, p.stderr
        assert output_file.read_text(encoding='utf8') == '42\n'
    input_file.write_text('2147483647\n', encoding='utf8')
    p = call(bash, 'debug', 'a', work, env)
    assert p.returncode != 0 and 'runtime error' in p.stderr, p.stderr
    source.write_text('#include <iostream>\nint main(){int i;std::cin>>i;int *p=new int[1];p[i]=7;std::cout<<p[i];delete[] p;}\n', encoding='utf8')
    input_file.write_text('2\n', encoding='utf8')
    p = call(bash, 'debug', 'a', work, env)
    assert p.returncode != 0 and 'AddressSanitizer' in p.stderr, p.stderr
    source.write_text('#include <vector>\n#include <iostream>\nint main(){std::vector<int> a(1);int i;std::cin>>i;std::cout<<a[i];}\n', encoding='utf8')
    p = call(bash, 'debug', 'a', work, env)
    assert p.returncode != 0 and 'out-of-bounds' in p.stderr, p.stderr
    print('PASS Linux: real C++17 run/debug, UBSan overflow, ASan heap overflow, STL bounds')


def main():
    bash = find_bash()
    chapter = (ROOT / 'sections/01_tools.typ').read_text(encoding='utf8')
    attributes = (ROOT / '.gitattributes').read_text(encoding='utf8')
    cpp_refs = re.findall(r'#raw\(read\("([^\"]+\.hpp)"\)', chapter)
    headers = set((ROOT / 'code/basic').glob('*.hpp'))
    assert len(cpp_refs) == len(headers)
    assert {(ROOT / 'sections' / p).resolve() for p in cpp_refs} == {p.resolve() for p in headers}
    ast.parse((ROOT / 'test/basic/run_tests.py').read_text(encoding='utf8'))
    for script in SCRIPTS:
        data = script.read_bytes()
        assert data.startswith(b'#!/bin/bash\n') and b'\r' not in data
        assert chapter.count(f'#raw(read("../code/basic/{script.name}"), lang: "bash", block: true)') == 1
        assert f'/code/basic/{script.name} text eol=lf' in attributes
        subprocess.run([bash, '-n', script.as_posix()], check=True, timeout=10)
    with tempfile.TemporaryDirectory(prefix='icpc_shell_') as temporary:
        work = Path(temporary)
        for script in SCRIPTS:
            copy = work / script.name
            shutil.copyfile(script, copy)
            copy.chmod(0o755)
        mock_checks(bash, work)
        linux_checks(bash, work)


if __name__ == '__main__':
    main()
