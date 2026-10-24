"""Difference, reroot, ordered HLD, virtual-tree and bottleneck models."""
from collections import Counter, deque
from itertools import combinations
import random


def run():
    rg = random.Random(150008)
    for _ in range(240):
        n = rg.randrange(1, 15)
        parent = [0]+[rg.randrange(i) for i in range(1, n)]
        g = [[] for _ in range(n)]
        weight = [0]+[rg.randrange(6) for _ in range(1, n)]
        for v in range(1, n):
            g[parent[v]].append(v)
            g[v].append(parent[v])
        order, tin, size, depth, dist, son = [], [0]*n, [0]*n, [0]*n, [0]*n, [-1]*n

        def prep(u):
            size[u] = 1
            for v in g[u]:
                if v == parent[u] and u != 0:
                    continue
                depth[v], dist[v] = depth[u]+1, dist[u]+weight[v]
                prep(v)
                size[u] += size[v]
                if son[u] == -1 or size[v] > size[son[u]]:
                    son[u] = v
        prep(0)
        top = [0]*n

        def visit(u, t):
            top[u], tin[u] = t, len(order)
            order.append(u)
            if son[u] != -1:
                visit(son[u], t)
            for v in g[u]:
                if v != son[u] and (u == 0 or v != parent[u]):
                    visit(v, v)
        visit(0, 0)

        def lca(u, v):
            while depth[u] > depth[v]:
                u = parent[u]
            while depth[v] > depth[u]:
                v = parent[v]
            while u != v:
                u, v = parent[u], parent[v]
            return u

        def path(u, v):
            queue, prev = deque([u]), {u: -1}
            while v not in prev:
                x = queue.popleft()
                for y in g[x]:
                    if y not in prev:
                        prev[y] = x
                        queue.append(y)
            out, x = [], v
            while x != -1:
                out.append(x)
                x = prev[x]
            return out[::-1]

        for u in range(n):
            subtree = {v for v in range(n) if lca(u, v) == u}
            assert subtree == set(order[tin[u]:tin[u]+size[u]])
        point, edge, bp, be = [0]*n, [0]*n, [0]*n, [0]*n
        for _ in range(20):
            u, v, w = rg.randrange(n), rg.randrange(n), rg.randrange(-3, 4)
            t = lca(u, v)
            point[u] += w
            point[v] += w
            point[t] -= w
            if t:
                point[parent[t]] -= w
            edge[u] += w
            edge[v] += w
            edge[t] -= 2*w
            p = path(u, v)
            for x in p:
                bp[x] += w
            for x, y in zip(p, p[1:]):
                be[x if depth[x] > depth[y] else y] += w
            # Ordered path decomposition tested with node sequences, not commutative sums.
            left, right, a, b = [], [], u, v
            while top[a] != top[b]:
                if depth[top[a]] >= depth[top[b]]:
                    left += order[tin[top[a]]:tin[a]+1][::-1]
                    a = parent[top[a]]
                else:
                    right = order[tin[top[b]]:tin[b]+1]+right
                    b = parent[top[b]]
            middle = (order[tin[b]:tin[a]+1][::-1] if depth[a] > depth[b]
                      else order[tin[a]:tin[b]+1])
            assert left+middle+right == p
        for u in order[::-1]:
            if u:
                point[parent[u]] += point[u]
                edge[parent[u]] += edge[u]
        assert point == bp and edge[1:] == be[1:]
        brute = [sum(dist[u]+dist[v]-2*dist[lca(u, v)] for v in range(n)) for u in range(n)]
        answer = [0]*n
        answer[0] = brute[0]
        for u in order[1:]:
            answer[u] = answer[parent[u]]+(n-2*size[u])*weight[u]
        assert answer == brute
        keys = set(rg.sample(range(n), rg.randrange(n+1)))
        nodes = set(keys)
        initial = sorted(keys, key=tin.__getitem__)
        nodes.update(lca(u, v) for u, v in zip(initial, initial[1:]))
        virtual = sorted(nodes, key=tin.__getitem__)
        stk, vp = [], {}
        for u in virtual:
            while stk and lca(stk[-1], u) != stk[-1]:
                stk.pop()
            if stk:
                vp[u] = stk[-1]
            stk.append(u)
        counts, got = Counter(keys), 0
        for u in virtual[::-1]:
            if u in vp:
                p = vp[u]
                got += (dist[u]-dist[p])*counts[u]*(len(keys)-counts[u])
                counts[p] += counts[u]
        want = sum(dist[u]+dist[v]-2*dist[lca(u, v)] for u, v in combinations(keys, 2))
        assert got == want
    print('PASS tree models: DFS intervals, point/edge differences, reroot, directed HLD, virtual distances', flush=True)
