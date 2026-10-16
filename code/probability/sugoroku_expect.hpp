#pragma once
#include <bits/stdc++.h>
using namespace std;

// 在 i 等概率前进 0..a[i]，终点是 n；输入 a[1..n-1]。
template<class T>
vector<T> sugoroku_expect(const vector<int> &a)
{
    int n = (int)a.size();
    assert(n >= 1);
    vector<T> dp(n + 2), suf(n + 2);
    for (int i = n - 1; i >= 1; i--)
    {
        assert(1 <= a[i] && a[i] <= n - i);
        dp[i] = (T(a[i] + 1) + suf[i + 1] - suf[i + a[i] + 1]) / T(a[i]);
        suf[i] = suf[i + 1] + dp[i];
    }
    return dp;
}
