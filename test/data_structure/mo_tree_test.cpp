#include "code/data_structure/mo_tree.hpp"
int main()
{
    mt19937 gen(20261016);
    vector<vector<int>> g{{}, {2, 3}, {1}, {1}};
    ll now = 0;
    auto ans = mo_tree(
        g, vector<pair<int, int>>{{2, 3}}, now, [&](int u) { now += u; },
        [&](int u) { now -= u; });
    assert(ans[0] == 6);
    {
        for (int n = 1; n <= 70; n++)
        {
            vector<vector<int>> g(n + 1);
            vector<int> par(n + 1), dep(n + 1);
            for (int i = 2; i <= n; i++)
            {
                par[i] = gen() % (i - 1) + 1;
                dep[i] = dep[par[i]] + 1;
                g[i].push_back(par[i]);
                g[par[i]].push_back(i);
            }
            vector<pair<int, int>> q;
            vector<ll> ref;
            for (int k = 0; k < 100; k++)
            {
                int u = gen() % n + 1, v = gen() % n + 1;
                q.push_back({u, v});
                ll s = 0;
                while (u != v)
                {
                    if (dep[u] < dep[v])
                    {
                        swap(u, v);
                    }
                    s += u;
                    u = par[u];
                }
                ref.push_back(s + u);
            }
            ll now = 0;
            assert(mo_tree(
                       g, q, now, [&](int u) { now += u; }, [&](int u) { now -= u; }) ==
                   ref);
        }
    }
    cout << "PASS mo_tree\n";
}
