#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// 默认：区间加
struct Tag
{
    ll add = 0;

    Tag(ll add = 0) : add(add)
    {
    }

    void apply(const Tag &t)
    {
        add += t.add;
    }

    bool empty() const
    {
        return add == 0;
    }
};

// 默认：区间和
struct Info
{
    ll sum = 0;
    int len = 0;

    Info() = default;

    Info(ll x) : sum(x), len(1)
    {
    }

    void apply(const Tag &t)
    {
        sum += 1LL * len * t.add;
    }
};

inline Info operator+(const Info &A, const Info &B)
{
    Info res;
    res.sum = A.sum + B.sum;
    res.len = A.len + B.len;
    return res;
}

struct Seg
{
    int n = 0;
    vector<Info> info;
    vector<Tag> tag;

    Seg() = default;

    Seg(const vector<ll> &a)
    {
        build(a);
    }

    void build(const vector<ll> &a)
    {
        n = (int)a.size() - 1;
        info.assign(n * 4 + 5, Info());
        tag.assign(n * 4 + 5, Tag());
        if (n)
        {
            build(1, 1, n, a);
        }
    }

    void pushup(int p)
    {
        info[p] = info[p << 1] + info[p << 1 | 1];
    }

    void apply(int p, const Tag &t)
    {
        info[p].apply(t);
        tag[p].apply(t);
    }

    void pushdown(int p)
    {
        if (tag[p].empty())
        {
            return;
        }

        apply(p << 1, tag[p]);
        apply(p << 1 | 1, tag[p]);
        tag[p] = Tag();
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

    void modify(int p, int l, int r, int ql, int qr, const Tag &t)
    {
        if (ql <= l && r <= qr)
        {
            apply(p, t);
            return;
        }

        pushdown(p);

        int mid = (l + r) >> 1;

        if (ql <= mid)
        {
            modify(p << 1, l, mid, ql, qr, t);
        }
        if (qr > mid)
        {
            modify(p << 1 | 1, mid + 1, r, ql, qr, t);
        }

        pushup(p);
    }

    void modify(int l, int r, const Tag &t)
    {
        modify(1, 1, n, l, r, t);
    }

    Info query(int p, int l, int r, int ql, int qr)
    {
        if (ql <= l && r <= qr)
        {
            return info[p];
        }

        pushdown(p);

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

    Info query(int l, int r)
    {
        return query(1, 1, n, l, r);
    }
};
