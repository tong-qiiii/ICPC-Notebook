#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct DiffCons
{
    struct Edge
    {
        int to;
        ll w;
    };

    int n;
    vector<vector<Edge>> g;

    DiffCons(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        g.assign(n + 1, {});
    }

    // x_v - x_u <= w  <=>  x_v <= x_u + w
    void leq(int u, int v, ll w)
    {
        g[u].push_back({v, w});
    }

    pair<bool, vector<ll>> solve() const

    {
        vector<ll> dis(n + 1, 0);
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
                        return {false, {}};

                    if (!in[v])
                    {
                        q.push(v);
                        in[v] = true;
                    }
                }
            }
        }

        return {true, dis};
    }
};
