#pragma once
#include <bits/stdc++.h>
using namespace std;

// u[1..k]，统计 0<=a[i]<=u[i]、和为 s 的方案数。
template<class T>
T bounded_sum(const vector<int> &u, int s)
{
    if (s < 0) return T(0);
    vector<T> dp(s + 1), ndp(s + 1);
    dp[0] = T(1);
    for (int i = 1; i < (int)u.size(); i++)
    {
        assert(u[i] >= 0);
        T val = 0;
        for (int j = 0; j <= s; j++)
        {
            val = val + dp[j];
            if (j > u[i]) val = val - dp[j - u[i] - 1];
            ndp[j] = val;
        }
        dp.swap(ndp);
    }
    return dp[s];
}
