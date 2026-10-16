#include "code/dp/knapsack01.hpp"
int main()
{
    mt19937 gen(20261016);
    auto dp = knapsack01(5, vector<pair<int, ll>>{{}, {2, 3}, {3, 5}});
    assert(dp[5] == 8);
    {
        for (int run = 0; run < 500; run++)
        {
            int n = gen() % 8, C = gen() % 20;
            vector<pair<int, ll>> a(1);
            for (int i = 0; i < n; i++)
            {
                a.push_back({int(gen() % 8) + 1, int(gen() % 21) - 5});
            }
            for (bool exact : {false, true})
            {
                ll best = exact ? LLONG_MIN / 4 : 0;
                auto dfs = [&](auto &&self, int i, int used, ll val) -> void
                {
                    if (i > n)
                    {
                        if ((!exact && used <= C) || (exact && used == C))
                        {
                            best = max(best, val);
                        }
                        return;
                    }
                    auto [w, v] = a[i];
                    for (int k = 0; k <= 1 && used + k * w <= C; k++)
                    {
                        self(self, i + 1, used + k * w, val + k * v);
                    }
                };
                dfs(dfs, 1, 0, 0);
                assert(knapsack01(C, a, exact)[C] == best);
            }
        }
    }
    cout << "PASS knapsack01\n";
}
