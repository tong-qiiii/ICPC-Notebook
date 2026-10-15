#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct LCA
{
    int n = 0, lg = 0;
    vector<vector<int>> g, fa;
    vector<int> dep;
    LCA(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        lg = 1;
        while ((1LL << lg) <= n)
        {
            ++lg;
        }
        g.assign(n + 1, {});
        fa.assign(lg, vector<int>(n + 1));
        dep.assign(n + 1, 0);
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
        dfs(root, 0);
        for (int k = 1; k < lg; ++k)
        {
            for (int u = 1; u <= n; ++u)
            {
                fa[k][u] = fa[k - 1][fa[k - 1][u]];
            }
        }
    }

    int kth_up(int u, int k) const
    {
        if (k < 0 || k >= dep[u])
        {
            return 0;
        }
        for (int i = 0; i < lg; ++i)
        {
            if (k >> i & 1)
            {
                u = fa[i][u];
            }
        }
        return u;
    }

    int lca(int u, int v) const
    {
        if (dep[u] < dep[v])
        {
            swap(u, v);
        }
        u = kth_up(u, dep[u] - dep[v]);
        if (u == v)
        {
            return u;
        }
        for (int k = lg - 1; k >= 0; --k)
        {
            if (fa[k][u] != fa[k][v])
            {
                u = fa[k][u];
                v = fa[k][v];
            }
        }
        return fa[0][u];
    }
private:
    void dfs(int u, int p)
    {
        fa[0][u] = p;
        dep[u] = dep[p] + 1;
        for (int v : g[u])
        {
            if (v != p)
            {
                dfs(v, u);
            }
        }
    }
};
