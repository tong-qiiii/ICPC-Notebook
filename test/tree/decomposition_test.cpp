#include "code/tree/dsu_on_tree.hpp"
#include "code/tree/centroid.hpp"

int main()
{
    mt19937 rng(3162026);
    DSUOnTree dsu;
    Centroid cd;
    assert(dsu.work().size() == 1 && cd.work() == 0);
    for (int tc = 0; tc < 220; tc++)
    {
        int n = 1 + rng() % 75;
        dsu.init(n); cd.init(n, (int)(rng() % 121) - 60);
        vector<vector<pair<int, ll>>> g(n + 1);
        for (int u = 2; u <= n; u++)
        {
            int p = tc % 3 == 0 ? u - 1 : tc % 3 == 1 ? 1 : 1 + rng() % (u - 1);
            ll w = (int)(rng() % 201) - 100;
            g[u].push_back({p, w}); g[p].push_back({u, w});
            dsu.add(u, p); cd.add(u, p, w);
        }
        for (int repeat = 0; repeat < 3; repeat++)
        {
            int root = 1 + rng() % n;
            for (int u = 1; u <= n; u++)
            {
                int c = (int)(rng() % 9) - 4;
                if (u == 1) c = INT_MIN;
                if (u == n) c = INT_MAX;
                if (repeat == 2) c = 0;
                dsu.set_color(u, c);
            }
            auto ans = dsu.work(root);
            assert(dsu.work(root) == ans && dsu.diff == 0);
            function<set<int>(int, int)> dfs = [&](int u, int p)
            {
                set<int> colors = {dsu.color[u]};
                for (auto [v, w] : g[u])
                {
                    if (v == p) continue;
                    auto sub = dfs(v, u); colors.insert(sub.begin(), sub.end());
                }
                assert(ans[u] == (int)colors.size());
                return colors;
            };
            dfs(root, 0);
            ll expected = 0;
            for (int s = 1; s <= n; s++)
            {
                function<void(int, int, __int128)> walk = [&](int u, int p, __int128 dis)
                {
                    if (u > s && dis <= cd.k) expected++;
                    for (auto [v, w] : g[u]) if (v != p) walk(v, u, dis + w);
                };
                walk(s, 0, 0);
            }
            assert(cd.work() == expected && cd.work() == expected);
            cd.k += 17;
        }
    }
    cd.init(3, LLONG_MAX);
    cd.add(1, 2, LLONG_MAX); cd.add(2, 3, LLONG_MAX);
    assert(cd.work() == 2);
    cd.init(3, LLONG_MIN);
    cd.add(1, 2, LLONG_MIN); cd.add(2, 3, LLONG_MIN);
    assert(cd.work() == 3);
    cd.init(4, -1);
    cd.add(1, 2, LLONG_MIN); cd.add(2, 3, LLONG_MAX); cd.add(2, 4, LLONG_MAX);
    assert(cd.work() == 3);
    dsu.init(1800);
    for (int u = 1; u <= 1800; u++)
    {
        dsu.set_color(u, u);
        if (u > 1) dsu.add(u - 1, u);
    }
    auto ans = dsu.work();
    for (int u = 1; u <= 1800; u++) assert(ans[u] == 1801 - u);
    cd.init(1800, 0);
    for (int u = 2; u <= 1800; u++) cd.add(1, u, 0);
    assert(cd.work() == 1800LL * 1799 / 2);
    dsu.init(0); cd.init(0, 0);
    assert(dsu.work().size() == 1 && cd.work() == 0);
    cout << "PASS dsu_on_tree\nPASS centroid\n";
}
