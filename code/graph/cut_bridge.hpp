#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct CutBridge
{
    struct Edge
    {
        int to, id;
    };

    int n, edge_cnt, timer;
    vector<vector<Edge>> g;
    vector<int> dfn, low;
    vector<char> is_cut;
    vector<pair<int, int>> bridges;

    CutBridge(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        edge_cnt = timer = 0;
        g.assign(n + 1, {});
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        is_cut.assign(n + 1, false);
        bridges.clear();
    }

    void add(int u, int v)
    {
        g[u].push_back({v, edge_cnt});
        g[v].push_back({u, edge_cnt});
        ++edge_cnt;
    }

    void dfs(int u, int pe)
    {
        dfn[u] = low[u] = ++timer;
        int child = 0;

        for (auto [v, id] : g[u])
        {
            if (!dfn[v])
            {
                ++child;
                dfs(v, id);
                low[u] = min(low[u], low[v]);

                if (pe != -1 && low[v] >= dfn[u])
                    is_cut[u] = true;

                if (low[v] > dfn[u])
                    bridges.push_back({u, v});
            }
            else if (id != pe)
            {
                low[u] = min(low[u], dfn[v]);
            }
        }

        if (pe == -1 && child >= 2)
            is_cut[u] = true;
    }

    void work()
    {
        timer = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(is_cut.begin(), is_cut.end(), false);
        bridges.clear();

        for (int i = 1; i <= n; ++i)
            if (!dfn[i])
                dfs(i, -1);
    }
};
