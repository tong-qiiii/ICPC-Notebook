#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline vector<ll> knapsack_complete(int C,
                                    const vector<pair<int, ll>> &items,
                                    bool exact = false)
{
    const ll NEG = LLONG_MIN / 4;
    vector<ll> dp(C + 1, exact ? NEG : 0);
    dp[0] = 0;
    for (int i = 1; i < (int)items.size(); i++)
    {
        auto [w, v] = items[i];
        assert(w > 0);
        for (int j = w; j <= C; j++)
        {
            if (dp[j - w] != NEG)
            {
                dp[j] = max(dp[j], dp[j - w] + v);
            }
        }
    }
    return dp;
}
