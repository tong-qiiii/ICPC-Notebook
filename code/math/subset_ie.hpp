#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

template <class F> auto subset_ie(int m, F work)
{
    using T = decltype(work(1ULL));
    T ans = 0;

    for (uint64_t mask = 1; mask < (1ULL << m); ++mask)
    {
        T val = work(mask);

        if (__builtin_popcountll(mask) & 1)
        {
            ans += val;
        }
        else
        {
            ans -= val;
        }
    }

    return ans;
}
