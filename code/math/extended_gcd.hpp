#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline long long exgcd(long long a, long long b, long long &x, long long &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }

    long long x1, y1;
    long long g = exgcd(b, a % b, x1, y1);

    x = y1;
    y = (long long)((__int128)x1 - (__int128)(a / b) * y1);
    return g;
}
