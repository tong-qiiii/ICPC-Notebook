#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// 仅适用于非负边权；路径和须在 ll 范围内。
inline tuple<ll, int, int> tree_diameter_dfs(
    const vector<vector<pair<int, ll>>>& g)
{
    int n = (int)g.size() - 1;
    if (n <= 0)
    {
        return {0, 0, 0};
    }
    int s = 1, t = 1;
    ll ans = 0;
    auto dfs = [&](auto&& self, int u, int fa, ll dis) -> void
    {
        if (dis > ans)
        {
            ans = dis;
            t = u;
        }
        for (auto [v, w] : g[u])
        {
            if (v == fa) continue;
            self(self, v, u, dis + w);
        }
    };
    dfs(dfs, 1, 0, 0);
    s = t;
    ans = 0;
    dfs(dfs, s, 0, 0);
    return {ans, s, t};
}
