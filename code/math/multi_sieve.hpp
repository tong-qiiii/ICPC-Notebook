#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct MultiSieve
{
    int n = 0;
    vector<int> primes;
    vector<int> phi, mu, minp;

    MultiSieve() = default;

    MultiSieve(int n_)
    {
        init(n_);
    }

    void init(int n_)
    {
        n = n_;
        primes.clear();

        phi.assign(n + 1, 0);
        mu.assign(n + 1, 0);
        minp.assign(n + 1, 0);

        if (n >= 1)
        {
            phi[1] = 1;
            mu[1] = 1;
        }

        for (int i = 2; i <= n; ++i)
        {
            if (!minp[i])
            {
                minp[i] = i;
                primes.push_back(i);

                phi[i] = i - 1;
                mu[i] = -1;
            }

            for (int p : primes)
            {
                if (1LL * i * p > n)
                {
                    break;
                }

                int x = i * p;
                minp[x] = p;

                if (p == minp[i])
                {
                    phi[x] = phi[i] * p;
                    mu[x] = 0;
                    break;
                }

                phi[x] = phi[i] * (p - 1);
                mu[x] = -mu[i];
            }
        }
    }
};
