#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "division_evaluation.hpp"
namespace poly998
{
inline int bostan_mori(Poly P, Poly Q, long long n)
{
    assert(n >= 0);
    assert(Q[0] != 0);

    int extra = 0;

    if (P.size() >= Q.size())
    {
        auto [quo, rem] = P.divmod(Q);

        if (n <= quo.deg())
        {
            extra = quo[(int)n];
        }

        P = move(rem);
    }

    while (n > 0 && !P.empty())
    {
        vector<int> qneg = Q.a;

        for (int i = 1; i < (int)qneg.size(); i += 2)
        {
            if (qneg[i])
            {
                qneg[i] = MOD - qneg[i];
            }
        }

        Poly R = P * Poly(qneg);
        Poly S = Q * Poly(move(qneg));

        vector<int> np, nq;

        for (int i = (int)(n & 1); i < R.size(); i += 2)
        {
            np.push_back(R.a[i]);
        }

        for (int i = 0; i < S.size(); i += 2)
        {
            nq.push_back(S.a[i]);
        }

        P = Poly(move(np));
        Q = Poly(move(nq));

        n >>= 1;
    }

    int ans = P.empty() ? 0 : (int)(1LL * P[0] * invmod(Q[0]) % MOD);

    ans += extra;
    if (ans >= MOD)
    {
        ans -= MOD;
    }

    return ans;
}
} // namespace poly998
