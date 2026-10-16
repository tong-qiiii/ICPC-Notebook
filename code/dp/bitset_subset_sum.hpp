#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

template <size_t MAXC> bitset<MAXC + 1> subset_sum(const vector<int> &a)
{
    bitset<MAXC + 1> dp;
    dp[0] = 1;
    for (int i = 1; i < (int)a.size(); i++)
    {
        assert(a[i] >= 0);
        dp |= dp << a[i];
    }
    return dp;
}
