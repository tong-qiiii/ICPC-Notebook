#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include <bits/stdc++.h>
using namespace std;

using i128 = __int128_t;

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

inline ll norm(ll x, ll mod)
{
    x %= mod;
    if (x < 0)
    {
        x += mod;
    }
    return x;
}

inline pair<ll, ll> crt(const vector<ll> &r, const vector<ll> &m)
{
    ll ans = 0, mod = 1;

    for (int i = 0; i < (int)r.size(); ++i)
    {
        ll x, y;
        assert(m[i] > 0);
        ll g = exgcd(mod, m[i], x, y);
        assert(g == 1);

        ll d = (ll)(((i128)r[i] - ans) % m[i]);
        d = norm(d, m[i]);
        ll t = (ll)((i128)d * x % m[i]);
        t = norm(t, m[i]);

        i128 newMod = (i128)mod * m[i];
        assert(newMod <= LLONG_MAX);
        i128 newAns = (i128)ans + (i128)mod * t;

        ans = (ll)(newAns % newMod);
        mod = (ll)newMod;
    }

    return {ans, mod};
}
