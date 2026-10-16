#include "code/probability/tree_walk.hpp"
#include "code/probability/markov_absorb.hpp"

int main()
{
    mt19937 gen(1107);
    for (int k = 0; k < 150; k++)
    {
        int n = 1 + gen() % 14, t = 1 + gen() % n;
        vector<vector<int>> g(n + 1);
        for (int u = 2; u <= n; u++)
        {
            int v = 1 + gen() % (u - 1);
            g[u].push_back(v); g[v].push_back(u);
        }
        vector<vector<db>> p(n + 1, vector<db>(n + 1));
        vector<int> end(n + 1); end[t] = 1;
        for (int u = 1; u <= n; u++)
        {
            for (int v : g[u]) p[u][v] = 1.0 / g[u].size();
        }
        auto ans = tree_walk(g, t);
        auto ref = markov_absorb(p, end);
        for (int u = 1; u <= n; u++) assert(abs(ans[u] - ref.mean[u]) < 1e-8);
    }
    vector<vector<int>> chain(1001);
    for (int u = 2; u <= 1000; u++) chain[u - 1].push_back(u), chain[u].push_back(u - 1);
    assert(tree_walk(chain, 1)[1000] == 999LL * 999);
    cout << "PASS tree walk: 150 random trees vs linear systems, singleton and chain\n";
}
