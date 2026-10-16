#include "code/dp/knapsack_group.hpp"
int main()
{
    mt19937 gen(20261016);
    auto d = knapsack_group(
        4, vector<vector<pair<int, ll>>>{{}, {{2, 3}, {3, 7}}, {{1, 2}}});
    assert(d[4] == 9);
    {
        for (int run = 0; run < 500; run++)
        {
            int n = gen() % 5, C = gen() % 20;
            vector<vector<pair<int, ll>>> g(n + 1);
            for (int i = 1; i <= n; i++)
            {
                for (int j = gen() % 5; j; j--)
                {
                    g[i].push_back({gen() % 8 + 1, int(gen() % 21) - 5});
                }
            }
            ll best = 0;
            auto dfs = [&](auto &&self, int i, int used, ll val) -> void
            {
                if (i > n)
                {
                    best = max(best, val);
                    return;
                }
                self(self, i + 1, used, val);
                for (auto [w, v] : g[i])
                {
                    if (used + w <= C)
                    {
                        self(self, i + 1, used + w, val + v);
                    }
                }
            };
            dfs(dfs, 1, 0, 0);
            assert(knapsack_group(C, g)[C] == best);
        }
    }
    cout << "PASS knapsack_group\n";
}
