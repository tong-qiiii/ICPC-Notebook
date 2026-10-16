#include "code/dp/reroot_distance.hpp"
int main()
{
    mt19937 gen(20261016);
    auto a = reroot_distance(
        vector<vector<pair<int, ll>>>{{}, {{2, 3}}, {{1, 3}}});
    assert(a[1] == 3 && a[2] == 3);
    {
        for (int n = 1; n <= 70; n++)
        {
            vector<vector<pair<int, ll>>> g(n + 1);
            for (int i = 2; i <= n; i++)
            {
                int p = gen() % (i - 1) + 1;
                ll w = gen() % 20;
                g[p].push_back({i, w});
                g[i].push_back({p, w});
            }
            auto ans = reroot_distance(g);
            assert(ans == reroot_distance(g));
            for (int rt = 1; rt <= n; rt++)
            {
                ll ref = 0;
                auto dfs = [&](auto &&self, int u, int p, ll d) -> void
                {
                    ref += d;
                    for (auto [v, w] : g[u])
                    {
                        if (v != p)
                        {
                            self(self, v, u, d + w);
                        }
                    }
                };
                dfs(dfs, rt, 0, 0);
                assert(ans[rt] == ref);
            }
        }
    }
    cout << "PASS reroot_distance\n";
}
