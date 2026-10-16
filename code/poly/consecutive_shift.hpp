#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "poly_core.hpp"
namespace poly998
{
inline vector<int> consecutive_value_shift(const vector<int> &y,
                                           long long c, int m)
{
    c = norm(c);
    int n = (int)y.size();

    if (m <= 0)
    {
        return {};
    }
    if (n == 0)
    {
        return vector<int>(m);
    }

    assert(n < MOD);

    ensure_fact(n - 1);

    int len = n + m - 1;
    vector<int> vals(len);

    for (int p = 0; p < len; ++p)
    {
        vals[p] = norm(c + p - (n - 1LL));
    }

    vector<int> invs = batch_inv(vals);
    vector<int> a(n);

    for (int i = 0; i < n; ++i)
    {
        long long v =
            1LL * norm(y[i]) * ifac[i] % MOD * ifac[n - 1 - i] % MOD;

        if ((n - 1 - i) & 1)
        {
            v = v ? MOD - v : 0;
        }

        a[i] = (int)v;
    }

    vector<int> conv = convolution(a, invs);

    int zeros = 0;
    long long prod = 1;

    for (int p = 0; p < n; ++p)
    {
        if (vals[p] == 0)
        {
            ++zeros;
        }
        else
        {
            prod = prod * vals[p] % MOD;
        }
    }

    vector<int> ans(m);

    for (int j = 0; j < m; ++j)
    {
        int x = norm(c + j);

        if (x < n)
        {
            ans[j] = norm(y[x]);
        }
        else
        {
            assert(zeros == 0);
            ans[j] = (int)(prod * conv[j + n - 1] % MOD);
        }

        if (j + 1 == m)
        {
            break;
        }

        int out = j;
        int in = j + n;

        if (vals[out] == 0)
        {
            --zeros;
        }
        else
        {
            prod = prod * invs[out] % MOD;
        }

        if (vals[in] == 0)
        {
            ++zeros;
        }
        else
        {
            prod = prod * vals[in] % MOD;
        }
    }

    return ans;
}
} // namespace poly998
