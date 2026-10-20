#include "code/graph/hopcroft_karp.hpp"

int main()
{
    mt19937 rg(735);
    for (int z = 0; z < 1000; z++)
    {
        int n = rg() % 6, m = rg() % 6;
        HopcroftKarp h(n, m);
        for (int u = 1; u <= n; u++) for (int v = 1; v <= m; v++)
        {
            if (rg() % 2) h.add(u, v);
            if (rg() % 4 == 0) h.add(u, v);
        }
        int best = n + m;
        for (int s = 0; s < (1 << (n + m)); s++)
        {
            bool ok = true;
            for (int u = 1; u <= n; u++) for (int v : h.g[u])
                if (!(s >> (u - 1) & 1) && !(s >> (n + v - 1) & 1)) ok = false;
            if (ok) best = min(best, __builtin_popcount((unsigned)s));
        }
        assert(h.work() == best);
        auto [l, r] = h.cover();
        assert((int)(l.size() + r.size()) == best);
        vector<int> a(n + 1), b(m + 1);
        for (int u : l) a[u] = 1;
        for (int v : r) b[v] = 1;
        for (int u = 1; u <= n; u++) for (int v : h.g[u]) assert(a[u] || b[v]);
        assert(h.cover() == make_pair(l, r));
        assert(h.work() == best);
    }
    cout << "PASS matching_cover: exhaustive vertex covers, complement independent set\n";
}
