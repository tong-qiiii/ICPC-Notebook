#pragma once
#include <bits/stdc++.h>
using namespace std;

template<class T>
vector<T> bernoulli_dp(const vector<T> &p)
{
    int n = max(0, (int)p.size() - 1);
    vector<T> dp(n + 1);
    dp[0] = T(1);
    for (int i = 1; i <= n; i++)
    {
        // 倒序，避免同一轮的成功次数被重复使用。
        for (int j = i; j >= 0; j--)
        {
            dp[j] = dp[j] * (T(1) - p[i]) + (j ? dp[j - 1] * p[i] : T(0));
        }
    }
    return dp;
}
