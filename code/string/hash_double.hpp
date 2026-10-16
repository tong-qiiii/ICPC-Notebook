#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct Hash2
{
    using Hash = pair<int, int>;

    static constexpr int MOD1 = 1000000007;
    static constexpr int MOD2 = 1000000009;
    static constexpr int BASE = 911382323;

    int n = 0;
    vector<int> p1, p2;
    vector<int> h1, h2;

    Hash2() = default;

    Hash2(const string &s)
    {
        build(s);
    }

    void build(const string &s)
    {
        n = (int)s.size();

        p1.assign(n + 1, 1);
        p2.assign(n + 1, 1);
        h1.assign(n + 1, 0);
        h2.assign(n + 1, 0);

        for (int i = 1; i <= n; ++i)
        {
            p1[i] = 1LL * p1[i - 1] * BASE % MOD1;
            p2[i] = 1LL * p2[i - 1] * BASE % MOD2;

            int c = (unsigned char)s[i - 1] + 1;

            h1[i] = (1LL * h1[i - 1] * BASE + c) % MOD1;
            h2[i] = (1LL * h2[i - 1] * BASE + c) % MOD2;
        }
    }

    Hash get(int l, int r) const
    {
        int len = r - l + 1;

        int x1 = (h1[r + 1] - 1LL * h1[l] * p1[len] % MOD1 + MOD1) % MOD1;

        int x2 = (h2[r + 1] - 1LL * h2[l] * p2[len] % MOD2 + MOD2) % MOD2;

        return {x1, x2};
    }

    bool equal(int l1, int r1, int l2, int r2) const
    {
        if (r1 - l1 != r2 - l2)
        {
            return false;
        }

        return get(l1, r1) == get(l2, r2);
    }

    Hash merge(Hash left, Hash right, int right_len) const
    {
        int x1 = (1LL * left.first * p1[right_len] + right.first) % MOD1;

        int x2 = (1LL * left.second * p2[right_len] + right.second) % MOD2;

        return {x1, x2};
    }
};
