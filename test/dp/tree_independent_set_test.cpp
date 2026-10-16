#include "code/dp/tree_independent_set.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(tree_independent_set(vector<vector<int>>{{}, {2, 3}, {1}, {1}},
                                vector<ll>{0, 5, 4, 3}) == 7);
    {
        for (int t = 0; t < 300; t++)
        {
            int n = gen() % 13 + 1;
            vector<vector<int>> g(n + 1);
            vector<pair<int, int>> es;
            vector<ll> w(n + 1);
            for (int i = 1; i <= n; i++)
            {
                w[i] = int(gen() % 31) - 10;
                if (i > 1)
                {
                    int p = gen() % (i - 1) + 1;
                    g[i].push_back(p);
                    g[p].push_back(i);
                    es.push_back({p, i});
                }
            }
            ll ref = 0;
            for (int mask = 0; mask < (1 << n); mask++)
            {
                bool ok = true;
                for (auto [u, v] : es)
                {
                    if ((mask >> (u - 1) & 1) && (mask >> (v - 1) & 1))
                    {
                        ok = false;
                    }
                }
                if (ok)
                {
                    ll z = 0;
                    for (int i = 1; i <= n; i++)
                    {
                        if (mask >> (i - 1) & 1)
                        {
                            z += w[i];
                        }
                    }
                    ref = max(ref, z);
                }
            }
            assert(tree_independent_set(g, w) == ref);
        }
    }
    cout << "PASS tree_independent_set\n";
}
