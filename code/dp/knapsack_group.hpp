#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline vector<ll>
knapsack_group(int C, const vector<vector<pair<int, ll>>> &groups,
               bool exact = false)
{
    const ll NEG = LLONG_MIN / 4;
    vector<ll> dp(C + 1, exact ? NEG : 0);
    dp[0] = 0;
    for (int i = 1; i < (int)groups.size(); i++)
    {
        for (int j = C; j >= 0; j--)
        {
            for (auto [w, v] : groups[i])
            {
                assert(w > 0);
                if (j >= w && dp[j - w] != NEG)
                {
                    dp[j] = max(dp[j], dp[j - w] + v);
                }
            }
        }
    }
    return dp;
}
