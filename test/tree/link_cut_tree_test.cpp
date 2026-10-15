#include "code/tree/link_cut_tree.hpp"

int main()
{
    mt19937 rng(391610);
    LCT lct;
    assert(lct.siz[0] == 0 && lct.sum[0] == 0);
    for (int tc = 0; tc < 65; tc++)
    {
        int n = 1 + rng() % 27;
        lct.init(n);
        vector<vector<int>> g(n + 1, vector<int>(n + 1));
        vector<ll> val(n + 1);
        auto path = [&](int s, int t)
        {
            vector<int> par(n + 1, -1), res;
            queue<int> q; q.push(s); par[s] = 0;
            while (!q.empty())
            {
                int u = q.front(); q.pop();
                for (int v = 1; v <= n; v++) if (g[u][v] && par[v] < 0) par[v] = u, q.push(v);
            }
            if (par[t] < 0) return res;
            for (int u = t; u; u = par[u]) res.push_back(u);
            return res;
        };
        for (int step = 0; step < 2400; step++)
        {
            int u = 1 + rng() % n, v = 1 + rng() % n, op = rng() % 8;
            ll w = (int)(rng() % 201) - 100;
            auto nodes = path(u, v);
            if (op == 0)
            {
                bool expected = nodes.empty();
                assert(lct.link(u, v) == expected);
                if (expected) g[u][v] = g[v][u] = 1;
            }
            else if (op == 1)
            {
                bool expected = g[u][v];
                assert(lct.cut(u, v) == expected);
                if (expected) g[u][v] = g[v][u] = 0;
            }
            else if (op == 2)
            {
                lct.set_val(u, w); val[u] = w;
            }
            else if (op == 3 && !nodes.empty())
            {
                lct.path_add(u, v, w);
                for (int z : nodes) val[z] += w;
            }
            else if (op == 4 && !nodes.empty())
            {
                ll sum = 0; for (int z : nodes) sum += val[z];
                assert(lct.path_sum(u, v) == sum);
                lct.split(u, v);
                assert(lct.sum[v] == sum && lct.siz[v] == (int)nodes.size());
            }
            else if (op == 5) lct.makeroot(u);
            else if (op == 6) assert(!path(u, lct.findroot(u)).empty());
            assert(lct.connected(u, v) == !path(u, v).empty());
            assert(lct.path_sum(u, u) == val[u]);
            assert(lct.siz[0] == 0 && lct.sum[0] == 0 && lct.val[0] == 0);
        }
    }
    lct.init(2); assert(lct.link(1, 2));
    lct.set_val(1, 3000000000000000000LL); lct.set_val(2, 3000000000000000000LL);
    assert(lct.path_sum(1, 2) == 6000000000000000000LL);
    lct.path_add(1, 2, -1000000000000000000LL);
    assert(lct.path_sum(1, 2) == 4000000000000000000LL);
    assert(lct.cut(1, 2) && !lct.cut(1, 2));
    lct.init(4000);
    for (int u = 2; u <= 4000; u++) assert(lct.link(u - 1, u));
    lct.path_add(1, 4000, 2); lct.set_val(2000, -5);
    assert(lct.path_sum(1, 4000) == 7993);
    assert(!lct.link(1, 4000) && !lct.cut(1, 4000));
    assert(lct.cut(2000, 2001) && !lct.connected(1, 4000));
    lct.init(0); assert(lct.siz[0] == 0);
    cout << "PASS link_cut_tree\n";
}
