#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "poly_core.hpp"
namespace poly998
{
inline vector<int> chirp_z(const Poly &f, int q, int m)
{
    if (m <= 0)
    {
        return {};
    }

    vector<int> ans(m);

    if (f.empty())
    {
        return ans;
    }

    q = norm(q);

    if (q == 0)
    {
        int sum = 0;

        for (int x : f.a)
        {
            sum += x;
            if (sum >= MOD)
            {
                sum -= MOD;
            }
        }

        ans[0] = sum;

        for (int i = 1; i < m; ++i)
        {
            ans[i] = f[0];
        }

        return ans;
    }

    int n = f.size();
    int lim = n + m - 1;
    int iq = invmod(q);

    vector<int> pos(lim);
    vector<int> neg(max(n, m));

    long long cur = 1, pw = 1;

    // pos[i] = q^(i(i-1)/2)
    for (int i = 0; i < lim; ++i)
    {
        pos[i] = (int)cur;
        cur = cur * pw % MOD;
        pw = pw * q % MOD;
    }

    cur = 1;
    pw = 1;

    // neg[i] = q^(-i(i-1)/2)
    for (int i = 0; i < (int)neg.size(); ++i)
    {
        neg[i] = (int)cur;
        cur = cur * pw % MOD;
        pw = pw * iq % MOD;
    }

    vector<int> a(n);

    for (int i = 0; i < n; ++i)
    {
        a[n - 1 - i] = (int)(1LL * f.a[i] * neg[i] % MOD);
    }

    vector<int> c = convolution(a, pos);

    for (int k = 0; k < m; ++k)
    {
        ans[k] = (int)(1LL * c[n - 1 + k] * neg[k] % MOD);
    }

    return ans;
}
} // namespace poly998
