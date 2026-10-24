"""Small independent checks of the ten additional segment-tree models."""
from itertools import combinations
from math import gcd, isqrt
import random

RG = random.Random(141034)


def run():
    for _ in range(400):
        a = [RG.randrange(-9, 10) for _ in range(RG.randrange(1, 10))]
        x, y, u, v = [RG.randrange(-4, 5) for _ in range(4)]
        got = [u*(x*t+y)+v for t in a]
        assert got == [(u*x)*t+(u*y+v) for t in a]
        assert sum(got) == u*x*sum(a)+(u*y+v)*len(a)
        bits = [RG.randrange(2) for _ in a]
        inv = sum(bits[i] > bits[j] for i, j in combinations(range(len(a)), 2))
        b = [1-t for t in bits]
        assert sum(b[i] > b[j] for i, j in combinations(range(len(a)), 2)) == bits.count(0)*bits.count(1)-inv
        # Empty subarrays allowed: maximal/minimal sums exchange under negation.
        def summary(b):
            pre = [sum(b[:i]) for i in range(len(b)+1)]
            suf = [sum(b[i:]) for i in range(len(b)+1)]
            sums = [sum(b[i:j]) for i in range(len(b)+1) for j in range(i, len(b)+1)]
            return sum(b), max(pre), min(pre), max(suf), min(suf), max(sums), min(sums)
        s = summary(a)
        assert summary([-t for t in a]) == (-s[0], -s[2], -s[1], -s[4], -s[3], -s[6], -s[5])
        for _ in range(6):
            l, r = sorted([RG.randrange(len(a)), RG.randrange(len(a))])
            d = [a[0]]+[a[i]-a[i-1] for i in range(1, len(a))]
            add = RG.randrange(-7, 8)
            d[l] += add
            if r+1 < len(a):
                d[r+1] -= add
            a[l:r+1] = [t+add for t in a[l:r+1]]
            assert all(sum(d[:i+1]) == a[i] for i in range(len(a)))
            want = 0
            for t in a[l:r+1]:
                want = gcd(want, t)
            got = abs(sum(d[:l+1]))
            for t in d[l+1:r+1]:
                got = gcd(got, t)
            assert got == want
        l, r = sorted([RG.randrange(len(a)), RG.randrange(len(a))])
        k, step = RG.randrange(-5, 6), RG.randrange(-5, 6)
        direct = sum(k+(i-l)*step for i in range(l, r+1))
        assert direct == step*sum(i+1 for i in range(l, r+1))+(k-step*(l+1))*(r-l+1)
        # Beats fast chmin condition, including equality at the second maximum.
        vals = sorted(set(a))
        cap = RG.randrange(-12, 13)
        mx, second = vals[-1], vals[-2] if len(vals) > 1 else -10**9
        if second < cap < mx:
            assert sum(min(t, cap) for t in a) == sum(a)+(cap-mx)*a.count(mx)
        for value in range(80):
            for mod in range(1, 15):
                if value >= mod:
                    assert value % mod < value/2
            q = isqrt(value)
            assert q*q <= value < (q+1)*(q+1)
        # Prefix-version count differences and prefix-k sum; duplicate values retained.
        sub = sorted(a[l:r+1])
        for k in range(len(sub)+1):
            c, acc = 0, 0
            for value in sorted(set(a)):
                available = a[:r+1].count(value)-a[:l].count(value)
                take = min(k-c, available)
                acc, c = acc+take*value, c+take
            assert c == k and acc == sum(sub[:k])
        # Binary run summaries combine across a boundary and swap on flip.
        def runs(b):
            def one(v):
                pre = next((i for i, x in enumerate(b) if x != v), len(b))
                suf = next((i for i, x in enumerate(b[::-1]) if x != v), len(b))
                best, cur = 0, 0
                for x in b:
                    cur = cur+1 if x == v else 0
                    best = max(best, cur)
                return pre, suf, best
            return len(b), one(0), one(1)
        p = RG.randrange(len(bits)+1)
        L, R = runs(bits[:p]), runs(bits[p:])
        got = []
        for v in (0, 1):
            pre, suf, best = L[v+1]
            P, S, B = R[v+1]
            got.append((pre+(P if pre == L[0] else 0),
                        S+(suf if S == R[0] else 0), max(best, B, suf+P)))
        assert (len(bits), *got) == runs(bits)
        assert runs([1-x for x in bits]) == (len(bits), *runs(bits)[1:][::-1])

    # Sweep event formula for area AND perimeter, checked against unit-cell union.
    for _ in range(400):
        rects = []
        cells = set()
        for _ in range(RG.randrange(8)):
            x1, x2 = sorted([RG.randrange(-3, 5), RG.randrange(-3, 5)])
            y1, y2 = sorted([RG.randrange(-3, 5), RG.randrange(-3, 5)])
            if x1 == x2 or y1 == y2:
                continue
            rects.append((x1, y1, x2, y2))
            cells.update((x, y) for x in range(x1, x2) for y in range(y1, y2))
        events = sorted([(x1, 1, y1, y2) for x1, y1, x2, y2 in rects]
                        + [(x2, -1, y1, y2) for x1, y1, x2, y2 in rects],
                        key=lambda e: (e[0], -e[1]))
        counts, last, area, perimeter = {}, events[0][0] if events else 0, 0, 0
        for x, delta, l, r in events:
            active = sorted(y for y, c in counts.items() if c)
            segments = sum(i == 0 or y != active[i-1]+1 for i, y in enumerate(active))
            area += len(active)*(x-last)
            perimeter += 2*segments*(x-last)
            before = len(active)
            for y in range(l, r):
                counts[y] = counts.get(y, 0)+delta
                assert counts[y] >= 0
            perimeter += abs(sum(c > 0 for c in counts.values())-before)
            last = x
        want = sum((x+dx, y+dy) not in cells for x, y in cells
                   for dx, dy in ((-1, 0), (1, 0), (0, -1), (0, 1)))
        assert (area, perimeter) == (len(cells), want)
    print('PASS ten classic segment-tree models: actions, merges, differences, sweep perimeter, order statistics', flush=True)


if __name__ == '__main__':
    run()
