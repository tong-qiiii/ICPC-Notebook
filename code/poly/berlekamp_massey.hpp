#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "poly_core.hpp"
namespace poly998
{
inline vector<int> berlekamp_massey(const vector<int> &seq)
{
    vector<int> C{1};
    vector<int> B{1};

    int L = 0;
    int m = 1;
    int b = 1;

    for (int n = 0; n < (int)seq.size(); ++n)
    {
        long long d = norm(seq[n]);

        for (int i = 1; i <= L; ++i)
        {
            d = (d + 1LL * C[i] * norm(seq[n - i])) % MOD;
        }

        if (d == 0)
        {
            ++m;
            continue;
        }

        vector<int> T = C;
        int coef = (int)(d * invmod(b) % MOD);

        if ((int)C.size() < (int)B.size() + m)
        {
            C.resize(B.size() + m);
        }

        for (int i = 0; i < (int)B.size(); ++i)
        {
            C[i + m] = norm(C[i + m] - 1LL * coef * B[i]);
        }

        if (2 * L <= n)
        {
            L = n + 1 - L;
            B = move(T);
            b = (int)d;
            m = 1;
        }
        else
        {
            ++m;
        }
    }

    vector<int> coef(L);

    for (int i = 1; i <= L; ++i)
    {
        coef[i - 1] = norm(-C[i]);
    }

    return coef;
}
} // namespace poly998
