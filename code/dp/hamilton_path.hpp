#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline ll hamilton_path(const vector<vector<ll>> &w, int s, int t)
{
    const ll INF = LLONG_MAX / 4;
    int n = w.size();
    if (!n)
    {
        return 0;
    }
    assert(n <= 20);

    int S = 1 << n;

    vector<vector<ll>> dp(S, vector<ll>(n, INF));
    dp[1 << s][s] = 0;

    for (int mask = 0; mask < S; ++mask)
    {
        for (int u = 0; u < n; ++u)
        {
            if (!(mask >> u & 1) || dp[mask][u] == INF)
            {
                continue;
            }

            for (int v = 0; v < n; ++v)
            {
                if (mask >> v & 1)
                {
                    continue;
                }
                if (w[u][v] >= INF / 2)
                {
                    continue;
                }

                int nmask = mask | (1 << v);
                dp[nmask][v] = min(dp[nmask][v], dp[mask][u] + w[u][v]);
            }
        }
    }

    return dp[S - 1][t];
}
