#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "extended_gcd.hpp"
inline long long inv_mod(long long a, long long mod)
{
    a %= mod;
    if (a < 0)
    {
        a += mod;
    }

    long long x, y;
    long long g = exgcd(a, mod, x, y);

    if (g != 1)
    {
        return -1;
    }

    x %= mod;
    if (x < 0)
    {
        x += mod;
    }
    return x;
}
