#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// 0<=a,b,r<2^62；返回 {最小绝对差, 一种 0<=mask<=r 的方案}。
inline pair<ll, ll> xor_distance(ll a, ll b, ll r)
{
    assert(a >= 0 && b >= 0 && r >= 0);
    if (a < b)
    {
        swap(a, b);
    }
    int top = -1;
    for (int i = 61; i >= 0; i--)
    {
        if ((a ^ b) >> i & 1)
        {
            top = i;
            break;
        }
    }
    ll mask = 0;
    for (int i = top - 1; i >= 0; i--)
    {
        ll bit = 1LL << i;
        if ((a & bit) && !(b & bit) && bit <= r - mask)
        {
            mask |= bit;
        }
    }
    return {(a ^ mask) - (b ^ mask), mask};
}
