#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "poly_core.hpp"
namespace poly998
{
inline vector<int> or_convolution(vector<int> a, vector<int> b)
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

    auto fwt = [&](vector<int> &v, bool invert)
    {
        for (int bit = 1; bit < n; bit <<= 1)
        {
            for (int mask = 0; mask < n; ++mask)
            {
                if (mask & bit)
                {
                    int &x = v[mask];
                    int y = v[mask ^ bit];

                    x = invert ? x - y : x + y;

                    if (x < 0)
                    {
                        x += MOD;
                    }
                    if (x >= MOD)
                    {
                        x -= MOD;
                    }
                }
            }
        }
    };

    fwt(a, false);
    fwt(b, false);

    for (int i = 0; i < n; ++i)
    {
        a[i] = (int)(1LL * a[i] * b[i] % MOD);
    }

    fwt(a, true);

    return a;
}
} // namespace poly998
