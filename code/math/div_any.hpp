#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline long long div_any(long long n, const vector<long long> &d)
{
    int m = d.size();
    long long ans = 0;

    for (uint64_t mask = 1; mask < (1ULL << m); ++mask)
    {
        __int128 lcm = 1;
        bool valid = true;

        for (int i = 0; i < m; ++i)
        {
            if (!(mask >> i & 1))
            {
                continue;
            }

            long long g = gcd((long long)lcm, d[i]);
            lcm = lcm / g * d[i];

            if (lcm > n)
            {
                valid = false;
                break;
            }
        }

        if (!valid)
        {
            continue;
        }

        long long cnt = n / (long long)lcm;

        if (__builtin_popcountll(mask) & 1)
        {
            ans += cnt;
        }
        else
        {
            ans -= cnt;
        }
    }

    return ans;
}
