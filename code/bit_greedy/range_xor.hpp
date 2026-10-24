#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// 0<=l<=r<2^62；返回区间内两数的最大异或值，允许相同数。
inline ll range_max_xor(ll l, ll r)
{
    assert(0 <= l && l <= r && r < (1LL << 62));
    ll d = l ^ r, ans = 0;
    while (d)
    {
        ans = (ans << 1) | 1;
        d >>= 1;
    }
    return ans;
}
