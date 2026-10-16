#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// 默认：区间和
struct Info
{
    ll sum = 0;

    Info() = default;

    Info(ll x) : sum(x)
    {
    }
};

inline Info operator+(const Info &A, const Info &B)
{
    Info res;
    res.sum = A.sum + B.sum;
    return res;
}

struct Seg
{
    int n = 0;
    vector<Info> info;

    Seg() = default;

    Seg(const vector<ll> &a)
    {
        build(a);
    }

    void build(const vector<ll> &a)
    {
        n = (int)a.size() - 1;
        info.assign(n * 4 + 5, Info());
        if (n)
        {
            build(1, 1, n, a);
        }
    }

    void pushup(int p)
    {
        info[p] = info[p << 1] + info[p << 1 | 1];
    }

    void build(int p, int l, int r, const vector<ll> &a)
    {
        if (l == r)
        {
            info[p] = Info(a[l]);
            return;
        }

        int mid = (l + r) >> 1;

        build(p << 1, l, mid, a);
        build(p << 1 | 1, mid + 1, r, a);

        pushup(p);
    }

    void modify(int p, int l, int r, int x, const Info &v)
    {
        if (l == r)
        {
            info[p] = v;
            return;
        }

        int mid = (l + r) >> 1;

        if (x <= mid)
        {
            modify(p << 1, l, mid, x, v);
        }
        else
        {
            modify(p << 1 | 1, mid + 1, r, x, v);
        }

        pushup(p);
    }

    void modify(int x, const Info &v)
    {
        modify(1, 1, n, x, v);
    }

    Info query(int p, int l, int r, int ql, int qr) const
    {
        if (ql <= l && r <= qr)
        {
            return info[p];
        }

        int mid = (l + r) >> 1;

        if (qr <= mid)
        {
            return query(p << 1, l, mid, ql, qr);
        }

        if (ql > mid)
        {
            return query(p << 1 | 1, mid + 1, r, ql, qr);
        }

        return query(p << 1, l, mid, ql, qr) +
               query(p << 1 | 1, mid + 1, r, ql, qr);
    }

    Info query(int l, int r) const
    {
        return query(1, 1, n, l, r);
    }
};
