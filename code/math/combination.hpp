#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "modint.hpp"
template <int MOD> struct Comb
{
    using Z = Mint<MOD>;

    int n = 0;
    vector<Z> fac, ifac;

    Comb() = default;

    Comb(int n_)
    {
        init(n_);
    }

    void init(int n_)
    {
        n = n_;

        fac.assign(n + 1, 1);
        ifac.assign(n + 1, 1);

        for (int i = 1; i <= n; ++i)
        {
            fac[i] = fac[i - 1] * i;
        }

        ifac[n] = fac[n].inv();

        for (int i = n; i >= 1; --i)
        {
            ifac[i - 1] = ifac[i] * i;
        }
    }

    Z C(int N, int K) const
    {
        if (N < 0 || K < 0 || K > N || N > n)
        {
            return 0;
        }

        return fac[N] * ifac[K] * ifac[N - K];
    }

    Z A(int N, int K) const
    {
        if (N < 0 || K < 0 || K > N || N > n)
        {
            return 0;
        }

        return fac[N] * ifac[N - K];
    }
};
