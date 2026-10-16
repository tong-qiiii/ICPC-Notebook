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

inline ll exgcd(ll a, ll b, ll &x, ll &y)
{
    if (!b)
    {
        x = 1;
        y = 0;
        return a;
    }

    ll x1, y1;
    ll g = exgcd(b, a % b, x1, y1);

    x = y1;
    y = (long long)((__int128)x1 - (__int128)(a / b) * y1);

    return g;
}

inline ll invMod(ll a, ll mod)
{
    ll x, y;
    ll g = exgcd(a, mod, x, y);

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

inline ll BSGS(ll a, ll b, ll mod)
{
    if (mod == 1)
    {
        return 0;
    }

    a %= mod;
    if (a < 0)
    {
        a += mod;
    }
    b %= mod;
    if (b < 0)
    {
        b += mod;
    }

    if (b == 1)
    {
        return 0;
    }
    if (gcd(a, mod) != 1)
    {
        return -1;
    }

    ll n = sqrtl((long double)mod);

    while ((i128)n * n < mod)
    {
        ++n;
    }
    while (n > 0 && (i128)(n - 1) * (n - 1) >= mod)
    {
        --n;
    }

    unordered_map<ll, ll> mp;

    ll cur = 1;

    for (ll j = 0; j < n; ++j)
    {
        if (!mp.count(cur))
        {
            mp[cur] = j;
        }
        cur = mulMod(cur, a, mod);
    }

    ll inv = invMod(a, mod);
    ll step = powMod(inv, n, mod);

    cur = b;

    for (ll i = 0; i <= n; ++i)
    {
        auto it = mp.find(cur);

        if (it != mp.end())
        {
            return i * n + it->second;
        }

        cur = mulMod(cur, step, mod);
    }

    return -1;
}

inline ll exBSGS(ll a, ll b, ll mod)
{
    if (mod == 1)
    {
        return 0;
    }

    a %= mod;
    if (a < 0)
    {
        a += mod;
    }
    b %= mod;
    if (b < 0)
    {
        b += mod;
    }

    if (b == 1)
    {
        return 0;
    }

    ll cnt = 0;
    ll k = 1 % mod;

    while (true)
    {
        ll g = gcd(a, mod);

        if (g == 1)
        {
            break;
        }
        if (b % g)
        {
            return -1;
        }

        mod /= g;
        b /= g;

        k = mulMod(k, a / g, mod);
        ++cnt;

        if (k == b)
        {
            return cnt;
        }
    }

    ll inv = invMod(k, mod);
    ll target = mulMod(b, inv, mod);
    ll res = BSGS(a, target, mod);

    return res == -1 ? -1 : res + cnt;
}
