"""Small exact/brute comparisons for selected chapter-13 derivations.

These are model tests, not AtCoder full-constraint submissions.
"""
from collections import Counter, defaultdict, deque
from itertools import combinations, permutations, product
from math import comb, gcd
from pathlib import Path
import random
import re

ROOT = Path(__file__).resolve().parents[2]
RG = random.Random(130026)


def interval_count(n, rules):
    states = {(): 1}
    active = []
    for i in range(1, n + 1):
        added = [j for j, (l, r, x) in enumerate(rules) if l == i]
        active += added
        nxt = defaultdict(int)
        for key, ways in states.items():
            key += tuple(rules[j][2] for j in added)
            for b in (0, 1):
                rem = tuple(x - b for x in key)
                if any(x < 0 or x > rules[j][1] - i
                       for j, x in zip(active, rem)):
                    continue
                out = tuple(x for j, x in zip(active, rem) if rules[j][1] > i)
                nxt[out] += ways
        active = [j for j in active if rules[j][1] > i]
        states = nxt
    return states.get((), 0)


def records(a, left, right):
    groups = sorted(Counter(a).items(), reverse=True)
    states = {(1, 1): 1}
    t = groups[0][1]
    for _, c in groups[1:]:
        nxt = defaultdict(int)
        for (l, r), ways in states.items():
            for x, y in product(range(2), repeat=2):
                rem, v = c - x - y, t - 1 + x + y
                coeff = (int(rem == 0) if v == 0 else
                         comb(rem + v - 1, v - 1) if rem >= 0 else 0)
                if rem >= 0:
                    nxt[l + x, r + y] += ways * coeff
        states, t = nxt, t + c
    return states.get((left, right), 0)


def record_pair(a):
    def one(b):
        best, cnt = -1, 0
        for x in b:
            if x > best:
                best, cnt = x, cnt + 1
        return cnt
    return one(a), one(a[::-1])


def cartesian(p, a):
    if not p:
        return [0]
    u = p.index(min(p))
    l, r = cartesian(p[:u], a[:u]), cartesian(p[u + 1:], a[u + 1:])
    f = [10**20] * (len(p) + 1)
    for k in range(max(len(l), len(r))):
        f[k] = min(l[k] if k < len(l) else 10**20,
                   r[k] if k < len(r) else 10**20)
    for i, x in enumerate(l):
        for j, y in enumerate(r):
            f[i + j + 1] = min(f[i + j + 1], x + y + a[u])
    return f


def numeral(s):
    v, length = list(s), [1] * len(s)
    for b in range((len(s) - 1).bit_length()):
        for mask in range(len(s)):
            if mask >> b & 1:
                t = mask ^ (1 << b)
                v[mask] = v[t] * 10**length[mask] + v[mask]
                length[mask] += length[t]
    return v


def mouth_brute(a):
    """Enumerate small feeding walks; optimal monotone walks need no excess
    on positive sites and at most one candy on each zero site.
    """
    cap = [max(1, x) for x in a]
    seen = {(p, (0,) * len(a)) for p in range(len(a))}
    todo = deque(seen)
    best = sum(a)
    while todo:
        p, b = todo.popleft()
        best = min(best, sum(abs(x - y) for x, y in zip(a, b)))
        for q in range(max(0, p - 1), min(len(a), p + 2)):
            if b[q] == cap[q]:
                continue
            c = b[:q] + (b[q] + 1,) + b[q + 1:]
            state = q, c
            if state not in seen:
                seen.add(state)
                todo.append(state)
    return best


