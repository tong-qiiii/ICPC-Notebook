"""Eight offline models, with small arrays/graphs as independent oracles."""
from bisect import bisect_left, bisect_right
from collections import Counter, defaultdict
import random
from helpers import BIT


def run():
    rg = random.Random(180008)
    for _ in range(180):
        n = rg.randrange(1, 18)
        a = [rg.randrange(-5, 7) for _ in range(n)]
        queries = []
        for _ in range(12):
            l, r = sorted([rg.randrange(n), rg.randrange(n)])
            queries.append((l, r, rg.randrange(-7, 9)))
        values = sorted(set(a))
        events = sorted((p, x, sign, i) for i, (l, r, x) in enumerate(queries)
                        for p, sign in ((r+1, 1), (l, -1)))
        bit, used, answer = BIT(len(values)), 0, [0]*len(queries)
        for p, x, sign, i in events:
            while used < p:
                bit.add(bisect_left(values, a[used])+1, 1)
                used += 1
            answer[i] += sign*bit.sum(bisect_right(values, x))
        assert answer == [sum(v <= x for v in a[l:r+1]) for l, r, x in queries]
        # Last-occurrence sweep, an independent invariant from the value sweep.
        bit, last, used = BIT(n), {}, 0
        for l, r, _ in sorted(queries, key=lambda q: q[1]):
            while used <= r:
                value = a[used]
                if value in last:
                    bit.add(last[value]+1, -1)
                bit.add(used+1, 1)
                last[value], used = used, used+1
            assert bit.sum(r+1)-bit.sum(l) == len(set(a[l:r+1]))
        # Whole-answer binary split; no future data changes exist in this static model.
        tasks = [(l, r, rg.randrange(1, r-l+2), i) for i, (l, r, _) in enumerate(queries)]
        kth = [None]*len(tasks)
        def divide(lo, hi, points, requests):
            if lo == hi:
                for l, r, k, i in requests:
                    kth[i] = values[lo]
                return
            mid = (lo+hi)//2
            small = [(p, v) for p, v in points if v <= values[mid]]
            large = [(p, v) for p, v in points if v > values[mid]]
            bit, left, right = BIT(n), [], []
            for p, _ in small:
                bit.add(p+1, 1)
            for l, r, k, i in requests:
                c = bit.sum(r+1)-bit.sum(l)
                (left if k <= c else right).append((l, r, k if k <= c else k-c, i))
            divide(lo, mid, small, left)
            divide(mid+1, hi, large, right)
        divide(0, len(values)-1, list(enumerate(a)), tasks)
        assert kth == [sorted(a[l:r+1])[k-1] for l, r, k, _ in tasks]
        # 2D weighted rectangle sweep, including negative coordinates and point weights.
        pts = [(rg.randrange(-3, 4), rg.randrange(-3, 4), rg.randrange(-3, 6)) for _ in range(n)]
        rectangles = [tuple(sorted([rg.randrange(-4, 5), rg.randrange(-4, 5)]))
                      + tuple(sorted([rg.randrange(-4, 5), rg.randrange(-4, 5)])) for _ in range(8)]
        events = []
        for i, (l, r, b, t) in enumerate(rectangles):
            events += [(r, t, 1, i), (l-1, t, -1, i), (r, b-1, -1, i), (l-1, b-1, 1, i)]
        ys, pts, cursor = sorted({y for x, y, w in pts}), sorted(pts), 0
        bit, answer = BIT(len(ys)), [0]*8
        for x, y, sign, i in sorted(events):
            while cursor < len(pts) and pts[cursor][0] <= x:
                _, py, w = pts[cursor]
                bit.add(bisect_left(ys, py)+1, w)
                cursor += 1
            answer[i] += sign*bit.sum(bisect_right(ys, y))
        assert answer == [sum(w for x, y, w in pts if l <= x <= r and b <= y <= t)
                          for l, r, b, t in rectangles]
        # Count-CDQ with exact duplicates grouped before recursive splits.
        pts = [tuple(rg.randrange(4) for _ in range(3)) for _ in range(n)]
        groups = sorted(Counter(pts).items())
        partial = [0]*len(groups)
        bit = BIT(4)
        def cdq(ids):
            if len(ids) <= 1:
                return ids
            mid = len(ids)//2
            L, R = cdq(ids[:mid]), cdq(ids[mid:])
            cursor = 0
            for j in R:
                while cursor < len(L) and groups[L[cursor]][0][1] <= groups[j][0][1]:
                    point, w = groups[L[cursor]]
                    bit.add(point[2]+1, w)
                    cursor += 1
                partial[j] += bit.sum(groups[j][0][2]+1)
            for i in L[:cursor]:
                bit.add(groups[i][0][2]+1, -groups[i][1])
            return sorted(L+R, key=lambda j: groups[j][0][1])
        cdq(list(range(len(groups))))
        for i, (p, w) in enumerate(groups):
            want = sum(all(x <= y for x, y in zip(q, p)) for q in pts)-1
            assert partial[i]+w-1 == want
    check_times(rg)
    check_connectivity(rg)
    check_matrix(rg)
    print('PASS eight offline models: prefix/rectangle/distinct sweeps, count-CDQ, value/time binary search, rollback intervals, assignment baselines', flush=True)


