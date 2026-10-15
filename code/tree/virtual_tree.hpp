#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct VTree
{
    int n = 0, lg = 0, tms = 0;
    vector<vector<int>> g, vt, fa;
    vector<int> dep, siz, dfn, nodes;
    VTree(int n = 0)
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
        tms = 0;
        g.assign(n + 1, {});
        vt.assign(n + 1, {});
        fa.assign(lg, vector<int>(n + 1, 0));
        dep.assign(n + 1, 0);
        siz.assign(n + 1, 0);
        dfn.assign(n + 1, 0);
        nodes.clear();
    }

    void add(int u, int v)
    {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void prep(int root = 1)
    {
        clear_vt();
        tms = 0;
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

    int lca(int u, int v) const
    {
        if (dep[u] < dep[v])
        {
            swap(u, v);
        }
        int d = dep[u] - dep[v];
        for (int k = 0; k < lg; ++k)
        {
            if (d >> k & 1)
            {
                u = fa[k][u];
            }
        }
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

    int build(vector<int> key)
    {
        clear_vt();
        if (key.empty())
        {
            return 0;
        }
        auto cmp = [&](int u, int v)
        {
            return dfn[u] < dfn[v];
        };
        sort(key.begin(), key.end(), cmp);
        key.erase(unique(key.begin(), key.end()), key.end());
        int k = key.size();
        for (int i = 0; i + 1 < k; ++i)
        {
            key.push_back(lca(key[i], key[i + 1]));
        }
        sort(key.begin(), key.end(), cmp);
        key.erase(unique(key.begin(), key.end()), key.end());
        nodes = key;
        vector<int> stk;
        for (int u : nodes)
        {
            while (!stk.empty() && !is_ancestor(stk.back(), u))
            {
                stk.pop_back();
            }
            if (!stk.empty())
            {
                vt[stk.back()].push_back(u);
            }
            stk.push_back(u);
        }
        return nodes[0];
    }
private:
    bool is_ancestor(int u, int v) const
    {
        return dfn[u] <= dfn[v] && dfn[v] < dfn[u] + siz[u];
    }

    void clear_vt()
    {
        for (int u : nodes)
        {
            vt[u].clear();
        }
        nodes.clear();
    }

    void dfs(int u, int p)
    {
        dfn[u] = ++tms;
        fa[0][u] = p;
        dep[u] = dep[p] + 1;
        siz[u] = 1;
        for (int v : g[u])
        {
            if (v == p)
            {
                continue;
            }
            dfs(v, u);
            siz[u] += siz[v];
        }
    }
};
