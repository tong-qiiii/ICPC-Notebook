#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include <bits/stdc++.h>
using namespace std;

using i128 = __int128_t;

inline ll safeMod(ll x, ll m)
{
    x %= m;
    if (x < 0)
    {
        x += m;
    }
    return x;
}

inline i128 floorSumUnsigned(ll n, ll m, ll a, ll b)
{
    i128 ans = 0;

    while (true)
    {
        if (a >= m)
        {
            ans += (i128)(n - 1) * n / 2 * (a / m);
            a %= m;
        }

        if (b >= m)
        {
            ans += (i128)n * (b / m);
            b %= m;
        }

        i128 y = (i128)a * n + b;

        if (y < m)
        {
            break;
        }

        n = (ll)(y / m);
        b = (ll)(y % m);
        swap(a, m);
    }

    return ans;
}

inline i128 floorSum(ll n, ll m, ll a, ll b)
{
    i128 ans = 0;

    if (a < 0)
    {
        ll a2 = safeMod(a, m);
        ans -= (i128)n * (n - 1) / 2 * ((a2 - (i128)a) / m);
        a = a2;
    }

    if (b < 0)
    {
        ll b2 = safeMod(b, m);
        ans -= (i128)n * ((b2 - (i128)b) / m);
        b = b2;
    }

    return ans + floorSumUnsigned(n, m, a, b);
}
