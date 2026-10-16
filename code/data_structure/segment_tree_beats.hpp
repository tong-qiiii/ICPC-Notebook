#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct Info
{
    static constexpr ll INF = 1LL << 60;
    ll sum = 0, mn = INF, mx = -INF;
    ll mn2 = INF, mx2 = -INF;
    int len = 0, nmn = 0, nmx = 0;

    Info() = default;
    Info(ll v) : sum(v), mn(v), mx(v), len(1), nmn(1), nmx(1)
    {
        assert(-INF < v && v < INF);
    }

    // op: 0 加，1 chmin，2 chmax。失败时不能修改信息。
    bool apply(ll v, int op)
    {
        if (!len) return true;
        if (op == 0)
        {
            sum = (ll)((__int128)sum + (__int128)v * len);
            mn += v; mx += v;
            if (mn2 != INF) mn2 += v;
            if (mx2 != -INF) mx2 += v;
        }
        else if (op == 1)
        {
            if (mx <= v) return true;
            if (mx2 >= v) return false;
            sum = (ll)((__int128)sum + (__int128)(v - mx) * nmx);
            if (mn == mx) mn = v;
            else if (mn2 == mx) mn2 = v;
            mx = v;
        }
        else
        {
            if (mn >= v) return true;
            if (mn2 <= v) return false;
            sum = (ll)((__int128)sum + (__int128)(v - mn) * nmn);
            if (mx == mn) mx = v;
            else if (mx2 == mn) mx2 = v;
            mn = v;
        }
        return true;
    }
};

inline Info operator+(const Info &a, const Info &b)
{
    if (!a.len) return b;
    if (!b.len) return a;
    Info c;
    c.sum = a.sum + b.sum;
    c.len = a.len + b.len;
    c.mx = max(a.mx, b.mx);
    c.nmx = (a.mx == c.mx ? a.nmx : 0) + (b.mx == c.mx ? b.nmx : 0);
    c.mx2 = max(a.mx == c.mx ? a.mx2 : a.mx,
                b.mx == c.mx ? b.mx2 : b.mx);
    c.mn = min(a.mn, b.mn);
    c.nmn = (a.mn == c.mn ? a.nmn : 0) + (b.mn == c.mn ? b.nmn : 0);
    c.mn2 = min(a.mn == c.mn ? a.mn2 : a.mn,
                b.mn == c.mn ? b.mn2 : b.mn);
    return c;
}

struct Seg
{
    int n = 0;
    vector<Info> info;
    vector<ll> tag;

    Seg() = default;
    Seg(const vector<ll> &a)
    {
        build(a);
    }

    void pull(int p)
    {
        info[p] = info[p << 1] + info[p << 1 | 1];
    }

    bool apply(int p, ll v, int op)
    {
        if (!info[p].apply(v, op)) return false;
        if (op == 0) tag[p] += v;
        return true;
    }

    void push(int p)
    {
        for (int u : {p << 1, p << 1 | 1})
        {
            // 必须先加，再用父节点极值限制儿子；此时一定能整段更新。
            apply(u, tag[p], 0);
            apply(u, info[p].mx, 1);
            apply(u, info[p].mn, 2);
        }
        tag[p] = 0;
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
        pull(p);
    }

    void build(const vector<ll> &a)
    {
        n = max(0, (int)a.size() - 1);
        info.assign(4 * n + 5, Info());
        tag.assign(4 * n + 5, 0);
        if (n) build(1, 1, n, a);
    }

    void modify(int p, int l, int r, int ql, int qr, ll v, int op)
    {
        if (op == 1 && info[p].mx <= v) return;
        if (op == 2 && info[p].mn >= v) return;
        if (ql <= l && r <= qr && apply(p, v, op)) return;
        push(p);
        int mid = (l + r) >> 1;
        if (ql <= mid) modify(p << 1, l, mid, ql, qr, v, op);
        if (qr > mid) modify(p << 1 | 1, mid + 1, r, ql, qr, v, op);
        pull(p);
    }

    void modify(int l, int r, ll v, int op)
    {
        if (l > r) return;
        assert(1 <= l && r <= n && 0 <= op && op <= 2);
        assert(-Info::INF < v && v < Info::INF);
        modify(1, 1, n, l, r, v, op);
    }

    void add(int l, int r, ll v)
    {
        modify(l, r, v, 0);
    }

    void chmin(int l, int r, ll v)
    {
        modify(l, r, v, 1);
    }

    void chmax(int l, int r, ll v)
    {
        modify(l, r, v, 2);
    }

    Info query(int p, int l, int r, int ql, int qr)
    {
        if (ql <= l && r <= qr) return info[p];
        push(p);
        int mid = (l + r) >> 1;
        if (qr <= mid) return query(p << 1, l, mid, ql, qr);
        if (ql > mid) return query(p << 1 | 1, mid + 1, r, ql, qr);
        return query(p << 1, l, mid, ql, qr) +
               query(p << 1 | 1, mid + 1, r, ql, qr);
    }

    Info query(int l, int r)
    {
        if (l > r) return Info();
        assert(1 <= l && r <= n);
        return query(1, 1, n, l, r);
    }
};
