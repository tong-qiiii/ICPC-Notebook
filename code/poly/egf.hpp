#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "poly_core.hpp"
namespace poly998
{
inline Poly egf_from_sequence(const vector<int> &seq)
{
    int n = (int)seq.size();

    if (n == 0)
    {
        return Poly();
    }

    assert(n <= MOD);

    ensure_fact(n - 1);

    vector<int> a(n);

    for (int i = 0; i < n; ++i)
    {
        a[i] = (int)(1LL * norm(seq[i]) * ifac[i] % MOD);
    }

    return Poly(move(a));
}
} // namespace poly998
