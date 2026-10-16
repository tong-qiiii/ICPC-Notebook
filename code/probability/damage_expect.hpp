#pragma once
#include <bits/stdc++.h>
using namespace std;

// 每次以 p 造成 2 点伤害，否则 1 点，生命 <=0 后停止。
template<class T>
vector<T> damage_expect(int n, T p)
{
    assert(n >= 0);
    vector<T> dp(n + 1);
    for (int i = 1; i <= n; i++)
    {
        dp[i] = T(1) + (T(1) - p) * dp[i - 1] + p * dp[max(0, i - 2)];
    }
    return dp;
}
