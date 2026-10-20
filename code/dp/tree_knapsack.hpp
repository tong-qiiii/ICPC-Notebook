#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// 恰选 j 个点，非空方案必须包含 root 且连通；dp[0] = 0。
inline vector<ll> tree_knapsack(const vector<vector<int>> &g,
                                const vector<ll> &w, int k, int root = 1)
{
    const ll NEG = LLONG_MIN / 4;
    int n = (int)g.size() - 1;
    assert(k >= 0 && root >= 1 && root <= n);
    if (!k) return {0};
    auto dfs = [&](auto &&self, int u, int fa) -> vector<ll>
    {
        vector<ll> dp = {0, w[u]};
        for (int v : g[u])
        {
            if (v == fa) continue;
            auto f = self(self, v, u);
            int old = (int)dp.size() - 1, sz = (int)f.size() - 1;
            int lim = min(k, old + sz);
            dp.resize(lim + 1, NEG);
            for (int j = lim; j >= 1; j--)
            {
                for (int t = max(1, j - old); t <= min(sz, j - 1); t++)
                {
                    if (dp[j - t] != NEG && f[t] != NEG)
                        dp[j] = max(dp[j], dp[j - t] + f[t]);
                }
            }
        }
        return dp;
    };
    auto ans = dfs(dfs, root, 0);
    ans.resize(k + 1, NEG);
    return ans;
}
