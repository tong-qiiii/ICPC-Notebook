#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline long long floor_sum(long long n)
{
    long long ans = 0;

    for (long long l = 1, r; l <= n; l = r + 1)
    {
        long long q = n / l;
        r = n / q;
        ans += (r - l + 1) * q;
    }

    return ans;
}
