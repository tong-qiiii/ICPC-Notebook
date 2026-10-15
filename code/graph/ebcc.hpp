#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct EBCC
{
    struct Edge
    {
        int to, id;
    };

    int n, edge_cnt, timer, cnt;
    vector<vector<Edge>> g;
    vector<int> dfn, low, bel, siz;
    vector<char> is_bridge;

    EBCC(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        edge_cnt = timer = cnt = 0;
        g.assign(n + 1, {});
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        bel.assign(n + 1, 0);
        siz.clear();
        is_bridge.clear();
    }

    void add(int u, int v)
    {
        g[u].push_back({v, edge_cnt});
        g[v].push_back({u, edge_cnt});
        ++edge_cnt;
    }

    void dfs_bridge(int u, int pe)
    {
        dfn[u] = low[u] = ++timer;

        for (auto [v, id] : g[u])
        {
            if (!dfn[v])
            {
                dfs_bridge(v, id);
                low[u] = min(low[u], low[v]);

                if (low[v] > dfn[u])
                    is_bridge[id] = true;
            }
            else if (id != pe)
            {
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

    void dfs_comp(int u)
    {
        bel[u] = cnt;
        ++siz[cnt];

        for (auto [v, id] : g[u])
        {
            if (!bel[v] && !is_bridge[id])
                dfs_comp(v);
        }
    }

    void work()
    {
        timer = cnt = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(bel.begin(), bel.end(), 0);
        is_bridge.assign(edge_cnt, false);

        for (int i = 1; i <= n; ++i)
            if (!dfn[i])
                dfs_bridge(i, -1);

        siz.assign(n + 1, 0);

        for (int i = 1; i <= n; ++i)
        {
            if (!bel[i])
            {
                ++cnt;
                dfs_comp(i);
            }
        }

        siz.resize(cnt + 1);
    }

    vector<vector<int>> bridge_tree() const

    {
        vector<vector<int>> tree(cnt + 1);

        for (int u = 1; u <= n; ++u)
        {
            for (auto [v, id] : g[u])
            {
                if (bel[u] != bel[v])
                    tree[bel[u]].push_back(bel[v]);
            }
        }

        return tree;
    }
};
