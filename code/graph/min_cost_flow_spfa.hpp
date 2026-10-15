#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif
#ifndef pll
#define pll pair<long long, long long>
#endif

struct MCMF
{
    struct Edge
    {
        int to, rev;
        ll cap, cost;
    };

    int n;
    vector<vector<Edge>> g;
    vector<ll> dis;
    vector<int> pre_v, pre_e;
    vector<char> in;

    MCMF(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        g.assign(n + 1, {});
        dis.resize(n + 1);
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

    bool spfa(int s, int t)
    {
        const ll INF = 1LL << 60;

        fill(dis.begin(), dis.end(), INF);
        fill(in.begin(), in.end(), false);

        queue<int> q;
        dis[s] = 0;
        in[s] = true;
        q.push(s);

        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            in[u] = false;

            for (int i = 0; i < (int)g[u].size(); ++i)
            {
                Edge& e = g[u][i];

                if (e.cap > 0 && dis[e.to] > dis[u] + e.cost)
                {
                    dis[e.to] = dis[u] + e.cost;
                    pre_v[e.to] = u;
                    pre_e[e.to] = i;

                    if (!in[e.to])
                    {
                        in[e.to] = true;
                        q.push(e.to);
                    }
                }
            }
        }

        return dis[t] != INF;
    }

    pll flow(int s, int t, ll lim = numeric_limits<ll>::max())
    {
        if (s == t)
            return {0, 0};

        ll maxflow = 0;
        ll mincost = 0;

        while (maxflow < lim && spfa(s, t))
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
            mincost += f * dis[t];
        }

        return {maxflow, mincost};
    }
};
