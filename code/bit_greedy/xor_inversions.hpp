#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// a[1..n]，0<=a[i]<2^62；返回 {最少逆序数, 最小掩码}。
inline pair<ll, ll> xor_inversions(const vector<ll> &a)
{
    vector<ll> v(a.begin() + 1, a.end()), tmp(v.size());
    ll cost[62][2]{};
    auto dfs = [&](auto &&self, int l, int r, int b) -> void
    {
        if (b < 0 || r - l <= 1)
        {
            return;
        }
        ll cnt[2]{};
        for (int i = l; i < r; i++)
        {
            int t = v[i] >> b & 1;
            cost[b][t] += cnt[t ^ 1];
            cnt[t]++;
        }
        // cost[b][0] 是 1 在 0 前；分组必须保留原相对顺序。
        int p = l, q = l + (int)cnt[0];
        for (int i = l; i < r; i++)
        {
            if (v[i] >> b & 1)
            {
                tmp[q++] = v[i];
            }
            else
            {
                tmp[p++] = v[i];
            }
        }
        copy(tmp.begin() + l, tmp.begin() + r, v.begin() + l);
        self(self, l, p, b - 1);
        self(self, p, r, b - 1);
    };
    dfs(dfs, 0, (int)v.size(), 61);
    ll ans = 0, mask = 0;
    for (int b = 0; b <= 61; b++)
    {
        ans += min(cost[b][0], cost[b][1]);
        if (cost[b][1] < cost[b][0])
        {
            mask |= 1LL << b;
        }
    }
    return {ans, mask};
}
