#include "code/counting/matrix_tree.hpp"

int main()
{
    const int P = 101;
    mt19937 rg(739);
    for (int z = 0; z < 800; z++)
    {
        int n = 1 + rg() % 5, m = rg() % 10, root = 1 + rg() % n;
        vector<tuple<int, int, ll>> e;
        for (int i = 0; i < m; i++) e.push_back({1 + int(rg() % n), 1 + int(rg() % n), int(rg() % 9) - 4});
        for (bool directed : {false, true})
        {
            ll ans = 0;
            for (int s = 0; s < (1 << m); s++)
            {
                if (__builtin_popcount((unsigned)s) != n - 1) continue;
                vector<vector<int>> g(n + 1);
                vector<int> out(n + 1);
                ll prod = 1;
                for (int i = 0; i < m; i++) if (s >> i & 1)
                {
                    auto [u, v, w] = e[i];
                    prod = prod * w % P;
                    if (directed) g[v].push_back(u), out[u]++;
                    else g[v].push_back(u), g[u].push_back(v);
                }
                vector<int> seen(n + 1);
                queue<int> q;
                q.push(root); seen[root] = 1;
                while (!q.empty())
                {
                    int u = q.front(); q.pop();
                    for (int v : g[u]) if (!seen[v]) seen[v] = 1, q.push(v);
                }
                bool ok = accumulate(seen.begin(), seen.end(), 0) == n;
                if (directed) for (int i = 1; i <= n; i++) if (out[i] != (i != root)) ok = false;
                if (ok) ans = (ans + prod) % P;
            }
            assert(tree_count(n, e, P, root, directed) == (ans + P) % P);
        }
    }
    cout << "PASS matrix_tree: enumerate weighted edge subsets, both directions, loops, parallel edges\n";
}
