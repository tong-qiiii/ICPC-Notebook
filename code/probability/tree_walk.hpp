#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// 无权树上等概率选邻居，求每个点首次到 t 的期望步数。
inline vector<ll> tree_walk(const vector<vector<int>> &g, int t)
{
    int n = (int)g.size() - 1;
    assert(1 <= t && t <= n);
    vector<int> siz(n + 1);
    vector<ll> dp(n + 1);
    auto dfs = [&](auto &&self, int u, int fa) -> void
    {
        siz[u] = 1;
        for (int v : g[u])
        {
            if (v == fa) continue;
            self(self, v, u);
            siz[u] += siz[v];
        }
    };
    auto work = [&](auto &&self, int u, int fa) -> void
    {
        for (int v : g[u])
        {
            if (v == fa) continue;
            dp[v] = dp[u] + 2LL * siz[v] - 1;
            self(self, v, u);
        }
    };
    dfs(dfs, t, 0);
    work(work, t, 0);
    return dp;
}
