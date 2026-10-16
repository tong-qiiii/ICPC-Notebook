#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// 允许负边和零边路径；全负边返回 0，端点可以相同。
inline tuple<ll, int, int> tree_diameter_dp(
    const vector<vector<pair<int, ll>>>& g)
{
    int n = (int)g.size() - 1;
    if (n <= 0)
    {
        return {0, 0, 0};
    }
    ll ans = 0;
    int s = 1, t = 1;
    auto dfs = [&](auto&& self, int u, int fa) -> pair<ll, int>
    {
        pair<ll, int> best{0, u};
        for (auto [v, w] : g[u])
        {
            if (v == fa) continue;
            auto [len, id] = self(self, v, u);
            len += w;
            // 先拼接，再更新，保证两段来自不同子树。
            if (best.first + len > ans)
            {
                ans = best.first + len;
                s = best.second;
                t = id;
            }
            if (len > best.first)
            {
                best = {len, id};
            }
        }
        return best;
    };
    dfs(dfs, 1, 0);
    return {ans, s, t};
}
