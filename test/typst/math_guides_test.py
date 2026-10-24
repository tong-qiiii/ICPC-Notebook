"""Exact enumeration of the introductory worked examples; not general proofs."""
from collections import Counter
from fractions import Fraction as F
from itertools import product
from math import comb, gcd
from pathlib import Path
import re


def check():
    coins = list(product(range(2), repeat=3))
    assert F(sum(sum(x) == 2 for x in coins), len(coins)) == F(3, 8)
    pairs = list(product(range(2), repeat=2))
    eligible = [x for x in pairs if sum(x) >= 1]
    assert F(sum(sum(x) == 2 for x in eligible), len(eligible)) == F(1, 3)
    dist = Counter()
    for bits in pairs:
        w = F(1)
        for b, p in zip(bits, (F(1, 2), F(1, 3))):
            w *= p if b else 1 - p
        dist[sum(bits)] += w
    assert [dist[i] for i in range(3)] == [F(1, 3), F(1, 2), F(1, 6)]
    assignments = list(product(range(2), repeat=3))
    assert F(sum(len(set(x)) for x in assignments), 8) == F(7, 4)
    assert sum(len(set(x)) == 2 for x in assignments) == 6
    groups = {tuple(sorted((tuple(i for i, v in enumerate(x) if v == 0),
                            tuple(i for i, v in enumerate(x) if v == 1))))
              for x in assignments if len(set(x)) == 2}
    assert len(groups) == 3
    exact = Counter()
    for bits in product(range(2), repeat=3):
        if not any(bits):
            continue
        g = 0
        for take, a in zip(bits, (2, 3, 6)):
            if take:
                g = gcd(g, a)
        exact[g] += 1
    assert exact == {1: 2, 2: 2, 3: 2, 6: 1}
    forward = [sum(comb(n, k) * (k + 1) for k in range(n + 1))
               for n in range(3)]
    assert forward == [1, 3, 8]
    brackets = [x for x in product((-1, 1), repeat=4)
                if sum(x) == 0 and all(sum(x[:i]) >= 0 for i in range(5))]
    assert len(brackets) == 2
    fixed = [sum(x == x[r:] + x[:r] for x in coins) for r in range(3)]
    assert fixed == [8, 2, 2]
    assert len({min(x[r:] + x[:r] for r in range(3)) for x in coins}) == 4
    root = Path(__file__).resolve().parents[2]
    for name, count in [('11_probability', 18), ('12_counting', 40)]:
        text = (root / 'sections' / (name + '.typ')).read_text(encoding='utf8')
        assert len(re.findall(r'^== ', text, re.M)) == count
        assert text.count('*上手：*') == count
        prose = re.sub(r'```[\s\S]*?```', '', text)
        assert not any('^' in token for token in re.findall(r'`([^`\n]*)`', prose)), name
        assert '^' not in re.sub(r'\$[^$]*\$', '', prose), name
    print('PASS exact introductory examples and 58 topic guides; prose powers use math')


if __name__ == '__main__':
    check()
