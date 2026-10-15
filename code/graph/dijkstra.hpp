#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

template <class Edge>
vector<ll> dijkstra(const vector<vector<Edge>>& g, int s)
{
    int n = (int)g.size() - 1;
    assert(1 <= s && s <= n);
    const ll INF = 1LL << 62;

    vector<ll> dis(n + 1, INF);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;

    dis[s] = 0;
    pq.push({0, s});

    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();

        if (d != dis[u])
            continue;

        for (auto [v, w] : g[u])
        {
            if (w < INF - d && dis[v] > d + w)
            {
                dis[v] = d + w;
                pq.push({dis[v], v});
            }
        }
    }

    return dis;
}
