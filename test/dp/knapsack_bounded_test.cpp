#include "code/dp/knapsack_bounded.hpp"
int main()
{
    mt19937 gen(20261016);
    auto d = knapsack_bounded(6, vector<BoundedItem>{{}, {2, 2, 3}});
    assert(d[6] == 6);
    {
        for (int run = 0; run < 500; run++)
        {
            int n = gen() % 6, C = gen() % 20;
            vector<BoundedItem> a(1);
            for (int i = 0; i < n; i++)
            {
                a.push_back({int(gen() % 8) + 1, int(gen() % 5),
                             int(gen() % 21) - 5});
            }
            ll best = 0;
            auto dfs = [&](auto &&self, int i, int used, ll val) -> void
            {
                if (i > n)
                {
                    best = max(best, val);
                    return;
                }
                auto [w, c, v] = a[i];
                for (int k = 0; k <= c && used + k * w <= C; k++)
                {
                    self(self, i + 1, used + k * w, val + k * v);
                }
            };
            dfs(dfs, 1, 0, 0);
            assert(knapsack_bounded(C, a)[C] == best);
        }
        assert(knapsack_bounded(
                   10, vector<BoundedItem>{
                           {}, {INT_MAX, INT_MAX, LLONG_MAX}})[10] == 0);
    }
    cout << "PASS knapsack_bounded\n";
}
