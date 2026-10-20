"""Audit source deduplication and reconstruct exactly the PDF's copyable code."""
from collections import Counter
from dataclasses import dataclass
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
CODE = ROOT / "code"
SOURCE = re.compile(r'#source_code\(\((.*?)\)(?:,\s*shared:\s*<([^>]+)>)?\)')
GRAPH = re.compile(r'#graph_code\("([^"]+)"(?:,\s*shared:\s*<([^>]+)>)?\)')
LABEL = re.compile(r'(?m)^={1,4} .* <([^>]+)>\s*$')


@dataclass
class Source:
    file: str
    shared: str | None
    start: int
    end: int


def source_calls(text):
    calls = []
    for match in SOURCE.finditer(text):
        files = re.findall(r'"([^"]+)"', match[1])
        assert len(files) == 1, "Each PDF block must display only its own file"
        calls.append(Source(files[0], match[2], match.start(), match.end()))
    for match in GRAPH.finditer(text):
        calls.append(Source("graph/" + match[1], match[2], match.start(), match.end()))
    return sorted(calls, key=lambda call: call.start)


def dependency_order(file):
    """Read the real local include graph, with prerequisites before each user."""
    done, active, result = set(), set(), []

    def visit(name):
        if name in done:
            return
        assert name not in active, f"Cyclic dependency: {name}"
        active.add(name)
        path = CODE / name
        for include in re.findall(r'(?m)^#include "([^"]+)"', path.read_text(encoding="utf8")):
            dep = (path.parent / include).resolve().relative_to(CODE.resolve()).as_posix()
            visit(dep)
        active.remove(name)
        done.add(name)
        result.append(name)

    visit(file)
    return result


def expand(paths):
    chunks = []
    for file in paths:
        text = (CODE / file).read_text(encoding="utf8")
        # Match styles/source.typ; the PDF never requires repository includes.
        chunks.append(re.sub(r'(?m)^#pragma once\n|^#include ".*"\n', "", text))
    return "\n".join(chunks)


def chapters():
    entry = (ROOT / "main.typ").read_text(encoding="utf8")
    return [ROOT / "sections" / name for name in
            re.findall(r'(?m)^#include "sections/([^"]+)"', entry)]


def source_anchors():
    targets = {}
    for path in chapters():
        text = path.read_text(encoding="utf8")
        labels = list(LABEL.finditer(text))
        for call in source_calls(text):
            label = [m for m in labels if m.start() < call.start][-1][1]
            targets.setdefault(label, set()).add(call.file)
    return targets


def copy_order(file, note, anchors):
    """Include explicit PDF dependencies too, e.g. a caller-selected Mint type."""
    order = []
    line = re.search(r'(?m)^\*代码依赖：\* (.*)$', note)
    if line:
        for label in re.findall(r'#(?:link|ref)\(<([^>]+)>', line[1]):
            assert label in anchors, ("Missing dependency target", label)
            candidates = anchors[label] - {file}
            if len(candidates) > 1:
                # One algorithm heading may own several important variants.
                candidates &= set(dependency_order(file)[:-1])
            assert candidates, ("Ambiguous dependency", label, file)
            for dep in sorted(candidates):
                for item in dependency_order(dep):
                    if item not in order:
                        order.append(item)
    for item in dependency_order(file):
        if item not in order:
            order.append(item)
    return order


def examples(selected=None):
    result = []
    anchors = source_anchors()
    for path in selected or chapters():
        text = path.read_text(encoding="utf8")
        first = re.search(r'(?m)^== ', text)
        if not first:
            continue
        previous = first.start()
        for call in source_calls(text):
            if call.start < first.start():
                continue  # Chapter-front shared sources have no separate example.
            calls = re.findall(r'```cpp\n(.*?)\n```', text[previous:call.start], re.S)
            assert len(calls) == 1, f"Expected one minimal example: {path.name}: {call.file}"
            note = text[previous:call.start]
            result.append((call.file, expand(copy_order(call.file, note, anchors)), calls[0]))
            previous = call.end
    return result


def audit():
    shown, anchors, uses, source_labels = {}, {}, [], set()
    position = 0
    for path in chapters():
        text = path.read_text(encoding="utf8")
        labels = list(LABEL.finditer(text))
        source_labels.update(match[1] for match in labels)
        first = re.search(r'(?m)^== ', text).start()
        previous = first
        for call in source_calls(text):
            if call.shared or call.start >= first:
                uses.append((call, position, text[previous:call.start]))
            if not call.shared:
                assert call.file not in shown, f"Repeated PDF source: {call.file}"
                shown[call.file] = position
                heading = [m for m in labels if m.start() < call.start][-1][1]
                anchors[call.file] = heading
            if call.start >= first:
                previous = call.end
            else:
                assert not call.shared, "Chapter-front skeletons must actually print their source"
            position += 1
    expected = {p.relative_to(CODE).as_posix() for p in CODE.glob("*/*.hpp")
                if p.parent.name not in ["basic", "tree"]}
    assert set(shown) == expected, ("Missing/extra visible source", expected ^ set(shown))
    copied = examples()
    assert Counter(name for name, _, _ in copied) == Counter(expected), "Algorithm/example coverage"
    targets = source_anchors()
    for call, order, note in uses:
        if call.shared:
            assert call.shared in source_labels, f"Missing skeleton label: {call.shared}"
            assert anchors[call.file] == call.shared, f"Skeleton link points elsewhere: {call.file}"
            assert shown[call.file] < order, f"Skeleton must be printed before use: {call.file}"
        for dep in copy_order(call.file, note, targets)[:-1]:
            assert dep in shown and shown[dep] < order, f"Dependency not printed before use: {call.file}: {dep}"
            assert f'<{anchors[dep]}>' in note, f"Missing visible dependency link: {call.file}: {dep}"
    print(f"PASS PDF sources: {len(shown)} distinct files, each printed once; "
          f"{len(copied)} examples with visible prerequisite links", flush=True)
    return shown


if __name__ == "__main__":
    audit()
