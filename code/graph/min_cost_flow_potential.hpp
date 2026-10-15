#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif
#ifndef pll
#define pll pair<long long, long long>
#endif

struct MCMFPotential
{
    struct Edge
    {
        int to, rev;
        ll cap, cost;
    };

    int n;
    vector<vector<Edge>> g;
    vector<ll> dis, h;
    vector<int> pre_v, pre_e;
    vector<char> in;

    MCMFPotential(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        g.assign(n + 1, {});
        dis.resize(n + 1);
        h.resize(n + 1);
        pre_v.resize(n + 1);
        pre_e.resize(n + 1);
        in.resize(n + 1);
    }

    void add(int u, int v, ll cap, ll cost)
    {
        int iu = g[u].size();
        int iv = g[v].size();

        if (u == v)
            ++iv;

        g[u].push_back({v, iv, cap, cost});
        g[v].push_back({u, iu, 0, -cost});
    }

    void init_potential(int s)
    {
        const ll INF = 1LL << 60;

        fill(h.begin(), h.end(), INF);
        fill(in.begin(), in.end(), false);

        queue<int> q;
        h[s] = 0;
        in[s] = true;
        q.push(s);

        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            in[u] = false;

            for (const auto& e : g[u])
            {
                if (e.cap > 0 && h[e.to] > h[u] + e.cost)
                {
                    h[e.to] = h[u] + e.cost;

                    if (!in[e.to])
                    {
                        in[e.to] = true;
                        q.push(e.to);
                    }
                }
            }
        }

        for (int i = 1; i <= n; ++i)
            if (h[i] == INF)
                h[i] = 0;
    }

    bool dijkstra(int s, int t)
    {
        const ll INF = 1LL << 60;

        fill(dis.begin(), dis.end(), INF);

        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;

        dis[s] = 0;
        pq.push({0, s});

        while (!pq.empty())
        {
            auto [d, u] = pq.top();
            pq.pop();

            if (d != dis[u])
                continue;

            for (int i = 0; i < (int)g[u].size(); ++i)
            {
                Edge& e = g[u][i];

                if (e.cap == 0)
                    continue;

                ll nd = d + e.cost + h[u] - h[e.to];

                if (dis[e.to] > nd)
                {
                    dis[e.to] = nd;
                    pre_v[e.to] = u;
                    pre_e[e.to] = i;
                    pq.push({nd, e.to});
                }
            }
        }

        if (dis[t] == INF)
            return false;

        for (int i = 1; i <= n; ++i)
            if (dis[i] != INF)
                h[i] += dis[i];

        return true;
    }

    pll flow(int s, int t, ll lim = numeric_limits<ll>::max())
    {
        if (s == t)
            return {0, 0};

        ll maxflow = 0;
        ll mincost = 0;

        init_potential(s);

        while (maxflow < lim && dijkstra(s, t))
        {
            ll f = lim - maxflow;

            for (int v = t; v != s; v = pre_v[v])
                f = min(f, g[pre_v[v]][pre_e[v]].cap);

            for (int v = t; v != s; v = pre_v[v])
            {
                Edge& e = g[pre_v[v]][pre_e[v]];

                e.cap -= f;
                g[e.to][e.rev].cap += f;
            }

            maxflow += f;
            mincost += f * (h[t] - h[s]);
        }

        return {maxflow, mincost};
    }
};
