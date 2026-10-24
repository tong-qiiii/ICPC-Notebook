#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// 非负异或值 u、和 s；无解 {-1}，u=s=0 时空数组；输出元素无哨兵。
inline vector<ll> sum_xor_construct(ll u, ll s)
{
    assert(u >= 0 && s >= 0);
    if (u > s || ((s - u) & 1))
    {
        return {-1};
    }
    if (u == s)
    {
        if (!u)
        {
            return {};
        }
        return {u};
    }
    ll d = (s - u) / 2;
    if (!(d & u))
    {
        return {d, d + u};
    }
    return {u, d, d};
}
