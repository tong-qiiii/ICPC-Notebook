#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline pair<int, ll> nim_move(const vector<ll> &a)
{
    ll s = 0;
    for (ll x : a) s ^= x;
    if (!s) return {-1, 0};
    for (int i = 0; i < (int)a.size(); i++)
    {
        if ((a[i] ^ s) < a[i]) return {i, a[i] ^ s};
    }
    return {-1, 0};
}

inline vector<int> sg_subtract(int n, const vector<int> &moves)
{
    vector<int> sg(n + 1), vis(moves.size() + 1, -1);
    for (int i = 1; i <= n; i++)
    {
        for (int x : moves)
        {
            assert(x > 0);
            if (x <= i) vis[sg[i - x]] = i;
        }
        while (vis[sg[i]] == i) sg[i]++;
    }
    return sg;
}

// 1-index，状态图必须是 DAG；终止状态 SG = 0。
inline vector<int> sg_dag(const vector<vector<int>> &g)
{
    int n = (int)g.size() - 1;
    vector<int> sg(n + 1), color(n + 1);
    auto dfs = [&](auto &&self, int u) -> void
    {
        assert(color[u] != 1);
        if (color[u] == 2) return;
        color[u] = 1;
        for (int v : g[u]) self(self, v);
        vector<char> vis(g[u].size() + 1);
        for (int v : g[u])
        {
            if (sg[v] < (int)vis.size()) vis[sg[v]] = true;
        }
        while (vis[sg[u]]) sg[u]++;
        color[u] = 2;
    };
    for (int i = 1; i <= n; i++) dfs(dfs, i);
    return sg;
}
