#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct Dinic
{
    struct Edge
    {
        int to, rev;
        ll cap;
    };

    int n;
    vector<vector<Edge>> g;
    vector<int> level, cur;

    Dinic(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        g.assign(n + 1, {});
        level.assign(n + 1, 0);
        cur.assign(n + 1, 0);
    }

    int add_node()
    {
        ++n;
        g.emplace_back();
        level.push_back(0);
        cur.push_back(0);
        return n;
    }

    int add(int u, int v, ll cap)
    {
        int iu = g[u].size();
        int iv = g[v].size();

        if (u == v)
            ++iv;

        g[u].push_back({v, iv, cap});
        g[v].push_back({u, iu, 0});
        return iu;
    }

    void add_undirected(int u, int v, ll cap)
    {
        int iu = g[u].size();
        int iv = g[v].size();

        if (u == v)
            ++iv;

        g[u].push_back({v, iv, cap});
        g[v].push_back({u, iu, cap});
    }

    bool bfs(int s, int t)
    {
        fill(level.begin(), level.end(), -1);

        queue<int> q;
        level[s] = 0;
        q.push(s);

        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            for (const auto& e : g[u])
            {
                if (e.cap > 0 && level[e.to] == -1)
                {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }

        return level[t] != -1;
    }

    ll dfs(int u, int t, ll flow)
    {
        if (u == t)
            return flow;

        ll used = 0;

        for (int& i = cur[u]; i < (int)g[u].size(); ++i)
        {
            Edge& e = g[u][i];

            if (e.cap > 0 && level[e.to] == level[u] + 1)
            {
                ll pushed = dfs(e.to, t, min(flow - used, e.cap));

                if (pushed)
                {
                    e.cap -= pushed;
                    g[e.to][e.rev].cap += pushed;
                    used += pushed;
                }

                if (used == flow)
                    return used;
            }
        }

        level[u] = -1;
        return used;
    }

    ll maxflow(int s, int t, ll lim = numeric_limits<ll>::max())
    {
        if (s == t)
            return 0;

        ll flow = 0;

        while (flow < lim && bfs(s, t))
        {
            fill(cur.begin(), cur.end(), 0);

            while (flow < lim)
            {
                ll pushed = dfs(s, t, lim - flow);

                if (!pushed)
                    break;

                flow += pushed;
            }
        }

        return flow;
    }

    void disable(int u, int idx)
    {
        int v = g[u][idx].to, rev = g[u][idx].rev;
        g[u][idx].cap = g[v][rev].cap = 0;
    }

    vector<char> reachable(int s) const

    {
        vector<char> vis(n + 1, false);
        queue<int> q;

        vis[s] = true;
        q.push(s);

        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            for (const auto& e : g[u])
            {
                if (e.cap > 0 && !vis[e.to])
                {
                    vis[e.to] = true;
                    q.push(e.to);
                }
            }
        }

        return vis;
    }
};
