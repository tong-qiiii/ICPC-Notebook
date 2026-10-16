#pragma once
#include <bits/stdc++.h>
using namespace std;

template<class T>
vector<T> walk_steps(const vector<vector<pair<int, T>>> &g, int s, int k)
{
    int n = (int)g.size() - 1;
    assert(1 <= s && s <= n && k >= 0);
    vector<T> dp(n + 1), ndp(n + 1);
    dp[s] = T(1);
    while (k--)
    {
        fill(ndp.begin(), ndp.end(), T(0));
        for (int u = 1; u <= n; u++)
        {
            for (auto [v, p] : g[u])
            {
                ndp[v] = ndp[v] + dp[u] * p;
            }
        }
        dp.swap(ndp);
    }
    return dp;
}
