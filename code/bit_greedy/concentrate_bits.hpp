#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// a[1..n]，0<=a[i]<2^62；返回同样下标的规范集中结果。
inline vector<ll> concentrate_bits(const vector<ll> &a)
{
    int n = (int)a.size() - 1;
    vector<ll> ans(n + 1);
    for (int b = 0; b <= 61; b++)
    {
        int cnt = 0;
        for (int i = 1; i <= n; i++)
        {
            cnt += a[i] >> b & 1;
        }
        for (int i = 1; i <= cnt; i++)
        {
            ans[i] |= 1LL << b;
        }
    }
    return ans;
}
