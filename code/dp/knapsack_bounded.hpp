#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct BoundedItem
{
    int w, c;
    ll v;
};
inline vector<ll> knapsack_bounded(int C, const vector<BoundedItem> &items,
                                   bool exact = false)
{
    const ll NEG = LLONG_MIN / 4;
    vector<ll> dp(C + 1, exact ? NEG : 0);
    dp[0] = 0;
    for (int i = 1; i < (int)items.size(); i++)
    {
        auto [w, c, v] = items[i];
        assert(w > 0 && c >= 0);
        c = min(c, C / w);
        for (ll k = 1; c > 0; k *= 2)
        {
            int take = (int)min(k, (ll)c);
            ll ww = 1LL * w * take;
            ll vv = v * take;
            c -= take;
            for (int j = C; j >= ww; j--)
            {
                if (dp[j - ww] != NEG)
                {
                    dp[j] = max(dp[j], dp[j - ww] + vv);
                }
            }
        }
    }
    return dp;
}
