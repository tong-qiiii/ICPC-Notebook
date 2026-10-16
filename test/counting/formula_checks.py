"""Exact small-instance audits; these tests do not replace general proofs."""
from collections import Counter
from fractions import Fraction
from functools import lru_cache
from itertools import combinations, permutations, product
from math import comb, factorial, gcd, prod
from pathlib import Path
import random
import re

CHECKS = []

def check(fn):
    CHECKS.append(fn)
    return fn

def C(n, k):
    return comb(n, k) if n >= 0 and 0 <= k <= n else 0

@lru_cache(None)
def mu(n):
    ans, p = 1, 2
    while p * p <= n:
        if n % p == 0:
            n //= p
            if n % p == 0:
                return 0
            ans = -ans
        while n % p == 0:
            n //= p
        p += 1
    return -ans if n > 1 else ans

def phi(n):
    return sum(gcd(i, n) == 1 for i in range(1, n + 1))

@lru_cache(None)
def partitions(n, least=1):
    if n == 0:
        return ((),)
    return tuple((v,) + tail for v in range(least, n + 1)
                 for tail in partitions(n - v, v))

@lru_cache(None)
def set_partitions(n):
    if n == 0:
        return ((),)
    out = []
    for blocks in set_partitions(n - 1):
        out.append(blocks + ((n - 1,),))
        for i in range(len(blocks)):
            out.append(blocks[:i] + (blocks[i] + (n - 1,),) + blocks[i + 1:])
    return tuple(out)

def S(n, k):
    return sum(len(p) == k for p in set_partitions(n))

def determinant(a):
    a = [list(map(Fraction, row)) for row in a]
    ans = Fraction(1)
    for i in range(len(a)):
        pivot = next((j for j in range(i, len(a)) if a[j][i]), None)
        if pivot is None:
            return 0
        if pivot != i:
            a[pivot], a[i] = a[i], a[pivot]
            ans = -ans
        v = a[i][i]
        ans *= v
        for j in range(i + 1, len(a)):
            ratio = a[j][i] / v
            for k in range(i, len(a)):
                a[j][k] -= ratio * a[i][k]
    return ans

@check
def binomial_identities():
    for n in range(12):
        assert sum(C(n, k) for k in range(n + 1)) == 2 ** n
        assert sum((-1) ** k * C(n, k) for k in range(n + 1)) == (n == 0)
        assert sum(C(n, k) ** 2 for k in range(n + 1)) == C(2 * n, n)
        for r in range(n + 1):
            falling = lambda k: prod(range(k - r + 1, k + 1)) if k >= r else 0
            assert sum(falling(k) * C(n, k) for k in range(n + 1)) == falling(n) * 2 ** (n - r)
    for a in range(7):
        for b in range(7):
            for r in range(a + b + 2):
                assert sum(C(a, k) * C(b, r - k) for k in range(a + 1)) == C(a + b, r)
                if b >= a:
                    assert sum((-1) ** k * C(a, k) * C(b - k, r) for k in range(a + 1)) == C(b - a, r - a)
    for n in range(12):
        for k in range(n + 1):
            assert sum(C(i, k) for i in range(k, n + 1)) == C(n + 1, k + 1)

@check
def differences_and_powers():
    for degree in range(8):
        delta = [sum((-1) ** (r - j) * C(r, j) * j ** degree for j in range(r + 1))
                 for r in range(degree + 2)]
        assert delta[-1] == 0
        for n in range(13):
            assert sum(C(n, r) * delta[r] for r in range(degree + 1)) == n ** degree
        for n in range(12):
            assert sum(i ** degree for i in range(n)) == sum(
                S(degree, k) * factorial(k) * C(n, k + 1) for k in range(degree + 1))

@check
def boxes_and_stars():
    for n in range(7):
        for m in range(1, 5):
            functions = list(product(range(m), repeat=n))
            assert len(functions) == m ** n
            assert sum(len(set(f)) == m for f in functions) == factorial(m) * S(n, m)
            assert sum(len(set(f)) == n for f in functions) == C(m, n) * factorial(n)
            occupancy = {tuple(f.count(i) for i in range(m)) for f in functions}
            assert len(occupancy) == C(n + m - 1, m - 1)
            if n:
                assert sum(all(v > 0 for v in a) for a in occupancy) == C(n - 1, m - 1)
            assert len({tuple(sorted(a)) for a in occupancy}) == sum(len(p) <= m for p in partitions(n))
            assert sum(len(p) <= m for p in set_partitions(n)) == sum(S(n, k) for k in range(m + 1))
    for n in range(1, 10):
        for k in range(n + 1):
            for d in range(1, 4):
                actual = sum(all(a[i + 1] - a[i] >= d for i in range(k - 1)) for a in combinations(range(1, n + 1), k))
                assert actual == (1 if not k else C(n - (k - 1) * (d - 1), k))
    for bounds in product(range(4), repeat=3):
        for total in range(10):
            actual = sum(sum(a) == total for a in product(*(range(u + 1) for u in bounds)))
            expected = sum((-1) ** len(s) * C(total - sum(bounds[i] + 1 for i in s) + 2, 2)
                           for k in range(4) for s in combinations(range(3), k))
            assert actual == expected

