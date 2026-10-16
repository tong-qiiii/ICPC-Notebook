#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

inline int modPow(int a, int e)
{
    ll res = 1;

    while (e)
    {
        if (e & 1)
        {
            res = res * a % MOD;
        }
        a = (ll)a * a % MOD;
        e >>= 1;
    }

    return res;
}

struct Lagrange
{
    int n;
    vector<int> x, y, w;

    Lagrange(vector<int> x_, vector<int> y_)
        : n(x_.size()), x(move(x_)), y(move(y_)), w(n, 1)
    {
        assert(x.size() == y.size());
        for (int &v : x)
        {
            v %= MOD;
            if (v < 0)
            {
                v += MOD;
            }
        }

        for (int &v : y)
        {
            v %= MOD;
            if (v < 0)
            {
                v += MOD;
            }
        }

        for (int i = 0; i < n; ++i)
        {
            ll den = 1;

            for (int j = 0; j < n; ++j)
            {
                if (i == j)
                {
                    continue;
                }

                int d = x[i] - x[j];
                if (d < 0)
                {
                    d += MOD;
                }

                den = den * d % MOD;
            }

            assert(den != 0);
            w[i] = modPow(den, MOD - 2);
        }
    }

    int query(ll X) const
    {
        int z = X % MOD;
        if (z < 0)
        {
            z += MOD;
        }

        for (int i = 0; i < n; ++i)
        {
            if (z == x[i])
            {
                return y[i];
            }
        }

        ll prod = 1;

        for (int i = 0; i < n; ++i)
        {
            int d = z - x[i];
            if (d < 0)
            {
                d += MOD;
            }

            prod = prod * d % MOD;
        }

        ll ans = 0;

        for (int i = 0; i < n; ++i)
        {
            int d = z - x[i];
            if (d < 0)
            {
                d += MOD;
            }

            ll cur = (ll)y[i] * w[i] % MOD;
            cur = cur * modPow(d, MOD - 2) % MOD;

            ans += cur;
            if (ans >= MOD)
            {
                ans -= MOD;
            }
        }

        return prod * ans % MOD;
    }
};
