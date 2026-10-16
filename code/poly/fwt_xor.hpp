#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "poly_core.hpp"
namespace poly998
{
inline vector<int> xor_convolution(vector<int> a, vector<int> b)
{
    if (a.empty() || b.empty())
    {
        return {};
    }

    int n = 1;
    while (n < (int)max(a.size(), b.size()))
    {
        n <<= 1;
    }

    a.resize(n);
    b.resize(n);

    for (int &x : a)
    {
        x = norm(x);
    }
    for (int &x : b)
    {
        x = norm(x);
    }

    auto fwt = [&](vector<int> &v)
    {
        for (int len = 1; len < n; len <<= 1)
        {
            for (int i = 0; i < n; i += len << 1)
            {
                for (int j = 0; j < len; ++j)
                {
                    int x = v[i + j];
                    int y = v[i + j + len];

                    int p = x + y;
                    if (p >= MOD)
                    {
                        p -= MOD;
                    }

                    int q = x - y;
                    if (q < 0)
                    {
                        q += MOD;
                    }

                    v[i + j] = p;
                    v[i + j + len] = q;
                }
            }
        }
    };

    fwt(a);
    fwt(b);

    for (int i = 0; i < n; ++i)
    {
        a[i] = (int)(1LL * a[i] * b[i] % MOD);
    }

    fwt(a);

    int inv_n = invmod(n);

    for (int &x : a)
    {
        x = (int)(1LL * x * inv_n % MOD);
    }

    return a;
}
} // namespace poly998
