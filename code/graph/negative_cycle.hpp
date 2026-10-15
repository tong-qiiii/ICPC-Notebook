#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

template <class Edge>
bool neg_cycle(const vector<vector<Edge>>& g)
{
    int n = (int)g.size() - 1;
    vector<__int128> dis(n + 1, 0);
    vector<int> cnt(n + 1, 0);
    vector<char> in(n + 1, true);
    queue<int> q;

    for (int i = 1; i <= n; ++i)
        q.push(i);

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        in[u] = false;

        for (auto [v, w] : g[u])
        {
            if (dis[v] > dis[u] + w)
            {
                dis[v] = dis[u] + w;
                cnt[v] = cnt[u] + 1;

                if (cnt[v] >= n)
                    return true;

                if (!in[v])
                {
                    q.push(v);
                    in[v] = true;
                }
            }
        }
    }

    return false;
}
