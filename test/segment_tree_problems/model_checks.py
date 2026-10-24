"""Exact small model checks for chapter 14, not full AtCoder submissions.

No performance benchmarks or dependency on .cache / third-party packages.
Run: python test/segment_tree_problems/model_checks.py
"""
from itertools import combinations, permutations, product
from math import comb
from pathlib import Path
import random
import re

ROOT = Path(__file__).resolve().parents[2]
RG = random.Random(140024)
NEG = -10**30


def fold(a, merge, empty):
    for x in a:
        empty = merge(empty, x)
    return empty


def check_inventory():
    text = (ROOT / 'sections/14_segment_tree_problems.typ').read_text(encoding='utf8')
    headings = re.findall(r'^== AAL ([A-X]) .* <(algo-[^>]+)>$', text, re.M)
    assert [x for x, _ in headings] == list('ABCDEFGHIJKLMNOPQRSTUVWX')
    assert len({label for _, label in headings}) == 24
    for letter, _ in headings:
        assert f'/tasks/aalc001_{letter.lower()}' in text
    assert len(re.findall(r'/editorial/\d+', text)) == 24
    all_sections = '\n'.join(p.read_text(encoding='utf8')
                             for p in (ROOT / 'sections').glob('*.typ'))
    for label in re.findall(r'#ref\(<([^>]+)>', text):
        assert len(re.findall(rf'(?m)^=+ .* <{re.escape(label)}>$', all_sections)) == 1, label
    assert 'source_code(' not in text and 'read(' not in text
    assert not re.search(r'^=+ \d+\.\d+', text, re.M)


def check_small_alphabets():
    def inv_merge(a, b):
        c, v = a
        d, w = b
        return (tuple(x+y for x, y in zip(c, d)),
                v+w+c[1]*d[0]+c[2]*(d[0]+d[1]))

    def rgb_merge(a, b):
        r, g, bl, rg, gb, rgb = a
        R, G, B, RG, GB, RGB = b
        return r+R, g+G, bl+B, rg+RG+r*G, gb+GB+g*B, rgb+RGB+rg*B+r*GB

    for _ in range(400):
        n = RG.randrange(10)
        a = [RG.randrange(3) for _ in range(n)]
        leaves = [(tuple(int(x == j) for j in range(3)), 0) for x in a]
        got = fold(leaves, inv_merge, ((0, 0, 0), 0))
        assert got[1] == sum(a[i] > a[j] for i, j in combinations(range(n), 2))
        w = [RG.randrange(6) for _ in a]
        leaves = [tuple(w[i] if a[i] == j else 0 for j in range(3))+(0, 0, 0)
                  for i in range(n)]
        got = fold(leaves, rgb_merge, (0,)*6)
        want = sum(w[i]*w[j]*w[k] for i, j, k in combinations(range(n), 3)
                   if (a[i], a[j], a[k]) == (0, 1, 2))
        assert got[5] == want
        p, q = sorted([RG.randrange(n+1), RG.randrange(n+1)])
        x, y, z = [fold(part, rgb_merge, (0,)*6)
                   for part in (leaves[:p], leaves[p:q], leaves[q:])]
        assert rgb_merge(rgb_merge(x, y), z) == rgb_merge(x, rgb_merge(y, z))


