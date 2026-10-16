#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "modint.hpp"
template <int MOD> struct Lucas
{
    using Z = Mint<MOD>;

    vector<Z> fac, ifac;

    Lucas()
    {
        init();
    }

    void init()
    {
        fac.assign(MOD, 1);
        ifac.assign(MOD, 1);

        for (int i = 1; i < MOD; ++i)
        {
            fac[i] = fac[i - 1] * i;
        }

        ifac[MOD - 1] = fac[MOD - 1].inv();

        for (int i = MOD - 1; i >= 1; --i)
        {
            ifac[i - 1] = ifac[i] * i;
        }
    }

    Z smallC(int n, int k) const
    {
        if (k < 0 || k > n)
        {
            return 0;
        }

        return fac[n] * ifac[k] * ifac[n - k];
    }

    Z C(long long n, long long k) const
    {
        if (n < 0 || k < 0 || k > n)
        {
            return 0;
        }

        Z ans = 1;

        while (n || k)
        {
            ans *= smallC(n % MOD, k % MOD);
            n /= MOD;
            k /= MOD;
        }

        return ans;
    }
};
