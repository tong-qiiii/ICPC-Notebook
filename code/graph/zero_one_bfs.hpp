#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline vector<ll> zeroOneBFS(const vector<vector<pair<int, int>>>& g, int s)
{
    int n = (int)g.size() - 1;
    const ll INF = 1LL << 60;

    vector<ll> dis(n + 1, INF);
    deque<int> q;

    dis[s] = 0;
    q.push_front(s);

    while (!q.empty())
    {
        int u = q.front();
        q.pop_front();

        for (auto [v, w] : g[u])
        {
            if (dis[v] <= dis[u] + w) continue;

            dis[v] = dis[u] + w;

            if (w == 0) q.push_front(v);
            else q.push_back(v);
        }
    }

    return dis;
}
