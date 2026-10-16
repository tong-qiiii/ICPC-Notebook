#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline ll tree_independent_set(const vector<vector<int>> &g,
                               const vector<ll> &w)
{
    int n = (int)g.size() - 1;
    if (n <= 0)
    {
        return 0;
    }
    vector<array<ll, 2>> dp(n + 1);
    auto dfs = [&](auto &&self, int u, int fa) -> void
    {
        dp[u][0] = 0;
        dp[u][1] = w[u];

        for (int v : g[u])
        {
            if (v == fa)
            {
                continue;
            }
            self(self, v, u);

            dp[u][0] += max(dp[v][0], dp[v][1]);
            dp[u][1] += dp[v][0];
        }
    };

    dfs(dfs, 1, 0);
    return max(dp[1][0], dp[1][1]);
}
