#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline vector<ll> reroot_distance(const vector<vector<pair<int, ll>>> &g)
{
    int n = (int)g.size() - 1;
    vector<int> siz(n + 1);
    vector<ll> down(n + 1), ans(n + 1);
    if (!n)
    {
        return ans;
    }
    auto dfs1 = [&](auto &&self, int u, int fa) -> void
    {
        siz[u] = 1;

        for (auto [v, w] : g[u])
        {
            if (v == fa)
            {
                continue;
            }
            self(self, v, u);

            siz[u] += siz[v];
            down[u] += down[v] + 1LL * siz[v] * w;
        }
    };

    auto dfs2 = [&](auto &&self, int u, int fa) -> void
    {
        for (auto [v, w] : g[u])
        {
            if (v == fa)
            {
                continue;
            }

            ans[v] = ans[u] + 1LL * (n - 2 * siz[v]) * w;
            self(self, v, u);
        }
    };

    dfs1(dfs1, 1, 0);
    ans[1] = down[1];
    dfs2(dfs2, 1, 0);

    return ans;
}
