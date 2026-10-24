#include "../../code/bit_greedy/min_or_tree.hpp"
#include "support.hpp"

ll brute(int n, const vector<tuple<int, int, ll>> &e)
{
    ll ans = LLONG_MAX;
    for (int mask = 0; mask < (1 << e.size()); mask++)
    {
        if (__builtin_popcount((unsigned)mask) != n - 1)
        {
            continue;
        }
        vector<vector<int>> g(n + 1);
        ll val = 0;
        for (int i = 0; i < (int)e.size(); i++)
        {
            if (!(mask >> i & 1))
            {
                continue;
            }
            auto [u, v, w] = e[i];
            g[u].push_back(v);
            g[v].push_back(u);
            val |= w;
        }
        queue<int> q;
        vector<int> vis(n + 1);
        q.push(1);
        vis[1] = 1;
        int cnt = 0;
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            cnt++;
            for (int v : g[u])
            {
                if (vis[v])
                {
                    continue;
                }
                vis[v] = 1;
                q.push(v);
            }
        }
        if (cnt == n)
        {
            ans = min(ans, val);
        }
    }
    return ans == LLONG_MAX ? -1 : ans;
}

int main()
{
    for (int t = 0; t < 700; t++)
    {
        int n = rnd(1, 5), m = rnd(0, 8);
        vector<tuple<int, int, ll>> e;
        for (int i = 0; i < m; i++)
        {
            e.push_back({rnd(1, n), rnd(1, n), rnd(0, 31)});
        }
        assert(min_or_tree(n, e) == brute(n, e));
    }
    assert(min_or_tree(2, {{1, 2, 1LL << 61}}) == (1LL << 61));
    assert(min_or_tree(1, {}) == 0);
    assert(min_or_tree(3, {{1, 2, 0}}) == -1);
    cout << "PASS min_or_tree: 700 edge-subset brute cases, loops, multiedges, disconnected\n";
}
