#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include <bits/stdc++.h>
using namespace std;

using u64 = uint64_t;
using u128 = __uint128_t;

inline mt19937_64
    rng(chrono::steady_clock::now().time_since_epoch().count());

#include "miller_rabin.hpp"
inline u64 pollardRho(u64 n)
{
    if (n % 2 == 0)
    {
        return 2;
    }
    if (n % 3 == 0)
    {
        return 3;
    }

    while (true)
    {
        u64 c = uniform_int_distribution<u64>(1, n - 1)(rng);
        u64 x = uniform_int_distribution<u64>(0, n - 1)(rng);
        u64 y = x;
        u64 d = 1;

        auto f = [&](u64 x)
        { return (u64)(((u128)mulMod(x, x, n) + c) % n); };

        while (d == 1)
        {
            x = f(x);
            y = f(f(y));

            u64 diff = x > y ? x - y : y - x;
            d = gcd(diff, n);
        }

        if (d != n)
        {
            return d;
        }
    }
}

inline void factorDfs(u64 n, vector<u64> &fac)
{
    if (n == 1)
    {
        return;
    }

    if (isPrime(n))
    {
        fac.push_back(n);
        return;
    }

    u64 d = pollardRho(n);

    factorDfs(d, fac);
    factorDfs(n / d, fac);
}

inline vector<u64> factorize(u64 n)
{
    if (!n)
    {
        return {};
    }
    vector<u64> fac;

    factorDfs(n, fac);
    sort(fac.begin(), fac.end());

    return fac;
}
