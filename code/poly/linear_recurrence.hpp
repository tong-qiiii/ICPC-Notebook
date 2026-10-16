#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "bostan_mori.hpp"
namespace poly998
{
inline int linear_recurrence_nth(const vector<int> &init,
                                 const vector<int> &coef, long long k)
{
    assert(k >= 0);

    int d = (int)coef.size();

    if (k < (long long)init.size())
    {
        return norm(init[(int)k]);
    }

    if (d == 0)
    {
        return 0;
    }

    assert((int)init.size() >= d);

    vector<int> q(d + 1);

    q[0] = 1;

    for (int i = 1; i <= d; ++i)
    {
        q[i] = norm(-1LL * coef[i - 1]);
    }

    vector<int> s(d);

    for (int i = 0; i < d; ++i)
    {
        s[i] = norm(init[i]);
    }

    Poly Q(move(q));
    Poly P = (Poly(move(s)) * Q).mod_xk(d);

    return bostan_mori(P, Q, k);
}
} // namespace poly998
