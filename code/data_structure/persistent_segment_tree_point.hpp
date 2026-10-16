#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct Info
{
    ll sum = 0;

    Info()
    {
    }

    Info(ll x)
    {
        sum = x;
    }

    friend Info operator+(const Info &a, const Info &b)
    {
        return Info(a.sum + b.sum);
    }
};

struct PST
{
    struct Node
    {
        int lc = 0;
        int rc = 0;
        Info info;
    };

    vector<Node> tr;

    PST()
    {
        init();
    }

    void init()
    {
        tr.assign(1, {});
    }

    int clone(int p)
    {
        tr.push_back(tr[p]);
        return (int)tr.size() - 1;
    }

    void pull(int p)
    {
        tr[p].info = tr[tr[p].lc].info + tr[tr[p].rc].info;
    }

    int build(const vector<Info> &a, int l, int r)
    {
        int p = clone(0);

        if (l == r)
        {
            tr[p].info = a[l];
            return p;
        }

        int mid = (l + r) >> 1;
        tr[p].lc = build(a, l, mid);
        tr[p].rc = build(a, mid + 1, r);
        pull(p);

        return p;
    }

    int modify(int pre, int l, int r, int pos, const Info &v)
    {
        int cur = clone(pre);

        if (l == r)
        {
            tr[cur].info = v;
            return cur;
        }

        int mid = (l + r) >> 1;

        if (pos <= mid)
        {
            tr[cur].lc = modify(tr[pre].lc, l, mid, pos, v);
        }
        else
        {
            tr[cur].rc = modify(tr[pre].rc, mid + 1, r, pos, v);
        }

        pull(cur);

        return cur;
    }

    Info query(int p, int l, int r, int L, int R) const
    {
        if (L <= l && r <= R)
        {
            return tr[p].info;
        }

        int mid = (l + r) >> 1;
        Info ans;

        if (L <= mid)
        {
            ans = ans + query(tr[p].lc, l, mid, L, R);
        }

        if (R > mid)
        {
            ans = ans + query(tr[p].rc, mid + 1, r, L, R);
        }

        return ans;
    }
};
