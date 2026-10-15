#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct LCAHLD
{
    int n = 0;
    vector<vector<int>> g;
    vector<int> fa, dep, siz, son, top;
    LCAHLD(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        g.assign(n + 1, {});
        fa.assign(n + 1, 0);
        dep.assign(n + 1, 0);
        siz.assign(n + 1, 0);
        son.assign(n + 1, 0);
        top.assign(n + 1, 0);
    }

    void add(int u, int v)
    {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void build(int root = 1)
    {
        if (!n)
        {
            return;
        }
        fill(son.begin(), son.end(), 0);
        dfs1(root, 0);
        dfs2(root, root);
    }

    int lca(int u, int v) const
    {
        while (top[u] != top[v])
        {
            if (dep[top[u]] < dep[top[v]])
            {
                swap(u, v);
            }
            u = fa[top[u]];
        }
        return dep[u] < dep[v] ? u : v;
    }
private:
    void dfs1(int u, int p)
    {
        fa[u] = p;
        dep[u] = dep[p] + 1;
        siz[u] = 1;
        for (int v : g[u])
        {
            if (v == p)
            {
                continue;
            }
            dfs1(v, u);
            siz[u] += siz[v];
            if (siz[v] > siz[son[u]])
            {
                son[u] = v;
            }
        }
    }

    void dfs2(int u, int tp)
    {
        top[u] = tp;
        if (son[u])
        {
            dfs2(son[u], tp);
        }
        for (int v : g[u])
        {
            if (v == fa[u] || v == son[u])
            {
                continue;
            }
            dfs2(v, v);
        }
    }
};
