#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct PDSU
{
    struct Node
    {
        int l, r;
        int fa, rk;
    };

    int n, base;
    vector<Node> tr;

    PDSU(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        tr.clear();
        tr.push_back({});
        base = n ? build(1, n) : 0;
    }

    int new_node()
    {
        tr.push_back({});
        return tr.size() - 1;
    }

    int new_node(const Node& x)
    {
        tr.push_back(x);
        return tr.size() - 1;
    }

    int build(int l, int r)
    {
        int u = new_node();

        if (l == r)
        {
            tr[u].fa = l;
            return u;
        }

        int mid = (l + r) >> 1;
        tr[u].l = build(l, mid);
        tr[u].r = build(mid + 1, r);
        return u;
    }

    int get(int u, int l, int r, int x) const

    {
        if (l == r)
            return u;

        int mid = (l + r) >> 1;

        if (x <= mid)
            return get(tr[u].l, l, mid, x);
        else
            return get(tr[u].r, mid + 1, r, x);
    }

    int modify(int pre, int l, int r, int x, int fa, int rk)
    {
        int u = new_node(tr[pre]);

        if (l == r)
        {
            if (fa != -1)
                tr[u].fa = fa;
            if (rk != -1)
                tr[u].rk = rk;
            return u;
        }

        int mid = (l + r) >> 1;

        if (x <= mid)
            tr[u].l = modify(tr[pre].l, l, mid, x, fa, rk);
        else
            tr[u].r = modify(tr[pre].r, mid + 1, r, x, fa, rk);

        return u;
    }

    int find(int root, int x) const

    {
        while (true)
        {
            int u = get(root, 1, n, x);

            if (tr[u].fa == x)
                return x;

            x = tr[u].fa;
        }
    }

    int merge(int root, int x, int y)
    {
        x = find(root, x);
        y = find(root, y);

        if (x == y)
            return root;

        int px = get(root, 1, n, x);
        int py = get(root, 1, n, y);

        if (tr[px].rk > tr[py].rk)
        {
            swap(x, y);
            swap(px, py);
        }

        int res = modify(root, 1, n, x, y, -1);

        if (tr[px].rk == tr[py].rk)
            res = modify(res, 1, n, y, -1, tr[py].rk + 1);

        return res;
    }

    bool same(int root, int x, int y) const

    {
        return find(root, x) == find(root, y);
    }
};
