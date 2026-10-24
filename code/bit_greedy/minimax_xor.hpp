#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// a[1..n]，0<=a[i]<2^62；返回 {最小的最大异或值, 一种掩码}。
inline pair<ll, ll> minimax_xor(const vector<ll> &a)
{
    vector<ll> v(a.begin() + 1, a.end());
    sort(v.begin(), v.end());
    auto dfs = [&](auto &&self, int l, int r, int b) -> pair<ll, ll>
    {
        if (b < 0 || l == r)
        {
            return {0, 0};
        }
        int mid = l;
        while (mid < r && !(v[mid] >> b & 1))
        {
            mid++;
        }
        ll bit = 1LL << b;
        if (mid == r)
        {
            return self(self, l, r, b - 1);
        }
        if (mid == l)
        {
            auto ans = self(self, l, r, b - 1);
            ans.second |= bit;
            return ans;
        }
        auto z = self(self, l, mid, b - 1);
        auto o = self(self, mid, r, b - 1);
        if (z.first < o.first)
        {
            return {bit + z.first, bit | z.second};
        }
        return {bit + o.first, o.second};
    };
    return dfs(dfs, 0, (int)v.size(), 61);
}
