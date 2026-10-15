#include "code/tree/lca.hpp"
#include "code/tree/lca_hld.hpp"
#include "code/tree/lca_tarjan.hpp"
#include "code/tree/hld.hpp"
#include "code/tree/virtual_tree.hpp"

int main()
{
    mt19937 rng(20261016);
    LCA a;
    LCAHLD b;
    LCATarjan c;
    HLD h;
    VTree vt;
    a.build(); b.build(); c.work(); h.build(); vt.prep();
    assert(vt.build({}) == 0);
    for (int tc = 0; tc < 180; tc++)
    {
        int n = tc < 3 ? tc + 1 : 1 + rng() % 65;
        a.init(n); b.init(n); c.init(n); h.init(n); vt.init(n);
        vector<vector<int>> g(n + 1);
        for (int u = 2; u <= n; u++)
        {
            int p = tc % 4 == 0 ? u - 1 : tc % 4 == 1 ? 1 : 1 + rng() % (u - 1);
            g[u].push_back(p); g[p].push_back(u);
            a.add(u, p); b.add(u, p); c.add(u, p); h.add(u, p); vt.add(u, p);
        }
        vector<pair<int, int>> qs;
        for (int i = 0; i < 90; i++)
        {
            int u = 1 + rng() % n, v = i < n ? u : 1 + rng() % n;
            qs.push_back({u, v});
            assert(c.ask(u, v) == i);
        }
        for (int repeat = 0; repeat < 3; repeat++)
        {
            int root = repeat == 0 ? 1 : 1 + rng() % n;
            vector<int> par(n + 1), dep(n + 1);
            queue<int> q; q.push(root); dep[root] = 1;
            while (!q.empty())
            {
                int u = q.front(); q.pop();
                for (int v : g[u])
                {
                    if (v == par[u]) continue;
                    par[v] = u; dep[v] = dep[u] + 1; q.push(v);
                }
            }
            auto ancestor = [&](int u, int v)
            {
                while (v && v != u) v = par[v];
                return v == u;
            };
            auto brute = [&](int u, int v)
            {
                while (dep[u] > dep[v]) u = par[u];
                while (dep[v] > dep[u]) v = par[v];
                while (u != v) u = par[u], v = par[v];
                return u;
            };
            a.build(root); b.build(root); h.build(root); vt.prep(root);
            auto ans = c.work(root);
            assert(c.work(root) == ans);
            for (int id = 0; id < (int)qs.size(); id++)
            {
                auto [u, v] = qs[id];
                int p = brute(u, v);
                assert(a.lca(u, v) == p && b.lca(u, v) == p);
                assert(h.lca(u, v) == p && vt.lca(u, v) == p && ans[id] == p);
                set<int> expected = {p}, seen;
                for (int z = u; z != p; z = par[z]) expected.insert(z);
                for (int z = v; z != p; z = par[z]) expected.insert(z);
                vector<int> rev(n + 1);
                for (int z = 1; z <= n; z++) rev[h.dfn[z]] = z;
                auto add = [&](int l, int r)
                {
                    assert(1 <= l && l <= r && r <= n);
                    for (int i = l; i <= r; i++) assert(seen.insert(rev[i]).second);
                };
                h.path(u, v, add);
                assert(seen == expected);
                seen.clear(); expected.erase(p);
                h.path_edge(u, v, add);
                assert(seen == expected);
            }
            for (int u = 1; u <= n; u++)
            {
                int p = u;
                for (int k = 0; k <= n + 1; k++)
                {
                    assert(a.kth_up(u, k) == p);
                    p = par[p];
                }
                assert(a.kth_up(u, INT_MAX) == 0 && a.kth_up(u, -1) == 0);
                auto [l, r] = h.subtree(u);
                int cnt = 0;
                for (int v = 1; v <= n; v++)
                {
                    bool inside = l <= h.dfn[v] && h.dfn[v] <= r;
                    assert(inside == ancestor(u, v)); cnt += inside;
                }
                assert(cnt == h.siz[u] && cnt == r - l + 1);
            }
            for (int query = 0; query < 12; query++)
            {
                vector<int> keys;
                for (int i = 0, k = rng() % (2 * n + 1); i < k; i++) keys.push_back(1 + rng() % n);
                if (query == 0) keys.clear();
                if (query == 1) keys = {root, root};
                int rt = vt.build(keys);
                set<int> expected(keys.begin(), keys.end());
                for (int u : keys) for (int v : keys) expected.insert(brute(u, v));
                assert(set<int>(vt.nodes.begin(), vt.nodes.end()) == expected);
                if (keys.empty()) assert(rt == 0);
                else assert(rt == vt.nodes[0] && expected.count(rt));
                int edges = 0;
                vector<int> parent(n + 1);
                for (int u = 1; u <= n; u++)
                {
                    if (!expected.count(u)) assert(vt.vt[u].empty());
                    for (int v : vt.vt[u])
                    {
                        assert(!parent[v]); parent[v] = u; edges++;
                        int best = 0;
                        for (int z : expected)
                        {
                            if (z != v && ancestor(z, v) && dep[z] > dep[best]) best = z;
                        }
                        assert(u == best);
                    }
                }
                assert(edges == max(0, (int)expected.size() - 1));
            }
            vt.build({});
            for (const auto &edges : vt.vt) assert(edges.empty());
        }
        c.ask(1, n);
        assert((int)c.work().size() == (int)qs.size() + 1);
    }
    a.init(1025); b.init(1025); c.init(1025); h.init(1025); vt.init(1025);
    for (int u = 2; u <= 1025; u++)
    {
        a.add(u - 1, u); b.add(u - 1, u); c.add(u - 1, u);
        h.add(u - 1, u); vt.add(u - 1, u);
    }
    a.build(); b.build(); h.build(); vt.prep();
    assert(a.kth_up(1025, 1024) == 1 && a.kth_up(1025, 1025) == 0);
    assert(b.lca(513, 1025) == 513 && h.lca(513, 1025) == 513);
    c.ask(513, 1025); assert(c.work()[0] == 513);
    assert(vt.build({1, 1025}) == 1 && vt.nodes.size() == 2);
    cout << "PASS lca\nPASS lca_hld\nPASS lca_tarjan\nPASS hld\nPASS virtual_tree\n";
}
