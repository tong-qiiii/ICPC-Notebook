#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline ll knuth_merge(const vector<ll> &a)
{
    const ll INF = LLONG_MAX / 4;

    int n = (int)a.size() - 1;
    if (n <= 0)
    {
        return 0;
    }
    vector<ll> pre(n + 1);
    for (int i = 1; i <= n; ++i)
    {

        pre[i] = pre[i - 1] + a[i];
    }

    vector<vector<ll>> dp(n + 2, vector<ll>(n + 2));
    vector<vector<int>> opt(n + 2, vector<int>(n + 2));

    for (int i = 1; i <= n; ++i)
    {
        opt[i][i] = i;
    }

    for (int len = 2; len <= n; ++len)
    {
        for (int l = 1; l + len - 1 <= n; ++l)
        {
            int r = l + len - 1;
            dp[l][r] = INF;

            int L = opt[l][r - 1];
            int R = min(opt[l + 1][r], r - 1);

            for (int k = L; k <= R; ++k)
            {
                ll val = dp[l][k] + dp[k + 1][r];

                if (val < dp[l][r])
                {
                    dp[l][r] = val;
                    opt[l][r] = k;
                }
            }

            dp[l][r] += pre[r] - pre[l - 1];
        }
    }

    return dp[1][n];
}
