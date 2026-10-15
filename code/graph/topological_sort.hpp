#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline vector<int> topo(const vector<vector<int>>& g)
{
    int n = (int)g.size() - 1;
    vector<int> deg(n + 1);
    for (int u = 1; u <= n; ++u)
        for (int v : g[u]) ++deg[v];
    vector<int> order;
    queue<int> q;

    for (int i = 1; i <= n; ++i)
        if (deg[i] == 0)
            q.push(i);

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        order.push_back(u);

        for (int v : g[u])
            if (--deg[v] == 0)
                q.push(v);
    }

    if ((int)order.size() != n)
        return {};

    return order;
}
