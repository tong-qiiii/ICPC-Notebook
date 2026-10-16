#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct Seg
{
    static constexpr ll E = 0;
    int n = 0, m = 1;
    vector<ll> tr = vector<ll>(2, E);

    // 换维护量时，同时修改 merge 和单位元 E。
    static ll merge(ll a, ll b)
    {
        return a + b;
    }

    Seg() = default;
    Seg(const vector<ll> &a)
    {
        build(a);
    }

    void build(const vector<ll> &a)
    {
        n = max(0, (int)a.size() - 1);
        m = 1;
        while (m < n) m <<= 1;
        tr.assign(m << 1, E);
        for (int i = 1; i <= n; i++) tr[m + i - 1] = a[i];
        for (int p = m - 1; p; p--) tr[p] = merge(tr[p << 1], tr[p << 1 | 1]);
    }

    void modify(int p, ll v)
    {
        assert(1 <= p && p <= n);
        p += m - 1;
        tr[p] = v;
        while (p >>= 1) tr[p] = merge(tr[p << 1], tr[p << 1 | 1]);
    }

    ll query(int l, int r) const
    {
        if (l > r) return E;
        assert(1 <= l && r <= n);
        ll a = E, b = E;
        // 内部 [l,r)，保留左右合并顺序。
        for (l += m - 1, r += m; l < r; l >>= 1, r >>= 1)
        {
            if (l & 1) a = merge(a, tr[l++]);
            if (r & 1) b = merge(tr[--r], b);
        }
        return merge(a, b);
    }
};
