#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct SCC
{
    int n, timer, cnt;
    vector<vector<int>> g;
    vector<int> dfn, low, bel, siz, stk;
    vector<char> in;

    SCC(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        timer = cnt = 0;
        g.assign(n + 1, {});
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        bel.assign(n + 1, 0);
        siz.clear();
        stk.clear();
        in.assign(n + 1, false);
    }

    void add(int u, int v)
    {
        g[u].push_back(v);
    }

    void dfs(int u)
    {
        dfn[u] = low[u] = ++timer;
        stk.push_back(u);
        in[u] = true;

        for (int v : g[u])
        {
            if (!dfn[v])
            {
                dfs(v);
                low[u] = min(low[u], low[v]);
            }
            else if (in[v])
            {
                // back edge: use dfn[v], not low[v]
                low[u] = min(low[u], dfn[v]);
            }
        }

        if (low[u] == dfn[u])
        {
            ++cnt;
            while (true)
            {
                int x = stk.back();
                stk.pop_back();
                in[x] = false;
                bel[x] = cnt;
                if (x == u) break;
            }
        }
    }

    void work()
    {
        timer = cnt = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(bel.begin(), bel.end(), 0);
        fill(in.begin(), in.end(), false);
        stk.clear();

        for (int i = 1; i <= n; ++i)
            if (!dfn[i])
                dfs(i);

        siz.assign(cnt + 1, 0);
        for (int u = 1; u <= n; ++u)
            ++siz[bel[u]];
    }

    vector<vector<int>> dag() const

    {
        vector<vector<int>> nodes(cnt + 1), res(cnt + 1);
        for (int u = 1; u <= n; ++u)
            nodes[bel[u]].push_back(u);

        vector<int> vis(cnt + 1, 0);

        for (int c = 1; c <= cnt; ++c)
        {
            for (int u : nodes[c])
            {
                for (int v : g[u])
                {
                    int d = bel[v];
                    if (c != d && vis[d] != c)
                    {
                        vis[d] = c;
                        res[c].push_back(d);
                    }
                }
            }
        }

        return res;
    }
};
