#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct KRT
{
    struct Edge
    {
        int u, v;
        ll w;
        bool operator<(const Edge &o) const
        {
            return w < o.w;
        }
    };

    int n = 0, tot = 0, lg = 0, timer = 0;
    vector<Edge> edges;
    vector<vector<int>> g, fa;
    vector<ll> val;
    vector<int> dep, siz, dfn, bel, roots;
    KRT(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        tot = n;
        timer = 0;
        lg = 1;
        while ((1LL << lg) <= 2LL * n)
        {
            lg++;
        }
        edges.clear();
        roots.clear();
    }

    void add(int u, int v, ll w)
    {
        edges.push_back({u, v, w});
    }

    void build()
    {
        sort(edges.begin(), edges.end());
        int N = 2 * n + 5;
        tot = n;
        timer = 0;
        g.assign(N, {});
        fa.assign(lg, vector<int>(N));
        val.assign(N, 0);
        dep.assign(N, 0);
        siz.assign(N, 0);
        dfn.assign(N, 0);
        bel.assign(N, 0);
        roots.clear();
        dsu.resize(N);
        iota(dsu.begin(), dsu.end(), 0);
        for (auto [u, v, w] : edges)
        {
            int fu = find(u);
            int fv = find(v);
            if (fu == fv)
            {
                continue;
            }
            int p = ++tot;
            val[p] = w;
            g[p].push_back(fu);
            g[p].push_back(fv);
            dsu[fu] = dsu[fv] = p;
            dsu[p] = p;
        }
        for (int u = 1; u <= tot; u++)
        {
            if (dsu[u] == u)
            {
                roots.push_back(u);
                dep[u] = 1;
                dfs(u, u);
            }
        }
        for (int k = 1; k < lg; k++)
        {
            for (int u = 1; u <= tot; u++)
            {
                fa[k][u] = fa[k - 1][fa[k - 1][u]];
            }
        }
    }

    bool connected(int u, int v) const
    {
        return bel[u] == bel[v];
    }

    int lca(int u, int v) const
    {
        if (!connected(u, v))
        {
            return 0;
        }
        if (dep[u] < dep[v])
        {
            swap(u, v);
        }
        int d = dep[u] - dep[v];
        for (int k = 0; k < lg; k++)
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
        for (int k = lg - 1; k >= 0; k--)
        {
            if (fa[k][u] != fa[k][v])
            {
                u = fa[k][u];
                v = fa[k][v];
            }
        }
        return fa[0][u];
    }

    ll bottleneck(int u, int v) const
    {
        return val[lca(u, v)];
    }

    int climb(int u, ll x) const
    {
        for (int k = lg - 1; k >= 0; k--)
        {
            int v = fa[k][u];
            if (v && val[v] <= x)
            {
                u = v;
            }
        }
        return u;
    }
private:
    vector<int> dsu;
    int find(int x)
    {
        return dsu[x] == x ? x : dsu[x] = find(dsu[x]);
    }

    void dfs(int u, int root)
    {
        bel[u] = root;
        dfn[u] = timer + 1;
        if (u <= n)
        {
            siz[u] = 1;
            timer++;
            return;
        }
        for (int v : g[u])
        {
            fa[0][v] = u;
            dep[v] = dep[u] + 1;
            dfs(v, root);
            siz[u] += siz[v];
        }
    }
};
