#pragma once
#include <bits/stdc++.h>
using namespace std;

inline vector<int> tree_centroids(const vector<vector<int>>& g)
{
    int n = (int)g.size() - 1;
    vector<int> res;
    if (n <= 0)
    {
        return res;
    }
    auto dfs = [&](auto&& self, int u, int fa) -> int
    {
        int siz = 1, mx = 0;
        for (int v : g[u])
        {
            if (v == fa) continue;
            int cnt = self(self, v, u);
            siz += cnt;
            mx = max(mx, cnt);
        }
        // 别漏掉父亲方向的连通块。
        mx = max(mx, n - siz);
        if (mx <= n / 2)
        {
            res.push_back(u);
        }
        return siz;
    };
    dfs(dfs, 1, 0);
    return res;
}
