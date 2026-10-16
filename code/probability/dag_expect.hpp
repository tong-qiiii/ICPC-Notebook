#pragma once
#include <bits/stdc++.h>
using namespace std;

// 去掉自环后必须是 DAG；g[u] 为空表示已经终止。
template<class T>
vector<T> dag_expect(const vector<vector<pair<int, T>>> &g, const vector<T> &cost)
{
    int n = (int)g.size() - 1;
    assert(n >= 0 && (int)cost.size() == n + 1);
    vector<int> deg(n + 1), ord;
    for (int u = 1; u <= n; u++)
    {
        for (auto [v, p] : g[u])
        {
            if (u != v && p != T(0)) deg[v]++;
        }
    }
    queue<int> q;
    for (int u = 1; u <= n; u++) if (!deg[u]) q.push(u);
    while (!q.empty())
    {
        int u = q.front(); q.pop();
        ord.push_back(u);
        for (auto [v, p] : g[u])
        {
            if (u != v && p != T(0) && !--deg[v]) q.push(v);
        }
    }
    if ((int)ord.size() != n) throw domain_error("non-self cycle");
    vector<T> dp(n + 1);
    reverse(ord.begin(), ord.end());
    for (int u : ord)
    {
        if (g[u].empty()) continue;
        T den = 0, val = cost[u];
        for (auto [v, p] : g[u])
        {
            if (u != v) den = den + p, val = val + p * dp[v];
        }
        if (den == T(0)) throw domain_error("zero exit probability");
        dp[u] = val / den;
    }
    return dp;
}
