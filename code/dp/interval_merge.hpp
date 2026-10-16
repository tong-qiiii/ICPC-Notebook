#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline ll interval_merge(const vector<ll> &a)
{
    const ll INF = LLONG_MAX / 4;

    int n = (int)a.size() - 1;
    if (n <= 0)
    {
        return 0;
    }
    vector<ll> pre(n + 1);
    vector<vector<ll>> dp(n + 1, vector<ll>(n + 1, INF));

    for (int i = 1; i <= n; ++i)
    {

        pre[i] = pre[i - 1] + a[i];
        dp[i][i] = 0;
    }

    for (int len = 2; len <= n; ++len)
    {
        for (int l = 1; l + len - 1 <= n; ++l)
        {
            int r = l + len - 1;
            ll sum = pre[r] - pre[l - 1];

            for (int k = l; k < r; ++k)
            {
                dp[l][r] = min(dp[l][r], dp[l][k] + dp[k + 1][r] + sum);
            }
        }
    }

    return dp[1][n];
}
