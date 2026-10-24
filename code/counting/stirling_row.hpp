#pragma once
#include <bits/stdc++.h>
using namespace std;

// first=false：第二类；first=true：无符号第一类。只保留第 n 行的 0..k 项。
template<class T>
vector<T> stirling_row(int n, int k = -1, bool first = false)
{
    assert(n >= 0 && k >= -1);
    if (k == -1) k = n;
    assert(k <= n);
    vector<T> dp(k + 1);
    dp[0] = T(1);
    for (int i = 1; i <= n; i++)
    {
        for (int j = min(i, k); j >= 1; j--)
        {
            dp[j] = dp[j - 1] + T(first ? i - 1 : j) * dp[j];
        }
        dp[0] = T(0);
    }
    return dp;
}
