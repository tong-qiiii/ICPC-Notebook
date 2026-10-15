#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif
#include "dinic.hpp"

struct GomoryHu
{
    struct Edge
    {
        int u, v;
        ll w;
    };

    int n;
    vector<Edge> e;
    vector<int> fa;
    vector<ll> cut;
    vector<vector<pair<int, ll>>> tree;

    GomoryHu(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        e.clear();
        fa.clear();
        cut.clear();
        tree.clear();
    }

    void add(int u, int v, ll w)
    {
        if (u != v)
            e.push_back({u, v, w});
    }

    pair<ll, vector<char>>
    solve_flow(int s, int t) const
    {
        Dinic dinic(n);

        for (auto [u, v, w] : e)
            dinic.add_undirected(u, v, w);

        ll flow = dinic.maxflow(s, t);

        return {flow, dinic.reachable(s)};
    }

    void build()
    {
        fa.assign(n + 1, 1);
        cut.assign(n + 1, 0);
        tree.assign(n + 1, {});

        for (int s = 2; s <= n; ++s)
        {
            int t = fa[s];

            auto [flow, side] = solve_flow(s, t);

            for (int i = s + 1; i <= n; ++i)
            {
                if (fa[i] == t && side[i])
                    fa[i] = s;
            }

            if (side[fa[t]])
            {
                fa[s] = fa[t];
                fa[t] = s;

                cut[s] = cut[t];
                cut[t] = flow;
            }
            else
            {
                cut[s] = flow;
            }
        }

        for (int i = 2; i <= n; ++i)
        {
            tree[i].push_back({fa[i], cut[i]});
            tree[fa[i]].push_back({i, cut[i]});
        }
    }

    ll query(int s, int t) const

    {
        const ll INF = numeric_limits<ll>::max();

        vector<ll> mn(n + 1, -1);
        queue<int> q;

        mn[s] = INF;
        q.push(s);

        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            if (u == t)
                return mn[u];

            for (auto [v, w] : tree[u])
            {
                if (mn[v] == -1)
                {
                    mn[v] = min(mn[u], w);
                    q.push(v);
                }
            }
        }

        return -1;
    }
};
