#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline long long qpow(long long a, long long b, long long mod)
{
    a %= mod;
    if (a < 0)
    {
        a += mod;
    }

    long long res = 1 % mod;

    while (b)
    {
        if (b & 1)
        {
            res = (__int128)res * a % mod;
        }

        a = (__int128)a * a % mod;
        b >>= 1;
    }

    return res;
}
