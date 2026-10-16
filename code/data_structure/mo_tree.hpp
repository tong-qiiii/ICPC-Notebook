#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct Query
{
    int l, r, id, extra;
};

template <class Add, class Del>
vector<ll> mo_tree(const vector<vector<int>> &g,
                   const vector<pair<int, int>> &paths, ll &now,
                   Add add_node, Del del_node)
{
    int n = (int)g.size() - 1;
    int m = (int)paths.size();
    if (!n)
    {
        return {};
    }
    int LOG = 1;
    while ((1LL << LOG) <= n)
    {
        ++LOG;
    }

    vector<vector<int>> fa(LOG, vector<int>(n + 1));
    vector<int> dep(n + 1);
    vector<int> in(n + 1), out(n + 1);
    vector<int> euler(2 * n + 1);

    int timer = 0;

    auto dfs = [&](auto &&self, int u, int f) -> void
    {
        dep[u] = dep[f] + 1;
        fa[0][u] = f;

        for (int k = 1; k < LOG; ++k)
        {
            fa[k][u] = fa[k - 1][fa[k - 1][u]];
        }

        in[u] = ++timer;
        euler[timer] = u;

        for (int v : g[u])
        {
            if (v == f)
            {
                continue;
            }

            self(self, v, u);
        }

        out[u] = ++timer;
        euler[timer] = u;
    };

    dfs(dfs, 1, 0);

    auto lca = [&](int u, int v)
    {
        if (dep[u] < dep[v])
        {
            swap(u, v);
        }

        int d = dep[u] - dep[v];

        for (int k = LOG - 1; k >= 0; --k)
        {
            if ((d >> k) & 1)
            {
                u = fa[k][u];
            }
        }

        if (u == v)
        {
            return u;
        }

        for (int k = LOG - 1; k >= 0; --k)
        {
            if (fa[k][u] != fa[k][v])
            {
                u = fa[k][u];
                v = fa[k][v];
            }
        }

        return fa[0][u];
    };

    vector<Query> q(m);

    for (int id = 0; id < m; ++id)
    {
        int u, v;
        tie(u, v) = paths[id];

        if (in[u] > in[v])
        {
            swap(u, v);
        }

        int p = lca(u, v);

        if (p == u)
        {
            q[id] = {in[u], in[v], id, 0};
        }
        else
        {
            q[id] = {out[u], in[v], id, p};
        }
    }

    int len = 2 * n;
    int B = max(1, len / max(1, (int)sqrt(m)));

    sort(q.begin(), q.end(),
         [&](const Query &A, const Query &B_)
         {
             int ba = (A.l - 1) / B;
             int bb = (B_.l - 1) / B;

             if (ba != bb)
             {
                 return ba < bb;
             }

             // 奇偶块优化
             if (ba & 1)
             {
                 return A.r > B_.r;
             }

             return A.r < B_.r;
         });

    vector<int> vis(n + 1);

    auto flip = [&](int pos)
    {
        int u = euler[pos];

        if (vis[u])
        {
            del_node(u);
        }
        else
        {
            add_node(u);
        }

        vis[u] ^= 1;
    };

    vector<ll> ans(m);

    int L = 1, R = 0;

    for (auto [l, r, id, extra] : q)
    {
        while (L > l)
        {
            flip(--L);
        }
        while (R < r)
        {
            flip(++R);
        }
        while (L < l)
        {
            flip(L++);
        }
        while (R > r)
        {
            flip(R--);
        }

        if (extra)
        {
            add_node(extra);
            ans[id] = now;
            del_node(extra);
        }
        else
        {
            ans[id] = now;
        }
    }

    return ans;
}
