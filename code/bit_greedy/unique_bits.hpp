#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// a[1..n] 非空，0<=a[i]<2^62；返回独有位数值最大的原下标。
inline int unique_bits(const vector<ll> &a)
{
    assert(a.size() >= 2);
    int cnt[62]{};
    for (int i = 1; i < (int)a.size(); i++)
    {
        for (int b = 0; b <= 61; b++)
        {
            cnt[b] += a[i] >> b & 1;
        }
    }
    ll best = -1;
    int ans = 1;
    for (int i = 1; i < (int)a.size(); i++)
    {
        ll cur = 0;
        for (int b = 0; b <= 61; b++)
        {
            if (cnt[b] == 1 && (a[i] >> b & 1))
            {
                cur |= 1LL << b;
            }
        }
        if (cur > best)
        {
            best = cur;
            ans = i;
        }
    }
    return ans;
}
