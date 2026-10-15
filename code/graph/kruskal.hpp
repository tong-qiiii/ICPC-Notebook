#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif
#include "dsu.hpp"

struct MstEdge
{
    int u, v;
    ll w;
};

inline pair<bool, ll> kruskal(int n, vector<MstEdge> e)
{
    if (n <= 1) return {true, 0};
    sort(e.begin(), e.end(), [](const MstEdge& a, const MstEdge& b)
    {
        return a.w < b.w;
    });

    DSU dsu(n);
    int cnt = 0;
    ll ans = 0;

    for (auto [u, v, w] : e)
    {
        if (dsu.merge(u, v))
        {
            ans += w;
            if (++cnt == n - 1)
                break;
        }
    }

    return {cnt == n - 1, ans};
}
