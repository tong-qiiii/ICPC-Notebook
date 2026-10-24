"""Independent small reductions checked against exhaustive choices."""
from collections import deque
from heapq import heappop, heappush
from itertools import product
import random
from helpers import maxflow


def run():
    rg = random.Random(160008)
    for _ in range(250):
        n = rg.randrange(1, 7)
        costs = [(rg.randrange(-3, 8), rg.randrange(-3, 8)) for _ in range(n)]
        pairs = [(i, j, rg.randrange(6)) for i in range(n) for j in range(i+1, n)
                 if rg.randrange(3) == 0]
        constant = sum(min(x) for x in costs)
        edges = [(n, i, b-min(a, b)) for i, (a, b) in enumerate(costs)]
        edges += [(i, n+1, a-min(a, b)) for i, (a, b) in enumerate(costs)]
        edges += [(u, v, w) for i, j, w in pairs for u, v in ((i, j), (j, i))]
        want = min(sum(costs[i][b[i]] for i in range(n))
                   + sum(w for i, j, w in pairs if b[i] != b[j])
                   for b in product(range(2), repeat=n))
        assert constant+maxflow(n+2, edges, n, n+1) == want
        weights = [rg.randrange(-5, 8) for _ in range(n)]
        deps = [(i, j) for i in range(n) for j in range(n) if i != j and rg.randrange(8) == 0]
        pos = sum(max(w, 0) for w in weights)
        edges = [(n, i, w) if w >= 0 else (i, n+1, -w) for i, w in enumerate(weights)]
        edges += [(i, j, pos+1) for i, j in deps]
        want = max(sum(w*x for w, x in zip(weights, b)) for b in product(range(2), repeat=n)
                   if all(not b[i] or b[j] for i, j in deps))
        assert pos-maxflow(n+2, edges, n, n+1) == want
    for _ in range(120):
        n = rg.randrange(2, 5)
        edges = [(rg.randrange(n), rg.randrange(n), rg.randrange(4)) for _ in range(8)]
        def shortest(g, start):
            d, heap = [10**9]*len(g), [(0, start)]
            d[start] = 0
            while heap:
                val, u = heappop(heap)
                if val != d[u]:
                    continue
                for v, w in g[u]:
                    if val+w < d[v]:
                        d[v] = val+w
                        heappush(heap, (d[v], v))
            return d
        layer, forward, back = [[] for _ in range(2*n)], [[] for _ in range(n)], [[] for _ in range(n)]
        for u, v, w in edges:
            forward[u].append((v, w))
            back[v].append((u, w))
            layer[u].append((v, w))
            layer[u+n].append((v+n, w))
            layer[u].append((v+n, w//2))
        ds, dt = shortest(forward, 0), shortest(back, n-1)
        candidates = [ds[u]+w//2+dt[v] for u, v, w in edges if ds[u] < 10**9 and dt[v] < 10**9]
        assert shortest(layer, 0)[2*n-1] == min(candidates, default=10**9)
        # SCC coin model versus walking on (vertex, collected mask).
        reach = [[i == j for j in range(n)] for i in range(n)]
        for u, v, _ in edges:
            reach[u][v] = True
        for k in range(n):
            for i in range(n):
                for j in range(n):
                    reach[i][j] |= reach[i][k] and reach[k][j]
        bel, groups = [-1]*n, []
        for u in range(n):
            if bel[u] == -1:
                group = [v for v in range(n) if reach[u][v] and reach[v][u]]
                for v in group:
                    bel[v] = len(groups)
                groups.append(group)
        w = [rg.randrange(7) for _ in range(n)]
        totals = [sum(w[v] for v in group) for group in groups]
        dag = [set() for _ in groups]
        for u, v, _ in edges:
            if bel[u] != bel[v]:
                dag[bel[u]].add(bel[v])
        def best(c):
            return totals[c]+max((best(d) for d in dag[c]), default=0)
        states = {(i, 1 << i) for i in range(n)}
        queue = deque(states)
        while queue:
            u, mask = queue.popleft()
            for v, _ in forward[u]:
                key = v, mask | (1 << v)
                if key not in states:
                    states.add(key)
                    queue.append(key)
        assert max(best(c) for c in range(len(groups))) == max(
            sum(w[i] for i in range(n) if mask >> i & 1) for _, mask in states)
    for _ in range(150):
        n, m = 3, rg.randrange(1, 6)
        original = []
        for _ in range(m):
            u, v = rg.sample(range(n), 2)
            lo = rg.randrange(2)
            hi = lo+rg.randrange(3)
            original.append((u, v, lo, hi))
        balance = [0]*n
        residual = []
        for u, v, lo, hi in original:
            balance[u] -= lo
            balance[v] += lo
            residual.append((u, v, hi-lo))
        for u, b in enumerate(balance):
            residual.append((n, u, b) if b > 0 else (u, n+1, -b))
        feasible = maxflow(n+2, residual, n, n+1) == sum(max(b, 0) for b in balance)
        brute = False
        for values in product(*(range(lo, hi+1) for u, v, lo, hi in original)):
            net = [0]*n
            for (u, v, _, _), value in zip(original, values):
                net[u] -= value
                net[v] += value
            brute |= all(x == 0 for x in net)
        assert feasible == brute
    print('PASS graph models: layered paths, SCC collection, binary cut, closure, lower-bound balances', flush=True)
