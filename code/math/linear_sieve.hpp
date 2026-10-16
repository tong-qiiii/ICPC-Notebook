#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct LinearSieve
{
    int n = 0;
    vector<int> primes, minp;

    LinearSieve() = default;

    LinearSieve(int n_)
    {
        init(n_);
    }

    void init(int n_)
    {
        n = n_;
        primes.clear();
        minp.assign(n + 1, 0);

        for (int i = 2; i <= n; ++i)
        {
            if (!minp[i])
            {
                minp[i] = i;
                primes.push_back(i);
            }

            for (int p : primes)
            {
                if (1LL * i * p > n)
                {
                    break;
                }

                minp[i * p] = p;

                if (p == minp[i])
                {
                    break;
                }
            }
        }
    }
};