@check
def exact_conditions():
    # Universe consists of every possible event-membership bitmask.
    for m in range(7):
        exact = [C(m, k) for k in range(m + 1)]
        inter = [sum(C(j, i) * exact[j] for j in range(i, m + 1)) for i in range(m + 1)]
        for k in range(m + 1):
            assert exact[k] == sum((-1) ** (i - k) * C(i, k) * inter[i] for i in range(k, m + 1))
            if k:
                assert sum(exact[k:]) == sum((-1) ** (i - k) * C(i - 1, k - 1) * inter[i] for i in range(k, m + 1))
    for n in range(7):
        for m in range(5):
            assert factorial(m) * S(n, m) == sum((-1) ** j * C(m, j) * (m - j) ** n for j in range(m + 1))

@check
def permutation_models():
    for n in range(8):
        perms = list(permutations(range(n)))
        fixed = Counter(sum(v == i for i, v in enumerate(p)) for p in perms)
        descent = Counter(sum(p[i] > p[i + 1] for i in range(n - 1)) for p in perms)
        cycle_types = Counter()
        for p in perms:
            vis = set(); lengths = []
            for i in range(n):
                if i in vis:
                    continue
                u, length = i, 0
                while u not in vis:
                    vis.add(u); length += 1; u = p[u]
                lengths.append(length)
            cycle_types[tuple(sorted(lengths))] += 1
        for k in range(n + 1):
            der = sum((-1) ** j * factorial(n - k) // factorial(j) for j in range(n - k + 1))
            # Use exact fractions; the termwise factorial quotients happen to be integral.
            assert fixed[k] == C(n, k) * der
        for lengths, count in cycle_types.items():
            mult = Counter(lengths)
            assert count == factorial(n) // prod(j ** c * factorial(c) for j, c in mult.items())
        if n:
            for k in range(n):
                explicit = sum((-1) ** j * C(n + 1, j) * (k + 1 - j) ** n for j in range(k + 2))
                assert descent[k] == explicit
            for x in range(7):
                assert x ** n == sum(descent[k] * C(x + k, n) for k in range(n))
    for n in range(1, 7):
        parts = set_partitions(n)
        for k in range(1, n + 1):
            lah = sum(prod(factorial(len(b)) for b in p) for p in parts if len(p) == k)
            assert lah == C(n - 1, k - 1) * factorial(n) // factorial(k)
    assert len(set_partitions(3)) == 5
    assert sum(factorial(len(p)) for p in set_partitions(3)) == 13

@check
def ballot_and_fuss():
    for a in range(6):
        for b in range(6):
            for h in range(3):
                if h + a - b < 0:
                    continue
                valid = strict = 0
                for ups in combinations(range(a + b), a):
                    chosen = set(ups); bal = h; good = True; positive = True
                    for i in range(a + b):
                        bal += 1 if i in chosen else -1
                        good &= bal >= 0; positive &= bal > 0
                    valid += good; strict += positive
                assert valid == C(a + b, b) - C(a + b, b - h - 1)
                if h == 0 and a > b:
                    assert strict == Fraction(a - b, a + b) * C(a + b, b)
    for r in range(2, 5):
        for n in range(4):
            actual = 0
            for ups in combinations(range(r * n), n):
                chosen = set(ups); bal = 0; good = True
                for i in range(r * n):
                    bal += r - 1 if i in chosen else -1
                    good &= bal >= 0
                actual += good
            assert actual == C(r * n, n) // ((r - 1) * n + 1)

@check
def partition_models():
    for n in range(16):
        parts = partitions(n)
        assert sum(len(set(p)) == len(p) for p in parts) == sum(all(v % 2 for v in p) for p in parts)
        for k in range(n + 1):
            exact = sum(len(p) == k for p in parts)
            if n and k:
                assert exact == sum(len(p) == k - 1 for p in partitions(n - 1)) + (
                    sum(len(p) == k for p in partitions(n - k)) if n >= k else 0)
            if n >= k:
                assert exact == sum(len(p) <= k for p in partitions(n - k))
        assert sum(len(p) <= 3 for p in parts) == sum(not p or max(p) <= 3 for p in parts)

@check
def subset_inversion():
    for bits in range(7):
        n = 1 << bits
        f = [i * i - 3 for i in range(n)]
        z = [sum(f[t] for t in range(n) if t & s == t) for s in range(n)]
        inv = [sum((-1) ** ((s ^ t).bit_count()) * z[t] for t in range(n) if t & s == t) for s in range(n)]
        assert inv == f

@check
def mobius_and_euler_phi():
    for n in range(1, 101):
        divs = [d for d in range(1, n + 1) if n % d == 0]
        assert sum(mu(d) for d in divs) == (n == 1)
        assert sum(phi(d) for d in divs) == n
        h = lambda v: sum(mu(d) * d for d in range(1, v + 1) if v % d == 0)
        assert sum(phi(d) * h(n // d) for d in divs) == (n == 1)
        f = lambda v: v * v + 7
        conv = lambda v: sum(phi(d) * f(v // d) for d in range(1, v + 1) if v % d == 0)
        assert sum(h(d) * conv(n // d) for d in divs) == f(n)
        F = lambda v: sum(f(v // i) for i in range(1, v + 1))
        assert sum(mu(i) * F(n // i) for i in range(1, n + 1)) == f(n)

@check
def gcd_lcm_models():
    for n in range(1, 31):
        assert sum(gcd(i, n) for i in range(1, n + 1)) == sum(d * phi(n // d) for d in range(1, n + 1) if n % d == 0)
        assert sum(i * n // gcd(i, n) for i in range(1, n + 1)) == n * (1 + sum(d * phi(d) for d in range(1, n + 1) if n % d == 0)) // 2
        for m in range(1, 16):
            for power in range(3):
                weight = lambda d: sum(mu(d // k) * k ** power for k in range(1, d + 1) if d % k == 0)
                expected = sum(weight(d) * (n // d) * (m // d) for d in range(1, min(n, m) + 1))
                assert sum(gcd(i, j) ** power for i in range(1, n + 1) for j in range(1, m + 1)) == expected
    values = [2, 2, 3, 6]
    exact = Counter()
    for s in range(1, 1 << len(values)):
        v = 0
        for i, a in enumerate(values):
            if s >> i & 1:
                v = gcd(v, a)
        exact[v] += 1
    for d in range(1, 7):
        expected = sum(mu(k) * (2 ** sum(a % (d * k) == 0 for a in values) - 1) for k in range(1, 6 // d + 1))
        assert exact[d] == expected

@check
def burnside_and_periods():
    for n in range(1, 8):
        for colors in range(1, 4):
            rotations, bracelets, primitive = set(), set(), set()
            words = list(product(range(colors), repeat=n))
            for w in words:
                orbit = [w[r:] + w[:r] for r in range(n)]
                rotations.add(min(orbit))
                bracelets.add(min(orbit + [v[::-1] for v in orbit]))
                if len(set(orbit)) == n:
                    primitive.add(min(orbit))
            total = sum(phi(d) * colors ** (n // d) for d in range(1, n + 1) if n % d == 0)
            assert len(rotations) == total // n
            reflection = n * colors ** ((n + 1) // 2) if n % 2 else n // 2 * (colors ** (n // 2 + 1) + colors ** (n // 2))
            assert len(bracelets) == (total + reflection) // (2 * n)
            assert len(primitive) == sum(mu(d) * colors ** (n // d) for d in range(1, n + 1) if n % d == 0) // n

@check
def rooks_permanent_chromatic():
    rng = random.Random(1205)
    for n in range(1, 5):
        for _ in range(15):
            board = {(i, j) for i in range(n) for j in range(n) if rng.randrange(3) == 0}
            rook = [0] * (n + 1)
            for k in range(n + 1):
                for cells in combinations(board, k):
                    rook[k] += len({i for i, j in cells}) == len({j for i, j in cells}) == k
            allowed = [[int((i, j) not in board) for j in range(n)] for i in range(n)]
            actual = sum(all(allowed[i][p[i]] for i in range(n)) for p in permutations(range(n)))
            assert actual == sum((-1) ** k * rook[k] * factorial(n - k) for k in range(n + 1))
            ryser = sum((-1) ** (n - s.bit_count()) * prod(sum(allowed[i][j] for j in range(n) if s >> j & 1) for i in range(n)) for s in range(1 << n))
            assert actual == ryser
    for n in range(1, 5):
        edges = list(combinations(range(n), 2))
        for mask in range(1 << len(edges)):
            use = [edge for k, edge in enumerate(edges) if mask >> k & 1]
            for q in range(1, 4):
                actual = sum(all(w[u] != w[v] for u, v in use) for w in product(range(q), repeat=n))
                expected = 0
                for s in range(1 << len(use)):
                    parent = list(range(n))
                    def find(u):
                        while parent[u] != u:
                            u = parent[u]
                        return u
                    for k, (u, v) in enumerate(use):
                        if s >> k & 1:
                            parent[find(u)] = find(v)
                    expected += (-1) ** s.bit_count() * q ** len({find(u) for u in range(n)})
                assert actual == expected

@check
def trees_and_matrix_tree():
    for n in range(1, 6):
        edges = list(combinations(range(n), 2))
        degree_count = Counter()
        trees = []
        for chosen in combinations(edges, n - 1):
            seen = {0}
            for _ in range(n):
                for u, v in chosen:
                    if u in seen or v in seen:
                        seen.update((u, v))
            if len(seen) != n:
                continue
            trees.append(chosen)
            degree_count[tuple(sum(u == i or v == i for u, v in chosen) for i in range(n))] += 1
        assert len(trees) == (1 if n == 1 else n ** (n - 2))
        if n >= 2:
            for degrees, count in degree_count.items():
                assert count == factorial(n - 2) // prod(factorial(d - 1) for d in degrees)
        rng = random.Random(n)
        for _ in range(20):
            weights = {e: rng.randrange(4) for e in edges}
            lap = [[0] * n for _ in range(n)]
            for (u, v), w in weights.items():
                lap[u][u] += w; lap[v][v] += w
                lap[u][v] -= w; lap[v][u] -= w
            expected = sum(prod(weights[e] for e in tree) for tree in trees)
            assert determinant([row[:-1] for row in lap[:-1]]) == expected

@check
def connected_graphs():
    connected = [0] * 6
    for n in range(1, 6):
        edges = list(combinations(range(n), 2))
        for mask in range(1 << len(edges)):
            seen = {0}
            for _ in range(n):
                for i, (u, v) in enumerate(edges):
                    if mask >> i & 1 and (u in seen or v in seen):
                        seen.update((u, v))
            connected[n] += len(seen) == n
        assert connected[n] == 2 ** C(n, 2) - sum(C(n - 1, k - 1) * connected[k] * 2 ** C(n - k, 2) for k in range(1, n))
    assert connected[1:] == [1, 1, 4, 38, 728]

@check
def lgv_signed_paths():
    rng = random.Random(1206)
    for _ in range(50):
        g = [[v for v in range(u + 1, 6) if rng.randrange(2)] for u in range(6)]
        def paths(u, t):
            if u == t:
                return [(u,)]
            return [(u,) + tail for v in g[u] for tail in paths(v, t)]
        entries = [[paths(s, t) for t in [4, 5]] for s in [0, 1]]
        mat = [[len(p) for p in row] for row in entries]
        signed = 0
        for perm in [(0, 1), (1, 0)]:
            count = sum(not(set(a) & set(b)) for a in entries[0][perm[0]] for b in entries[1][perm[1]])
            signed += count if perm == (0, 1) else -count
        assert determinant(mat) == signed

@check
def finite_fields():
    def rank(rows, n):
        rows = list(rows); pos = 0
        for bit in range(n):
            pivot = next((i for i in range(pos, len(rows)) if rows[i] >> bit & 1), None)
            if pivot is None:
                continue
            rows[pos], rows[pivot] = rows[pivot], rows[pos]
            for i in range(pos + 1, len(rows)):
                if rows[i] >> bit & 1:
                    rows[i] ^= rows[pos]
            pos += 1
        return pos
    def gaussian(n, k, q):
        return prod((Fraction(q ** n - q ** i, q ** k - q ** i) for i in range(k)), start=Fraction(1))
    for m in range(1, 4):
        for n in range(1, 4):
            counts = Counter(rank(rows, n) for rows in product(range(1 << n), repeat=m))
            for r in range(min(m, n) + 1):
                assert counts[r] == gaussian(m, r, 2) * gaussian(n, r, 2) * prod(2 ** r - 2 ** i for i in range(r))

@check
def hook_lengths():
    @lru_cache(None)
    def corners(shape):
        if not shape:
            return 1
        ans = 0
        for i, length in enumerate(shape):
            if i + 1 == len(shape) or length > shape[i + 1]:
                nxt = list(shape); nxt[i] -= 1
                ans += corners(tuple(v for v in nxt if v))
        return ans
    for n in range(10):
        for part in partitions(n):
            shape = part[::-1]
            hooks = [length - j + sum(v > j for v in shape[i + 1:])
                     for i, length in enumerate(shape) for j in range(length)]
            assert corners(shape) == factorial(n) // prod(hooks)


def main():
    for fn in CHECKS:
        fn()
        print('PASS formula group:', fn.__name__, flush=True)
    # No source or document edits; explicitly report the finite scope.
    source = (Path(__file__).resolve().parents[2] / 'sections/12_counting.typ').read_text(encoding='utf8')
    labels = re.findall(r'^== .* <(algo-counting-[a-z0-9-]+)>$', source, re.M)
    assert labels and len(labels) == len(set(labels))
    print(f'PASS {len(CHECKS)} exact small-instance formula groups; {len(labels)} topics; general applicability remains a proof obligation', flush=True)

if __name__ == '__main__':
    main()
