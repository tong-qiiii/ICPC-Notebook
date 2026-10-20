#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct BITRange
{
    int n;
    vector<ll> t, s;

    BITRange(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        t.assign(n + 1, 0);
        s.assign(n + 1, 0);
    }

    void add(int l, int r, ll v)
    {
        assert(l >= 1 && l <= r && r <= n);
        auto change = [&](int p, ll x)
        {
            for (int i = p; i <= n; i += i & -i)
            {
                t[i] += x;
                s[i] += x * (p - 1);
            }
        };
        change(l, v);
        change(r + 1, -v);
    }

    ll sum(int p) const
    {
        assert(p >= 0 && p <= n);
        ll a = 0, b = 0;
        for (int i = p; i; i -= i & -i)
        {
            a += t[i];
            b += s[i];
        }
        return a * p - b;
    }

    ll query(int l, int r) const
    {
        assert(l >= 1 && l <= r && r <= n);
        return sum(r) - sum(l - 1);
    }
};
