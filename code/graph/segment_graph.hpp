#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct SegGraph
{
    struct Edge
    {
        int to;
        ll w;
    };

    int n, tot;
    vector<vector<Edge>> g;
    vector<int> out_id, in_id;

    SegGraph(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        tot = n;

        g.assign(n + 1, {});
        out_id.assign(4 * n + 5, 0);
        in_id.assign(4 * n + 5, 0);

        if (n)
            build(1, 1, n);
    }

    void add(int u, int v, ll w)
    {
        g[u].push_back({v, w});
    }

    void p2p(int u, int v, ll w)
    {
        add(u, v, w);
    }

    void p2r(int u, int L, int R, ll w)
    {
        link_p2r(1, 1, n, L, R, u, w);
    }

    void r2p(int L, int R, int v, ll w)
    {
        link_r2p(1, 1, n, L, R, v, w);
    }

    void r2r(int L1, int R1, int L2, int R2, ll w)
    {
        int mid = newnode();

        r2p(L1, R1, mid, w);
        p2r(mid, L2, R2, 0);
    }

private:
    int newnode()
    {
        ++tot;
        g.emplace_back();
        return tot;
    }

    void build(int id, int l, int r)
    {
        if (l == r)
        {
            out_id[id] = in_id[id] = l;
            return;
        }

        int mid = (l + r) >> 1;

        build(id << 1, l, mid);
        build(id << 1 | 1, mid + 1, r);

        out_id[id] = newnode();
        in_id[id] = newnode();

        // out: auxiliary -> original points
        add(out_id[id], out_id[id << 1], 0);
        add(out_id[id], out_id[id << 1 | 1], 0);

        // in: original points -> auxiliary
        add(in_id[id << 1], in_id[id], 0);
        add(in_id[id << 1 | 1], in_id[id], 0);
    }

    void link_p2r(int id, int l, int r, int L, int R, int u, ll w)
    {
        if (L <= l && r <= R)
        {
            add(u, out_id[id], w);
            return;
        }

        int mid = (l + r) >> 1;

        if (L <= mid)
            link_p2r(id << 1, l, mid, L, R, u, w);

        if (R > mid)
            link_p2r(id << 1 | 1, mid + 1, r, L, R, u, w);
    }

    void link_r2p(int id, int l, int r, int L, int R, int v, ll w)
    {
        if (L <= l && r <= R)
        {
            add(in_id[id], v, w);
            return;
        }

        int mid = (l + r) >> 1;

        if (L <= mid)
            link_r2p(id << 1, l, mid, L, R, v, w);

        if (R > mid)
            link_r2p(id << 1 | 1, mid + 1, r, L, R, v, w);
    }
};
