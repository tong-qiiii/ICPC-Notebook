#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef db
#define db double
#endif

inline db coupon_uniform(int n)
{
    assert(n >= 0);
    db ans = 0;
    for (int i = 1; i <= n; i++) ans += (db)n / i;
    return ans;
}

// p[1..n]，剩余概率为本轮没有收集到任何类型。
inline db coupon_expect(const vector<db> &p)
{
    int n = max(0, (int)p.size() - 1);
    assert(n <= 20);
    db sum = 0;
    for (int i = 1; i <= n; i++)
    {
        assert(isfinite(p[i]) && p[i] >= 0);
        sum += p[i];
    }
    assert(sum <= 1 + 1e-10);
    for (int i = 1; i <= n; i++)
    {
        if (p[i] == 0) return numeric_limits<db>::infinity();
    }
    int m = 1 << n;
    vector<db> dp(m);
    for (int s = m - 2; s >= 0; s--)
    {
        db den = 0, val = 1;
        for (int i = 0; i < n; i++)
        {
            if (s >> i & 1) continue;
            den += p[i + 1];
            val += p[i + 1] * dp[s | (1 << i)];
        }
        // 直接累加离开概率，避免 1-p_self 的消减误差。
        dp[s] = val / den;
    }
    return dp[0];
}
