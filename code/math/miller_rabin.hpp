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

inline u64 mulMod(u64 a, u64 b, u64 mod)
{
    return (u128)a * b % mod;
}

inline u64 powMod(u64 a, u64 e, u64 mod)
{
    u64 res = 1;

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

inline bool isPrime(u64 n)
{
    if (n < 2)
    {
        return false;
    }

    for (u64 p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL,
                  29ULL, 31ULL, 37ULL})
    {
        if (n % p == 0)
        {
            return n == p;
        }
    }

    u64 d = n - 1, s = 0;

    while (!(d & 1))
    {
        d >>= 1;
        ++s;
    }

    for (u64 a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL,
                  1795265022ULL})
    {
        if (a % n == 0)
        {
            continue;
        }

        u64 x = powMod(a % n, d, n);

        if (x == 1 || x == n - 1)
        {
            continue;
        }

        bool composite = true;

        for (u64 r = 1; r < s; ++r)
        {
            x = mulMod(x, x, n);

            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }

        if (composite)
        {
            return false;
        }
    }

    return true;
}