def check_runs_and_ranks():
    def merge(a, b):
        if not a[0]:
            return b
        if not b[0]:
            return a
        n, l, r, pre, suf, best = a
        m, L, R, Pre, Suf, Best = b
        same = r == L
        return (n+m, l, R, pre+(Pre if pre == n and same else 0),
                Suf+(suf if Suf == m and same else 0),
                max(best, Best, suf+Pre if same else 0))

    def ranked(a, b):
        n, s, w = a
        m, t, v = b
        return n+m, s+t, w+v+n*t

    empty = (0, 0, 0, 0, 0, 0)
    for _ in range(500):
        a = [RG.randrange(4) for _ in range(RG.randrange(15))]
        leaves = [(1, x, x, 1, 1, 1) for x in a]
        got = fold(leaves, merge, empty)
        best, cur, last = 0, 0, None
        for x in a:
            cur = cur+1 if x == last else 1
            best, last = max(best, cur), x
        assert got[-1] == best
        p = RG.randrange(len(a)+1)
        assert merge(fold(leaves[:p], merge, empty), fold(leaves[p:], merge, empty)) == got
        ordered = sorted(a)
        info = fold([(1, x, x) for x in ordered], ranked, (0, 0, 0))
        assert info == (len(a), sum(a), sum((i+1)*x for i, x in enumerate(ordered)))
        for x in set(a):
            c = a.count(x)
            assert fold([(1, x, x)]*c, ranked, (0, 0, 0)) == (c, c*x, c*(c+1)//2*x)


def check_power_tags():
    mod = 998244353

    def act(s, a):
        return [sum(comb(j, t)*pow(a, j-t, mod)*s[t] for t in range(j+1)) % mod
                for j in range(5)]

    for _ in range(400):
        a = [RG.randrange(-20, 21) for _ in range(RG.randrange(12))]
        s = [sum(pow(x, j, mod) for x in a) % mod for j in range(5)]
        x, y = RG.randrange(-10, 11), RG.randrange(-10, 11)
        want = [sum(pow(v+x, j, mod) for v in a) % mod for j in range(5)]
        assert act(s, x) == want
        assert act(act(s, x), y) == act(s, x+y)


def check_add_mod():
    for p in range(1, 12):
        def merge(l, r):
            lm, a, b = l
            rm, c, d = r
            if not rm:
                return (True, a, b+c) if lm else (False, a+c, 0)
            return True, (a+(b if lm else 0)+c) % p, d

        def apply(f, x):
            has, a, b = f
            return (x+a) % p+b if has else x+a

        for _ in range(100):
            a = [RG.randrange(6) for _ in range(RG.randrange(10))]
            leaves = [(False, x, 0) if x else (True, 0, 0) for x in a]
            f = fold(leaves, merge, (False, 0, 0))
            for x in range(20):
                want = x
                for v in a:
                    want = want+v if v else want % p
                assert apply(f, x) == want


def check_two_segments():
    for _ in range(250):
        a = [RG.randrange(-5, 6) for _ in range(RG.randrange(1, 9))]
        dp = [0]+[NEG]*4
        for x in a:
            dp = [max(dp[:j+1])+(x if j in (1, 3) else 0) for j in range(5)]
        want = 0
        for bits in product((0, 1), repeat=len(a)):
            runs = sum(b and (i == 0 or not bits[i-1]) for i, b in enumerate(bits))
            if runs <= 2:
                want = max(want, sum(x*b for x, b in zip(a, bits)))
        assert max(dp) == want


def check_permutations_and_nand():
    for p, q in product(permutations(range(3)), repeat=2):
        a = [10, 20, 30]
        once = [a[i] for i in p]
        assert [once[i] for i in q] == [a[p[q[i]]] for i in range(3)]
    for _ in range(400):
        a = [RG.randrange(-7, 8) for _ in range(RG.randrange(1, 15))]
        prefixes = [sum(a[:i]) for i in range(1, len(a)+1)]
        p = RG.randrange(1, len(a)+1)
        left = max(prefixes[:p])
        if p < len(a):
            right = max(sum(a[p:i]) for i in range(p+1, len(a)+1))
            assert max(left, sum(a[:p])+right) == max(prefixes)
        mask = (1 << 30)-1
        nums = [RG.getrandbits(30) for _ in range(RG.randrange(1, 12))]
        f0, f1 = 0, mask
        for v in nums[1:]:
            f0, f1 = ~(f0 & v) & mask, ~(f1 & v) & mask
        x = nums[0]
        for v in nums[1:]:
            x = ~(x & v) & mask
        assert ((~nums[0] & f0) | (nums[0] & f1)) & mask == x


def check_history_tags():
    def compose(a, b):
        u, v, w, z = a
        U, V, W, Z = b
        return u+U*w, v+U*z+V, W*w, W*z+Z

    def act(tag, h, c):
        u, v, w, z = tag
        return h+u*c+v, w*c+z

    for _ in range(400):
        tags = [(0, 0, 0, RG.randrange(-5, 6)) if RG.randrange(2) else (1, 0, 1, 0)
                for _ in range(12)]
        merged = fold(tags, compose, (0, 0, 1, 0))
        h, c = RG.randrange(10), RG.randrange(-5, 6)
        got = (h, c)
        for t in tags:
            got = act(t, *got)
        assert act(merged, h, c) == got
        n = RG.randrange(1, 10)
        a = [RG.randrange(7) for _ in range(n)]
        current, history = [], []
        stack = []
        for r, x in enumerate(a):
            while stack and a[stack[-1]] >= x:
                stack.pop()
            p = stack[-1] if stack else -1
            current.append(0)
            history.append(0)
            current[p+1:r+1] = [x]*(r-p)
            stack.append(r)
            history = [h+c for h, c in zip(history, current)]
            for l in range(r+1):
                want = sum(min(a[i:j+1]) for i in range(l, r+1) for j in range(i, r+1))
                assert sum(history[l:]) == want


def check_clamped_walk():
    def merge(x, y):
        l, r, a, b = x
        L, R, A, B = y
        u, v = L-a, R-a
        if r < u:
            return r, r, L+A-r, b+B-(u-r)
        if v < l:
            return l, l, R+A-l, b+B-(l-v)
        return max(l, u), min(r, v), a+A, b+B

    def evaluate(f, x):
        l, r, a, b = f
        return min(max(x, l), r)+a, b-max(l-x, 0, x-r)

    for _ in range(1200):
        m = RG.randrange(1, 20)
        steps = [RG.randrange(-30, 31) for _ in range(RG.randrange(15))]
        leaves = []
        for s in steps:
            h = min(abs(s), m)
            leaves.append((0, m-h, h, h) if s >= 0 else (h, m, -h, h))
        f = fold(leaves, merge, (0, m, 0, 0))
        p = RG.randrange(len(leaves)+1)
        assert merge(fold(leaves[:p], merge, (0, m, 0, 0)),
                     fold(leaves[p:], merge, (0, m, 0, 0))) == f
        for start in range(m+1):
            x, gain = start, 0
            for s in steps:
                y = min(max(x+s, 0), m)
                gain, x = gain+abs(y-x), y
            assert evaluate(f, start) == (x, gain), (m, steps, f, start)


def check_signed_subsequence():
    # States: before, first(sign), normal(prev,now), penultimate(sign), done.
    signs = (-1, 1)
    for _ in range(180):
        a = [RG.randrange(-3, 4) for _ in range(RG.randrange(3, 9))]
        states = {('before',): 0}
        for x in a:
            nxt = dict(states)  # skip, no in-place cascading across one element
            def put(key, v):
                nxt[key] = max(nxt.get(key, NEG), v)
            for key, val in states.items():
                if key[0] == 'before':
                    for s in signs:
                        put(('first', s), val+s*x)
                elif key[0] == 'first':
                    s = key[1]
                    put(('last', s), val+2*s*x)
                    for t in signs:
                        put(('normal', s, t), val+(t+2*s)*x)
                elif key[0] == 'normal':
                    s, t = key[1:]
                    put(('last', t), val+(2*t+3*s)*x)
                    for u in signs:
                        put(('normal', t, u), val+(u+2*t+3*s)*x)
                elif key[0] == 'last':
                    put(('done',), val+3*key[1]*x)
            states = nxt
        want = NEG
        for k in range(3, len(a)+1):
            for pos in combinations(range(len(a)), k):
                b = [a[i] for i in pos]
                want = max(want, sum(abs(b[i]+2*b[i+1]+3*b[i+2]) for i in range(k-2)))
        assert states['done',] == want


def check_k_inversions():
    for _ in range(300):
        k = RG.randrange(1, 6)
        a = [RG.randrange(9) for _ in range(RG.randrange(1, 9))]
        for x in range(10):
            e, q = 0, 0
            for m, v in enumerate(a, 1):
                q += v < x <= v+k
                e += v-k <= x < v
                q = min(q, e)
                b = a[:m]
                original = sum(0 < b[i]-b[j] <= k for i, j in combinations(range(m), 2))
                want = 10**9
                for t in range(m+1):
                    c = b[:t]+[x]+b[t:]
                    want = min(want, sum(0 < c[i]-c[j] <= k
                                         for i, j in combinations(range(m+1), 2)))
                assert original+q == want


def check_tree_history():
    for _ in range(300):
        n = RG.randrange(1, 12)
        labels = list(range(n))
        RG.shuffle(labels)
        edges = [(labels[i], labels[RG.randrange(i)]) for i in range(1, n)]
        c, d = [], []
        for r in range(n):
            c = [v+1 for v in c]
            for u, v in edges:
                if max(u, v) == r:
                    for l in range(min(u, v)+1):
                        c[l] -= 1
            c.append(1)
            d.append(0)
            d = [v+int(x == 1) for v, x in zip(d, c)]
            for l in range(r+1):
                want = sum(1 for a in range(l, r+1) for b in range(a, r+1)
                           if b-a == sum(a <= u <= b and a <= v <= b for u, v in edges))
                assert sum(d[l:]) == want
    # Check the minimum-height tag compression on sequences with valid G calls.
    for _ in range(600):
        initial = [RG.randrange(1, 8) for _ in range(6)]
        values, history = initial[:], [0]*6
        tags = []
        for _ in range(20):
            if RG.randrange(2):
                delta = RG.randrange(-min(values)+1, 6)
                values = [x+delta for x in values]
                tags.append((delta, None, 0))
            else:
                history = [h+int(x == 1) for h, x in zip(history, values)]
                tags.append((0, 0, 1))
        s, h, b = 0, None, 0
        for ds, dh, db in tags:
            if dh is not None:
                nh = s+dh
                if h is None or nh < h:
                    h, b = nh, db
                elif nh == h:
                    b += db
            s += ds
        assert [x+s for x in initial] == values
        assert [b if h is not None and x+h == 1 else 0 for x in initial] == history


def check_coverage_weights():
    for _ in range(300):
        n = RG.randrange(1, 9)
        rects = []
        for _ in range(RG.randrange(12)):
            a, c = sorted([RG.randrange(n), RG.randrange(n)])
            b, d = sorted([RG.randrange(n), RG.randrange(n)])
            rects.append((a, b, c, d))
        cover, answer = [0]*n, 0
        for i in range(n):
            for a, b, c, d in rects:
                delta = int(i == a)-int(i == c+1)
                for j in range(b, d+1):
                    cover[j] += delta
            mn = min(cover)
            weights = sum(j+1 for j, x in enumerate(cover) if x == mn)
            answer += (i+1)*weights if mn == 0 else 0
        want = sum((i+1)*(j+1) for i in range(n) for j in range(n)
                   if not any(a <= i <= c and b <= j <= d for a, b, c, d in rects))
        assert answer == want


def check_laminar_endpoints():
    for _ in range(300):
        n = RG.randrange(1, 10)
        pts = list(range(1, 2*n+1))
        RG.shuffle(pts)
        pairs = [tuple(sorted(pts[i:i+2])) for i in range(0, 2*n, 2)]
        for left in range(n):
            values = [0]*(2*n+1)
            window = []
            for i in range(left, n):
                a, b = pairs[i]
                legal = not any(c < a < d < b or a < c < b < d for c, d in window)
                assert (values[a] == values[b-1]) == legal
                if not legal:
                    break
                # Independent bit weights make this test collision-free.
                for j in range(a, b):
                    values[j] += 1 << i
                window.append((a, b))


def check_second_rectangles():
    for _ in range(300):
        n = RG.randrange(4, 13)
        a = [RG.randrange(5) for _ in range(n)]
        score = [[0]*n for _ in range(n)]
        for i in range(n):
            for direction in (1, -1):
                preferred = [j for j in range(n)
                             if (a[j], j) > (a[i], i)] if direction == 1 else [
                                 j for j in range(n) if (a[j], j) < (a[i], i)]
                left = [-1, -1]+[j for j in preferred if j < i]
                right = [j for j in preferred if j > i]+[n, n]
                k1, k2 = left[-2:]
                k4, k5 = right[:2]
                for l in range(k1+1, k2+1):
                    for r in range(i, k4):
                        score[l][r] += direction*a[i]
                for l in range(k2+1, i+1):
                    for r in range(k4, k5):
                        score[l][r] += direction*a[i]
        f, brute = [0]+[NEG]*n, [0]+[NEG]*n
        for r in range(4, n+1):
            for l in range(r-3):
                ordered = sorted(a[l:r])
                assert score[l][r-1] == ordered[-2]-ordered[1]
                f[r] = max(f[r], f[l]+score[l][r-1])
                brute[r] = max(brute[r], brute[l]+ordered[-2]-ordered[1])
        assert f == brute


def check_integer_functions():
    # Short-function summaries; long-summary implementation is deliberately absent.
    def merge(left, right):
        m, r, d, eps = left
        n, s, e, eta = right
        z = (eta*(r-e)) % (1 << m)
        return (m+n, s+(1 << n)*z, d+eps*((e+eta*z-r)//(1 << m)), eps*eta)

    for _ in range(1000):
        a = [RG.randrange(11) for _ in range(RG.randrange(1, 13))]
        leaves = [(1, 3*x % 2, (3*x-(3*x % 2))//2, -1) for x in a]
        m, r, d, eps = fold(leaves, merge, (0, 0, 0, 1))
        for z in range(-3, 4):
            t = r+(1 << m)*z
            got = t
            for x in a[::-1]:
                assert (3*x-got) % 2 == 0
                got = (3*x-got)//2
            assert got == d+eps*z
        numerator = sum((-2)**(len(a)-1-i)*x for i, x in enumerate(a))
        denominator = sum((-2)**i for i in range(len(a)))
        possible = numerator % denominator == 0
        assert ((d-r) % ((1 << m)-eps) == 0) == possible
        if possible:
            target = numerator//denominator
            op = 0
            for x in a:
                op = target-x-2*op
            assert op == 0
            assert -10**9 <= target <= 2*10**9
    for a in (0, 10**9):
        for t in (-10**9, 2*10**9):
            assert -10**9 <= (3*a-t)/2 <= 2*10**9
    assert 3*10**9 < 2**32


def main():
    from classic_checks import run as check_classics
    checks = [check_inventory, check_small_alphabets, check_runs_and_ranks,
              check_power_tags, check_add_mod, check_two_segments,
              check_permutations_and_nand, check_history_tags, check_clamped_walk,
              check_signed_subsequence, check_k_inversions, check_tree_history,
              check_coverage_weights, check_laminar_endpoints, check_second_rectangles,
              check_integer_functions, check_classics]
    for check in checks:
        check()
        print('PASS', check.__name__, flush=True)
    print(f'PASS 24 AAL problem entries and 10 classic models; {len(checks)} check groups. '
          'Small models only, not 24 accepted solutions.')


if __name__ == '__main__':
    main()
