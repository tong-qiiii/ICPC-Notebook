#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif
#include "../graph/dsu.hpp"

// 点 1..n，边表不加哨兵，0 <= w < 2^62；断图返回 -1。
inline ll min_or_tree(int n, const vector<tuple<int, int, ll>> &edges)
{
    assert(n >= 1);
    ll mask = 0;
    for (auto [u, v, w] : edges)
    {
        mask |= w;
    }
    auto check = [&](ll allowed)
    {
        DSU d(n);
        for (auto [u, v, w] : edges)
        {
            if (!(w & ~allowed))
            {
                d.merge(u, v);
            }
        }
        for (int u = 2; u <= n; u++)
        {
            if (!d.same(1, u))
            {
                return false;
            }
        }
        return true;
    };
    if (!check(mask))
    {
        return -1;
    }
    for (int b = 61; b >= 0; b--)
    {
        ll next = mask & ~(1LL << b);
        if (check(next))
        {
            mask = next;
        }
    }
    return mask;
}
