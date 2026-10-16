#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct Tag
{
    ll add = 0;

    Tag()
    {
    }

    Tag(ll x)
    {
        add = x;
    }

    bool empty() const
    {
        return add == 0;
    }

    void apply(const Tag &tag)
    {
        add += tag.add;
    }
};

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

    void apply(const Tag &tag, int l, int r)
    {
        sum += tag.add * (r - l + 1);
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
        Tag tag;
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

    void apply(int p, int l, int r, const Tag &tag)
    {
        tr[p].info.apply(tag, l, r);
        tr[p].tag.apply(tag);
    }

    void push(int p, int l, int r)
    {
        if (l == r || tr[p].tag.empty())
        {
            return;
        }

        int mid = (l + r) >> 1;

        tr[p].lc = clone(tr[p].lc);
        tr[p].rc = clone(tr[p].rc);

        apply(tr[p].lc, l, mid, tr[p].tag);
        apply(tr[p].rc, mid + 1, r, tr[p].tag);

        tr[p].tag = {};
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

    int modify(int pre, int l, int r, int L, int R, const Tag &tag)
    {
        int cur = clone(pre);

        if (L <= l && r <= R)
        {
            apply(cur, l, r, tag);
            return cur;
        }

        push(cur, l, r);

        int mid = (l + r) >> 1;

        if (L <= mid)
        {
            tr[cur].lc = modify(tr[cur].lc, l, mid, L, R, tag);
        }

        if (R > mid)
        {
            tr[cur].rc = modify(tr[cur].rc, mid + 1, r, L, R, tag);
        }

        pull(cur);

        return cur;
    }

    Info query(int p, int l, int r, int L, int R, Tag tag = {}) const
    {
        if (L <= l && r <= R)
        {
            Info res = tr[p].info;
            res.apply(tag, l, r);
            return res;
        }

        Tag nxt = tr[p].tag;
        nxt.apply(tag);

        int mid = (l + r) >> 1;
        Info ans;

        if (L <= mid)
        {
            ans = ans + query(tr[p].lc, l, mid, L, R, nxt);
        }

        if (R > mid)
        {
            ans = ans + query(tr[p].rc, mid + 1, r, L, R, nxt);
        }

        return ans;
    }
};
