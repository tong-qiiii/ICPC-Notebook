#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct JumpGraph
{
    int n, LOG, tot;

    vector<vector<int>> tree;
    vector<int> dep;

    vector<vector<int>> fa;
    vector<vector<int>> out_id, in_id;

    vector<vector<int>> g;
    vector<int> indeg;

    JumpGraph(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        LOG = 0;
        tot = n;

        tree.assign(n + 1, {});
        dep.clear();
        fa.clear();
        out_id.clear();
        in_id.clear();

        g.clear();
        indeg.clear();
    }

    void add_tree(int u, int v)
    {
        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    void build(int root = 1)
    {
        LOG = 1;
        while ((1LL << LOG) <= n)
            ++LOG;

        dep.assign(n + 1, 0);
        fa.assign(LOG, vector<int>(n + 1, 0));

        assert(1 <= root && root <= n);
        auto dfs = [&](auto&& dfs, int u, int pre) -> void
        {
            for (int v : tree[u])
            {
                if (v == pre) continue;
                fa[0][v] = u;
                dep[v] = dep[u] + 1;
                dfs(dfs, v, u);
            }
        };
        dep[root] = 1;
        dfs(dfs, root, 0);

        for (int k = 1; k < LOG; ++k)
            for (int u = 1; u <= n; ++u)
                fa[k][u] = fa[k - 1][fa[k - 1][u]];

        tot = n;
        g.assign(n + 1, {});
        indeg.assign(n + 1, 0);

        out_id.assign(LOG, vector<int>(n + 1, 0));
        in_id.assign(LOG, vector<int>(n + 1, 0));

        for (int u = 1; u <= n; ++u)
            out_id[0][u] = in_id[0][u] = u;

        for (int k = 1; k < LOG; ++k)
        {
            for (int u = 1; u <= n; ++u)
            {
                if ((1LL << k) > dep[u])
                    continue;

                int mid = fa[k - 1][u];

                out_id[k][u] = newnode();
                in_id[k][u] = newnode();

                // out: segment -> original points
                add(out_id[k][u], out_id[k - 1][u]);
                add(out_id[k][u], out_id[k - 1][mid]);

                // in: original points -> segment
                add(in_id[k - 1][u], in_id[k][u]);
                add(in_id[k - 1][mid], in_id[k][u]);
            }
        }
    }

    void p2p(int u, int v)
    {
        add(u, v);
    }

    void p2path(int s, int u, int v)
    {
        int L = lca(u, v);

        add_p2anc(s, u, L);

        if (v != L)
        {
            int son = kth(v, dep[v] - dep[L] - 1);
            add_p2anc(s, v, son);
        }
    }

    void path2p(int u, int v, int t)
    {
        int L = lca(u, v);

        add_anc2p(u, L, t);

        if (v != L)
        {
            int son = kth(v, dep[v] - dep[L] - 1);
            add_anc2p(v, son, t);
        }
    }

    int kth(int u, int k) const

    {
        for (int i = 0; i < LOG; ++i)
            if ((k >> i) & 1)
                u = fa[i][u];

        return u;
    }

    int lca(int u, int v) const

    {
        if (dep[u] < dep[v])
            swap(u, v);

        u = kth(u, dep[u] - dep[v]);

        if (u == v)
            return u;

        for (int k = LOG - 1; k >= 0; --k)
        {
            if (fa[k][u] != fa[k][v])
            {
                u = fa[k][u];
                v = fa[k][v];
            }
        }

        return fa[0][u];
    }

    // u != v
    int next_on_path(int u, int v) const
    {
        int L = lca(u, v);

        if (L != u)
            return fa[0][u];

        return kth(v, dep[v] - dep[u] - 1);
    }

    vector<int> topo() const

    {
        vector<int> deg = indeg;
        queue<int> q;

        for (int i = 1; i <= tot; ++i)
            if (deg[i] == 0)
                q.push(i);

        vector<int> order;
        order.reserve(n);

        int cnt = 0;

        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            ++cnt;

            if (u <= n)
                order.push_back(u);

            for (int v : g[u])
                if (--deg[v] == 0)
                    q.push(v);
        }

        if (cnt != tot)
            return {};

        return order;
    }

private:
    int newnode()
    {
        ++tot;
        g.emplace_back();
        indeg.push_back(0);
        return tot;
    }

    void add(int u, int v)
    {
        g[u].push_back(v);
        ++indeg[v];
    }

    // s -> ancestor chain [anc ... x]
    void add_p2anc(int s, int x, int anc)
    {
        int len = dep[x] - dep[anc] + 1;
        int cur = x;

        for (int k = LOG - 1; k >= 0; --k)
        {
            if ((1LL << k) <= len)
            {
                add(s, out_id[k][cur]);
                cur = fa[k][cur];
                len -= 1 << k;
            }
        }
    }

    // ancestor chain [anc ... x] -> t
    void add_anc2p(int x, int anc, int t)
    {
        int len = dep[x] - dep[anc] + 1;
        int cur = x;

        for (int k = LOG - 1; k >= 0; --k)
        {
            if ((1LL << k) <= len)
            {
                add(in_id[k][cur], t);
                cur = fa[k][cur];
                len -= 1 << k;
            }
        }
    }
};
