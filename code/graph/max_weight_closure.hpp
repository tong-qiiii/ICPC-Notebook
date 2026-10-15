#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif
#include "dinic.hpp"

struct MaxWeightClosure
{
    int n;
    vector<ll> w;
    vector<pair<int, int>> dep;

    MaxWeightClosure(int n_) : n(n_), w(n + 1)
    {
    }

    void addWeight(int u, ll x)
    {
        w[u] += x;
    }

    void addDependency(int u, int v)
    {
        dep.push_back({u, v});
    }

    pair<ll, vector<int>> work()
    {
        const ll INF = 1LL << 62;

        int S = 0, T = n + 1;
        Dinic dinic(n + 1);

        ll sum = 0;

        for (int u = 1; u <= n; ++u)
        {
            if (w[u] > 0)
            {
                assert(w[u] < INF - sum);
                dinic.add(S, u, w[u]);
                sum += w[u];
            }
            else if (w[u] < 0)
            {
                assert(w[u] != numeric_limits<ll>::min());
                dinic.add(u, T, -w[u]);
            }
        }

        for (auto [u, v] : dep) dinic.add(u, v, INF);

        ll ans = sum - dinic.maxflow(S, T);

        vector<char> vis(n + 2);
        queue<int> q;

        vis[S] = 1;
        q.push(S);

        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            for (auto& e : dinic.g[u])
            {
                if (e.cap && !vis[e.to])
                {
                    vis[e.to] = 1;
                    q.push(e.to);
                }
            }
        }

        vector<int> chosen;

        for (int u = 1; u <= n; ++u)
            if (vis[u])
                chosen.push_back(u);

        return {ans, chosen};
    }
};
