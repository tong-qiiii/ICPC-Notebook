"""Small independent test oracles, not Notebook templates."""
from collections import deque


class BIT:
    def __init__(self, n):
        self.t = [0]*(n+1)

    def add(self, i, x):
        assert i > 0
        while i < len(self.t):
            self.t[i] += x
            i += i & -i

    def sum(self, i):
        ans = 0
        while i:
            ans += self.t[i]
            i -= i & -i
        return ans


def maxflow(n, edges, s, t):
    """Edmonds-Karp oracle with parallel capacities combined."""
    cap = [[0]*n for _ in range(n)]
    for u, v, w in edges:
        cap[u][v] += w
    total = 0
    while True:
        prev = [-1]*n
        prev[s] = s
        queue = deque([s])
        while queue and prev[t] == -1:
            u = queue.popleft()
            for v, w in enumerate(cap[u]):
                if w > 0 and prev[v] == -1:
                    prev[v] = u
                    queue.append(v)
        if prev[t] == -1:
            return total
        v, amount = t, 10**30
        while v != s:
            amount = min(amount, cap[prev[v]][v])
            v = prev[v]
        v = t
        while v != s:
            cap[prev[v]][v] -= amount
            cap[v][prev[v]] += amount
            v = prev[v]
        total += amount