def check_times(rg):
    for _ in range(150):
        m, k, groups = rg.randrange(1, 9), rg.randrange(1, 9), rg.randrange(1, 6)
        owner = [rg.randrange(groups) for _ in range(m)]
        target = [rg.randrange(20) for _ in range(groups)]
        events = [(rg.randrange(m), rg.randrange(m), rg.randrange(6)) for _ in range(k)]
        def affected(l, r, p):
            return l <= p <= r if l <= r else p >= l or p <= r
        brute, vals = [0 if x == 0 else k+1 for x in target], [0]*m
        for t, (l, r, v) in enumerate(events, 1):
            vals = [x+v*affected(l, r, p) for p, x in enumerate(vals)]
            for g in range(groups):
                if brute[g] == k+1 and sum(x for p, x in enumerate(vals) if owner[p] == g) >= target[g]:
                    brute[g] = t
        lo, hi = [0 if x == 0 else 1 for x in target], [0 if x == 0 else k+1 for x in target]
        while any(a < b for a, b in zip(lo, hi)):
            buckets = defaultdict(list)
            for g in range(groups):
                if lo[g] < hi[g]:
                    buckets[(lo[g]+hi[g])//2].append(g)
            vals = [0]*m
            for t, (l, r, v) in enumerate(events, 1):
                vals = [x+v*affected(l, r, p) for p, x in enumerate(vals)]
                for g in buckets[t]:
                    if sum(x for p, x in enumerate(vals) if owner[p] == g) >= target[g]:
                        hi[g] = t
                    else:
                        lo[g] = t+1
        assert lo == brute


def check_connectivity(rg):
    for _ in range(120):
        n, length = 5, 24
        starts, active, intervals, expected = defaultdict(list), Counter(), [], []
        for t in range(length):
            edge = tuple(sorted(rg.sample(range(n), 2)))
            if active[edge] and rg.randrange(2):
                active[edge] -= 1
                intervals.append((starts[edge].pop(), t, edge))
            else:
                active[edge] += 1
                starts[edge].append(t)
            u, v = rg.randrange(n), rg.randrange(n)
            visited, todo = {u}, [u]
            while todo:
                x = todo.pop()
                for (a, b), count in active.items():
                    if count and (a == x or b == x):
                        y = b if a == x else a
                        if y not in visited:
                            visited.add(y)
                            todo.append(y)
            expected.append((u, v, v in visited))
        intervals += [(s, length, e) for e, times in starts.items() for s in times]
        seg = [[] for _ in range(length*4)]
        def add(p, l, r, a, b, edge):
            if b <= l or r <= a:
                return
            if a <= l and r <= b:
                seg[p].append(edge)
                return
            mid = (l+r)//2
            add(p*2, l, mid, a, b, edge)
            add(p*2+1, mid, r, a, b, edge)
        for l, r, e in intervals:
            add(1, 0, length, l, r, e)
        parent, size, changes = list(range(n)), [1]*n, []
        def find(u):
            while parent[u] != u:
                u = parent[u]
            return u
        def visit(p, l, r):
            snap = len(changes)
            for a, b in seg[p]:
                a, b = find(a), find(b)
                if a != b:
                    if size[a] < size[b]:
                        a, b = b, a
                    changes.append((a, b, size[a]))
                    parent[b], size[a] = a, size[a]+size[b]
            if r-l == 1:
                a, b, want = expected[l]
                assert (find(a) == find(b)) == want
            else:
                mid = (l+r)//2
                visit(p*2, l, mid)
                visit(p*2+1, mid, r)
            while len(changes) > snap:
                a, b, old = changes.pop()
                parent[b], size[a] = b, old
        visit(1, 0, length)


def check_matrix(rg):
    for _ in range(180):
        n, m, count = 4, 5, 28
        operations, matrix, expected = [], [[0]*m for _ in range(n)], []
        for _ in range(count):
            kind = rg.randrange(3)
            if kind == 0:
                l, r = sorted([rg.randrange(m), rg.randrange(m)])
                v = rg.randrange(-5, 6)
                operations.append((0, l, r, v))
                for row in matrix:
                    row[l:r+1] = [x+v for x in row[l:r+1]]
            elif kind == 1:
                i, v = rg.randrange(n), rg.randrange(-5, 6)
                operations.append((1, i, v))
                matrix[i] = [v]*m
            else:
                i, j = rg.randrange(n), rg.randrange(m)
                operations.append((2, i, j))
                expected.append(matrix[i][j])
        latest, base, attach, answer, query_id = [0]*n, [0]*n, defaultdict(list), [], {}
        for t, op in enumerate(operations, 1):
            if op[0] == 1:
                _, i, v = op
                latest[i], base[i] = t, v
            elif op[0] == 2:
                _, i, j = op
                idx = len(answer)
                answer.append(base[i])
                query_id[t] = idx
                attach[latest[i]].append((idx, j))
        bit = BIT(m)
        for t, op in enumerate(operations, 1):
            if op[0] == 0:
                _, l, r, v = op
                bit.add(l+1, v)
                if r+1 < m:
                    bit.add(r+2, -v)
            for idx, j in attach[t]:
                answer[idx] -= bit.sum(j+1)
            if op[0] == 2:
                answer[query_id[t]] += bit.sum(op[2]+1)
        assert answer == expected
