#pragma once
#include <bits/stdc++.h>
using namespace std;
#include "multi_sieve.hpp"

struct DuJiao
{
    int b;
    vector<ll> phi, mu;
    unordered_map<ll, pair<__int128_t, ll>> memo;

    DuJiao(int b = 1000000)
    {
        init(b);
    }

    void init(int limit)
    {
        b = max(1, limit);
        MultiSieve s(b);
        phi.assign(b + 1, 0);
        mu.assign(b + 1, 0);
        for (int i = 1; i <= b; i++)
        {
            phi[i] = phi[i - 1] + s.phi[i];
            mu[i] = mu[i - 1] + s.mu[i];
        }
        memo.clear();
    }

    pair<__int128_t, ll> sum(ll n)
    {
        assert(n >= 0);
        if (n <= b) return {phi[n], mu[n]};
        auto it = memo.find(n);
        if (it != memo.end()) return it->second;
        __int128_t p = (__int128_t)n * ((__int128_t)n + 1) / 2, m = 1;
        for (ll l = 2, r; l <= n; l = r + 1)
        {
            r = n / (n / l);
            auto [x, y] = sum(n / l);
            p -= (__int128_t)(r - l + 1) * x;
            m -= (__int128_t)(r - l + 1) * y;
            if (r == n) break;
        }
        return memo[n] = {p, (ll)m};
    }
};
