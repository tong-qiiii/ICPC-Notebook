#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct DynSeg
{

    struct Node
    {
        int lc = 0;
        int rc = 0;
        ll sum = 0;
        int tag = -1;
    };

    ll n = 0;
    int root = 0;
    std::vector<Node> tr;

    DynSeg() = default;

    DynSeg(ll n)
    {
        init(n);
    }

    void init(ll n_)
    {
        n = n_;
        tr.assign(1, Node{});
        root = n ? new_node(1, n, 1) : 0;
    }

    void assign(ll ql, ll qr, int value)
    {
        if (!root)
        {
            return;
        }

        ql = std::max(ql, 1LL);
        qr = std::min(qr, n);
        if (ql > qr)
        {
            return;
        }

        root = assign_impl(root, 1, n, ql, qr, value);
    }

    ll remain() const
    {
        return root ? tr[root].sum : 0;
    }

    ll occupied() const
    {
        return n - remain();
    }

  private:
    int new_node(ll l, ll r, int value)
    {
        tr.push_back(Node{});
        int p = (int)tr.size() - 1;
        tr[p].sum = value * (r - l + 1);
        tr[p].tag = value;
        return p;
    }

    void apply(int p, ll l, ll r, int value)
    {
        tr[p].sum = value * (r - l + 1);
        tr[p].tag = value;
    }

    void push_down(int p, ll l, ll r)
    {
        if (tr[p].tag == -1 || l == r)
        {
            return;
        }

        ll mid = l + (r - l) / 2;
        int tag = tr[p].tag;

        int lc = tr[p].lc;
        int rc = tr[p].rc;

        if (!lc)
        {
            lc = new_node(l, mid, tag);
        }
        else
        {
            apply(lc, l, mid, tag);
        }

        if (!rc)
        {
            rc = new_node(mid + 1, r, tag);
        }
        else
        {
            apply(rc, mid + 1, r, tag);
        }

        tr[p].lc = lc;
        tr[p].rc = rc;
        tr[p].tag = -1;
    }

    void pull(int p)
    {
        tr[p].sum = tr[tr[p].lc].sum + tr[tr[p].rc].sum;
    }

    int assign_impl(int p, ll l, ll r, ll ql, ll qr, int value)
    {
        if (!p)
        {
            p = new_node(l, r, 1);
        }

        if (ql <= l && r <= qr)
        {
            apply(p, l, r, value);
            return p;
        }

        push_down(p, l, r);

        ll mid = l + (r - l) / 2;

        if (ql <= mid)
        {
            tr[p].lc = assign_impl(tr[p].lc, l, mid, ql, qr, value);
        }

        if (qr > mid)
        {
            tr[p].rc = assign_impl(tr[p].rc, mid + 1, r, ql, qr, value);
        }

        pull(p);
        return p;
    }
};
