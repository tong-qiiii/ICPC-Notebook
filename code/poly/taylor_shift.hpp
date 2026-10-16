#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "poly_core.hpp"
namespace poly998
{
inline Poly Poly::shift(long long c) const
{
    int n = size();

    if (n <= 1)
    {
        return *this;
    }

    assert(n < MOD);

    ensure_fact(n - 1);
    c = norm(c);

    vector<int> p(n), q(n);

    long long pw = 1;

    for (int i = 0; i < n; ++i)
    {
        p[n - 1 - i] = (int)(1LL * a[i] * fac[i] % MOD);
        q[i] = (int)(pw * ifac[i] % MOD);
        pw = pw * c % MOD;
    }

    vector<int> r = convolution(p, q);
    vector<int> ans(n);

    for (int i = 0; i < n; ++i)
    {
        ans[i] = (int)(1LL * r[n - 1 - i] * ifac[i] % MOD);
    }

    return Poly(move(ans));
}
} // namespace poly998
