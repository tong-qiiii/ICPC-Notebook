#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct VBCC
{
    struct Edge
    {
        int to, id;
    };

    int n, edge_cnt, timer, cnt;
    vector<vector<Edge>> g;
    vector<int> dfn, low, stk;
    vector<vector<int>> bcc;
    vector<char> is_cut;

    VBCC(int n = 0)
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
        stk.clear();
        bcc.assign(1, {});
        is_cut.assign(n + 1, false);
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
        stk.push_back(u);
        int child = 0;

        for (auto [v, id] : g[u])
        {
            if (!dfn[v])
            {
                ++child;
                dfs(v, id);
                low[u] = min(low[u], low[v]);

                if (low[v] >= dfn[u])
                {
                    if (pe != -1)
                        is_cut[u] = true;

                    ++cnt;
                    bcc.push_back({u});

                    while (true)
                    {
                        int x = stk.back();
                        stk.pop_back();
                        bcc[cnt].push_back(x);
                        if (x == v) break;
                    }
                }
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
        timer = cnt = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(is_cut.begin(), is_cut.end(), false);
        stk.clear();
        bcc.assign(1, {});

        for (int i = 1; i <= n; ++i)
        {
            if (dfn[i]) continue;

            if (g[i].empty())
            {
                dfn[i] = low[i] = ++timer;
                ++cnt;
                bcc.push_back({i});
                continue;
            }

            dfs(i, -1);

            // DFS 根不会被子块弹出
            if (!stk.empty() && stk.back() == i)
                stk.pop_back();
        }
    }

    vector<vector<int>> block_tree() const

    {
        vector<vector<int>> tree(n + cnt + 1);

        for (int id = 1; id <= cnt; ++id)
        {
            int square = n + id;

            for (int u : bcc[id])
            {
                tree[u].push_back(square);
                tree[square].push_back(u);
            }
        }

        return tree;
    }
};
