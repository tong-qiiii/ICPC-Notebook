#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include <bits/stdc++.h>
using namespace std;

using i128 = __int128_t;

inline ll mulMod(ll a, ll b, ll mod)
{
    return (i128)a * b % mod;
}

inline ll powMod(ll a, ll e, ll mod)
{
    ll res = 1 % mod;

    while (e)
    {
        if (e & 1)
        {
            res = mulMod(res, a, mod);
        }
        a = mulMod(a, a, mod);
        e >>= 1;
    }

    return res;
}

inline ll modSqrt(ll n, ll p)
{
    n %= p;
    if (n < 0)
    {
        n += p;
    }

    if (n == 0)
    {
        return 0;
    }
    if (p == 2)
    {
        return n;
    }
    if (powMod(n, (p - 1) / 2, p) != 1)
    {
        return -1;
    }
    if (p % 4 == 3)
    {
        return powMod(n, (p + 1) / 4, p);
    }

    ll q = p - 1;
    int s = 0;

    while (!(q & 1))
    {
        q >>= 1;
        ++s;
    }

    ll z = 2;

    while (powMod(z, (p - 1) / 2, p) != p - 1)
    {
        ++z;
    }

    ll c = powMod(z, q, p);
    ll x = powMod(n, (q + 1) / 2, p);
    ll t = powMod(n, q, p);
    int m = s;

    while (t != 1)
    {
        ll cur = t;
        int i = 0;

        while (cur != 1)
        {
            cur = mulMod(cur, cur, p);
            ++i;
        }

        ll b = c;

        for (int j = 0; j < m - i - 1; ++j)
        {
            b = mulMod(b, b, p);
        }

        x = mulMod(x, b, p);
        ll b2 = mulMod(b, b, p);

        t = mulMod(t, b2, p);
        c = b2;
        m = i;
    }

    return x;
}
