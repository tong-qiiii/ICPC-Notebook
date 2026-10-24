#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// a[1..n] 非负；返回字典序最小的掩码数组，使 a[i]^ans[i] 按位包含前项。
inline vector<ll> growing_mask(const vector<ll> &a)
{
    vector<ll> ans(a.size());
    ll pre = 0;
    for (int i = 1; i < (int)a.size(); i++)
    {
        ans[i] = pre & ~a[i];
        pre |= a[i];
    }
    return ans;
}
