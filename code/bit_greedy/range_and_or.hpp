#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// 0<=l<=r<2^62，闭区间。
inline pair<ll, ll> range_and_or(ll l, ll r)
{
    assert(0 <= l && l <= r && r < (1LL << 62));
    int b = 0;
    while ((l >> b) != (r >> b))
    {
        b++;
    }
    ll pre = (l >> b) << b;
    return {pre, pre | ((1LL << b) - 1)};
}

// 0<=n,target<2^61；最小 m>=n 使 [n,m] 的 AND=target，无解 -1。
inline ll min_and_end(ll n, ll target)
{
    assert(0 <= n && n < (1LL << 61));
    assert(0 <= target && target < (1LL << 61));
    if (target & ~n)
    {
        return -1;
    }
    ll ans = n;
    for (int b = 0; b <= 60; b++)
    {
        if ((n >> b & 1) && !(target >> b & 1))
        {
            ll step = 1LL << (b + 1);
            ans = max(ans, (n / step + 1) * step);
        }
    }
    return range_and_or(n, ans).first == target ? ans : -1;
}
