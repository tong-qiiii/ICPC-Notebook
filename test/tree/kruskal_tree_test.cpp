#include "code/tree/kruskal_tree.hpp"

int main()
{
    mt19937 rng(38016);
    KRT krt;
    krt.build();
    assert(krt.roots.empty() && krt.tot == 0);
    for (int tc = 0; tc < 180; tc++)
    {
        int n = 1 + rng() % 20;
        krt.init(n);
        vector<vector<pair<int, ll>>> g(n + 1);
        const __int128 INF = (__int128)1 << 120;
        vector<vector<__int128>> dis(n + 1, vector<__int128>(n + 1, INF));
        for (int u = 1; u <= n; u++) dis[u][u] = -INF;
        for (int i = 0, m = rng() % 90; i < m; i++)
        {
            int u = 1 + rng() % n, v = 1 + rng() % n;
            ll w = (int)(rng() % 15) - 7;
            if (i == 0) w = LLONG_MIN;
            if (i == 1) w = LLONG_MAX;
            krt.add(u, v, w); g[u].push_back({v, w}); g[v].push_back({u, w});
            if (u != v) dis[u][v] = dis[v][u] = min(dis[u][v], (__int128)w);
        }
        for (int k = 1; k <= n; k++) for (int u = 1; u <= n; u++) for (int v = 1; v <= n; v++)
        {
            dis[u][v] = min(dis[u][v], max(dis[u][k], dis[k][v]));
        }
        for (int repeat = 0; repeat < 2; repeat++)
        {
            krt.build();
            assert(krt.tot <= 2 * n - 1);
            set<int> pos;
            for (int u = 1; u <= n; u++) pos.insert(krt.dfn[u]);
            assert((int)pos.size() == n && *pos.begin() == 1 && *pos.rbegin() == n);
            for (int u = 1; u <= n; u++) for (int v = 1; v <= n; v++)
            {
                assert(krt.connected(u, v) == (dis[u][v] != INF));
                if (!krt.connected(u, v)) assert(krt.lca(u, v) == 0);
                else assert(krt.bottleneck(u, v) == (u == v ? 0 : (ll)dis[u][v]));
            }
            for (int u = 1; u <= n; u++) for (ll threshold : {LLONG_MIN, -8LL, -1LL, 0LL, 2LL, 8LL, LLONG_MAX})
            {
                set<int> expected = {u}; queue<int> q; q.push(u);
                while (!q.empty())
                {
                    int z = q.front(); q.pop();
                    for (auto [v, w] : g[z]) if (w <= threshold && expected.insert(v).second) q.push(v);
                }
                int p = krt.climb(u, threshold);
                assert(krt.siz[p] == (int)expected.size());
                set<int> actual;
                for (int v = 1; v <= n; v++)
                {
                    if (krt.dfn[p] <= krt.dfn[v] && krt.dfn[v] < krt.dfn[p] + krt.siz[p]) actual.insert(v);
                }
                assert(actual == expected);
            }
        }
    }
    krt.init(2); krt.add(1, 2, 5); krt.build();
    krt.init(0); krt.build(); assert(krt.roots.empty());
    cout << "PASS kruskal_tree\n";
}