def carry(a, s, t):
    q = Counter((sum(bits), sum(x * y for x, y in zip(a, bits)))
                for bits in product(range(2), repeat=len(a)))
    f = {(0, 0): 1}
    for bit in range(max(s, t).bit_length() + 5):
        g = defaultdict(int)
        sb, tb = s >> bit & 1, t >> bit & 1
        for (x, y), ways in f.items():
            for (u, v), c in q.items():
                xx, yy = x + u - sb, y + v - tb
                if xx >= 0 and yy >= 0 and xx % 2 == yy % 2 == 0:
                    g[xx // 2, yy // 2] += ways * c
        f = g
    return f.get((0, 0), 0)


def main():
    assert interval_count(3, [(1, 2, 1), (2, 3, 1)]) == 2
    assert interval_count(4, []) == 16
    assert interval_count(1, [(1, 1, 0), (1, 1, 1)]) == 0
    for _ in range(500):
        n = RG.randrange(1, 9)
        rules = []
        for j in range(RG.randrange(9)):
            l = RG.randrange(1, n + 1)
            r = RG.randrange(l, n + 1)
            rules.append((l, r, RG.randrange(r - l + 3)))
        brute = sum(all(sum(b[l - 1:r]) == x for l, r, x in rules)
                    for b in product(range(2), repeat=n))
        assert interval_count(n, rules) == brute, (n, rules)
    print('PASS interval exact: 500 random/brute cases, overlaps/zeros/conflicts/empty')
    for _ in range(100):
        n = RG.randrange(1, 8)
        a = [RG.randrange(1, 4) for _ in range(n)]
        counts = Counter(record_pair(p) for p in set(permutations(a)))
        for l, r in product(range(1, n + 1), repeat=2):
            assert records(a, l, r) == counts[l, r], (a, l, r)
        p = RG.sample(range(1, n + 1), n)
        weights = [RG.randrange(1, 10) for _ in range(n)]
        brute = [10**20] * (n + 1)
        for mask in range(1 << n):
            chosen = [i for i in range(n) if mask >> i & 1]
            if all(mask >> min(range(x, y + 1), key=p.__getitem__) & 1
                   for x, y in combinations(chosen, 2)):
                brute[len(chosen)] = min(brute[len(chosen)], sum(weights[i] for i in chosen))
        assert cartesian(p, weights) == brute
    print('PASS NDPC F/I: 100 multiset-permutation and RMQ-closure brute cases')
    for _ in range(150):
        n = RG.randrange(1, 6)
        board = [[RG.randrange(2) for _ in range(n)] for _ in range(n)]
        board[0][0] = 0
        paths = [[set() for _ in range(n)] for _ in range(n)]
        paths[0][0] = {0}
        for i, j in product(range(n), repeat=2):
            if i == j == 0:
                continue
            src = (paths[i - 1][j] if i else set()) | (paths[i][j - 1] if j else set())
            paths[i][j] = {x + board[i][j] for x in src}
            assert paths[i][j] == set(range(min(paths[i][j]), max(paths[i][j]) + 1))
        bits = RG.randrange(1, 5)
        s = [RG.randrange(1, 10) for _ in range(1 << bits)]
        slow = [int(''.join(str(s[t]) for t in range(len(s)) if t & mask == t))
                for mask in range(len(s))]
        assert numeral(s) == slow
        a = RG.sample(range(1, 10), RG.randrange(2, 8))
        f, g, sums = [1], [1], defaultdict(int)
        bound = max(a)
        # Use exact rational weights; no modular rounding involved.
        from fractions import Fraction as Fr
        h = [Fr(0)] * (bound + 1)
        for d in range(1, bound + 1):
            h[d] = Fr(1, d) - sum(h[k] for k in range(1, d) if d % k == 0)
        for j, x in enumerate(a):
            if j:
                f.append(sum(f[i] * (a[i] * x // gcd(a[i], x)) for i in range(j)))
                g.append(x * sum(h[d] * sums[d] for d in range(1, x + 1) if x % d == 0))
            for d in range(1, x + 1):
                if x % d == 0:
                    sums[d] += g[j] * x
        assert f == g
    print('PASS NDPC H/L/M: 150 path-count intervals, LCM DP and ordered-concatenation cases')
    for _ in range(100):
        a = [RG.randrange(1, 6) for _ in range(RG.randrange(1, 5))]
        s, t = RG.randrange(7), RG.randrange(25)
        brute = sum(sum(c) == s and sum(x * y for x, y in zip(a, c)) == t
                    for c in product(range(s + 1), repeat=len(a)))
        assert carry(a, s, t) == brute
    print('PASS NDPC J: 100 small double-carry vs integer enumeration cases')
    for _ in range(80):
        a = [RG.randrange(3) for _ in range(RG.randrange(1, 5))]
        cur = best = 0
        for x in a:
            cur = max(0, cur + (x if x else -1))
            best = max(best, cur)
        assert sum(a) - best == mouth_brute(a), a
    print('PASS NDPC G: 80 small feeding-walk enumerations vs maximum subarray')
    text = (ROOT / 'sections/13_dp_problems.typ').read_text(encoding='utf8')
    for contest, letters, slug in [('edp', 'abcdefghijklmnopqrstuvwxyz', 'dp_'),
                                    ('ndpc', 'abcdefghijklmnopqrst', 'ndpc2026_')]:
        for c in letters:
            assert f'<algo-dp-problem-{contest}-{c}>' in text
            assert f'/tasks/{slug}{c}' in text
    assert len(re.findall(r'^== ', text, re.M)) == 52
    assert len(re.findall(r'^= ', text, re.M)) == 1
    print('PASS inventory: 46 linked contest tasks and 6 additional models')


if __name__ == '__main__':
    main()
