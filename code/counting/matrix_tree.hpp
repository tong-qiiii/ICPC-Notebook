#pragma once
#include <bits/stdc++.h>
using namespace std;
#include "../math/determinant.hpp"

// directed=true：沿有向边最终走向 root 的内向生成树。
inline ll tree_count(int n, const vector<tuple<int, int, ll>> &e,
                     int mod, int root = 1, bool directed = false)
{
    assert(n >= 1 && root >= 1 && root <= n && mod >= 2);
    vector<vector<ll>> a(n, vector<ll>(n));
    auto add = [&](int u, int v, ll w)
    {
        a[u][u] = (a[u][u] + w) % mod;
        a[u][v] = (a[u][v] - w + mod) % mod;
    };
    for (auto [u, v, w] : e)
    {
        assert(u >= 1 && u <= n && v >= 1 && v <= n);
        if (u == v) continue;
        w = (w % mod + mod) % mod;
        add(u - 1, v - 1, w);
        if (!directed) add(v - 1, u - 1, w);
    }
    vector<vector<ll>> b;
    for (int i = 0; i < n; i++)
    {
        if (i == root - 1) continue;
        b.push_back({});
        for (int j = 0; j < n; j++)
        {
            if (j != root - 1) b.back().push_back(a[i][j]);
        }
    }
    return det_mod(b, mod);
}
