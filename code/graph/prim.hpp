#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline pair<bool, ll> prim(const vector<vector<pair<int, ll>>>& g, int s = 1)
{
    int n = (int)g.size() - 1;
    if (!n) return {true, 0};
    assert(1 <= s && s <= n);
    const ll INF = 1LL << 62;

    vector<ll> best(n + 1, INF);
    vector<char> vis(n + 1, false);

    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;

    best[s] = 0;
    pq.push({0, s});

    int cnt = 0;
    ll ans = 0;

    while (!pq.empty())
    {
        auto [w, u] = pq.top();
        pq.pop();

        if (vis[u])
            continue;

        vis[u] = true;
        ++cnt;
        ans += w;

        for (auto [v, cost] : g[u])
        {
            if (!vis[v] && best[v] > cost)
            {
                best[v] = cost;
                pq.push({cost, v});
            }
        }
    }

    return {cnt == n, ans};
}
