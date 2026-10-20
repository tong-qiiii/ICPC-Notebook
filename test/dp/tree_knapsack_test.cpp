#include "code/dp/tree_knapsack.hpp"

int main()
{
    mt19937 rg(743);
    for (int z = 0; z < 1200; z++)
    {
        int n = 1 + rg() % 11, root = 1 + rg() % n, k = rg() % (n + 3);
        vector<vector<int>> g(n + 1);
        vector<ll> w(n + 1);
        for (int i = 1; i <= n; i++) w[i] = int(rg() % 21) - 10;
        for (int i = 2; i <= n; i++)
        {
            int p = 1 + rg() % (i - 1);
            g[p].push_back(i); g[i].push_back(p);
        }
        vector<ll> best(k + 1, LLONG_MIN / 4);
        best[0] = 0;
        for (int s = 1; s < (1 << n); s++)
        {
            int cnt = __builtin_popcount((unsigned)s);
            if (!(s >> (root - 1) & 1) || cnt > k) continue;
            vector<int> seen(n + 1);
            queue<int> q;
            q.push(root); seen[root] = 1;
            while (!q.empty())
            {
                int u = q.front(); q.pop();
                for (int v : g[u]) if ((s >> (v - 1) & 1) && !seen[v]) seen[v] = 1, q.push(v);
            }
            if (accumulate(seen.begin(), seen.end(), 0) != cnt) continue;
            ll sum = 0;
            for (int i = 1; i <= n; i++) if (seen[i]) sum += w[i];
            best[cnt] = max(best[cnt], sum);
        }
        assert(tree_knapsack(g, w, k, root) == best);
        assert(tree_knapsack(g, w, k, root) == best);
    }
    cout << "PASS tree_knapsack: exhaustive connected subsets, negative weights, random roots, k>n\n";
}
