#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

template <size_t N>
void closure(vector<bitset<N>>& reach, int n)
{
    for (int k = 1; k <= n; ++k)
        for (int i = 1; i <= n; ++i)
            if (reach[i][k])
                reach[i] |= reach[k];
}
