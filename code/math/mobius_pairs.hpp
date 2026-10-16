#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct Mobius
{
    int n = 0;
    vector<int> primes, mu;
    vector<long long> pre;
    vector<char> vis;

    Mobius() = default;

    Mobius(int n_)
    {
        init(n_);
    }

    void init(int n_)
    {
        n = n_;
        primes.clear();

        mu.assign(n + 1, 0);
        pre.assign(n + 1, 0);
        vis.assign(n + 1, false);

        if (n >= 1)
        {
            mu[1] = 1;
        }

        for (int i = 2; i <= n; ++i)
        {
            if (!vis[i])
            {
                primes.push_back(i);
                mu[i] = -1;
            }

            for (int p : primes)
            {
                if (1LL * i * p > n)
                {
                    break;
                }

                vis[i * p] = true;

                if (i % p == 0)
                {
                    mu[i * p] = 0;
                    break;
                }

                mu[i * p] = -mu[i];
            }
        }

        for (int i = 1; i <= n; ++i)
        {
            pre[i] = pre[i - 1] + mu[i];
        }
    }

    long long coprime_pairs(int a, int b) const
    {
        int limit = min(a, b);
        long long ans = 0;

        for (int l = 1, r; l <= limit; l = r + 1)
        {
            r = min(a / (a / l), b / (b / l));

            ans += (pre[r] - pre[l - 1]) * 1LL * (a / l) * (b / l);
        }

        return ans;
    }

    long long gcd_eq(int a, int b, int k) const
    {
        if (k <= 0)
        {
            return 0;
        }

        return coprime_pairs(a / k, b / k);
    }
};
