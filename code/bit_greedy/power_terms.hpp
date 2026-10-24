#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// 能否把 s 写成恰好 k 个正二次幂之和，允许重复；0 项表示空和。
inline bool power_terms(ll s, ll k)
{
    if (s < 0 || k < 0)
    {
        return false;
    }
    return __builtin_popcountll(s) <= k && k <= s;
}
