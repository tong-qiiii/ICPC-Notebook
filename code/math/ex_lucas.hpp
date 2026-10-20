#pragma once
#include <bits/stdc++.h>
using namespace std;
#include "crt.hpp"

struct ExLucas
{
    struct Part
    {
        int p, mod;
        vector<int> f;
    };
    int mod;
    vector<Part> parts;

    ExLucas(int m = 1)
    {
        init(m);
    }

    void init(int m)
    {
        assert(m >= 1);
        mod = m;
        parts.clear();
        for (int p = 2; p <= m / p; p++)
        {
            if (m % p) continue;
            int pk = 1;
            while (m % p == 0) m /= p, pk *= p;
            parts.push_back({p, pk, {}});
        }
        if (m > 1) parts.push_back({m, m, {}});
        for (auto &s : parts)
        {
            s.f.assign((size_t)s.mod + 1, 1);
            for (int i = 1; i <= s.mod; i++)
            {
                s.f[i] = 1LL * s.f[i - 1] * (i % s.p ? i : 1) % s.mod;
            }
        }
    }

    ll C(ll n, ll k) const
    {
        if (n < 0 || k < 0 || k > n || mod == 1) return 0;
        auto power = [&](ll a, ll e, int m)
        {
            ll res = 1;
            for (; e; e >>= 1, a = a * a % m)
            {
                if (e & 1) res = res * a % m;
            }
            return res;
        };
        vector<ll> r, mods;
        for (const auto &s : parts)
        {
            auto count = [&](ll x)
            {
                ll res = 0;
                while (x) x /= s.p, res += x;
                return res;
            };
            auto fact = [&](ll x)
            {
                ll res = 1;
                for (; x; x /= s.p)
                {
                    res = res * power(s.f[s.mod], x / s.mod, s.mod) % s.mod;
                    res = res * s.f[x % s.mod] % s.mod;
                }
                return res;
            };
            auto inv = [&](ll x)
            {
                ll u, v;
                exgcd(x, s.mod, u, v);
                return (u % s.mod + s.mod) % s.mod;
            };
            ll e = count(n) - count(k) - count(n - k);
            ll res = fact(n) * inv(fact(k)) % s.mod;
            res = res * inv(fact(n - k)) % s.mod;
            res = res * power(s.p, e, s.mod) % s.mod;
            r.push_back(res);
            mods.push_back(s.mod);
        }
        return crt(r, mods).first;
    }
};
