#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef db
#define db double
#endif

inline db log_choose(int n, int k)
{
    if (k < 0 || k > n) return -numeric_limits<db>::infinity();
    return lgamma((db)n + 1) - lgamma((db)k + 1) - lgamma((db)(n - k) + 1);
}

inline db binom_pmf(int n, int k, db p)
{
    assert(n >= 0 && 0 <= p && p <= 1);
    if (k < 0 || k > n) return 0;
    if (p == 0) return k == 0;
    if (p == 1) return k == n;
    return exp(log_choose(n, k) + k * log(p) + (n - k) * log1p(-p));
}

// k 是首次成功所需的试验次数，包含最后的成功。
inline db geom_pmf(int k, db p)
{
    assert(0 < p && p <= 1);
    if (k < 1) return 0;
    if (p == 1) return k == 1;
    return p * exp((k - 1) * log1p(-p));
}

inline db poisson_pmf(int k, db rate)
{
    assert(isfinite(rate) && rate >= 0);
    if (k < 0) return 0;
    if (rate == 0) return k == 0;
    return exp(k * log(rate) - rate - lgamma((db)k + 1));
}

// 总数 n，成功类 m，不放回抽 take 个，恰有 k 个成功。
inline db hyper_pmf(int n, int m, int take, int k)
{
    assert(0 <= m && m <= n && 0 <= take && take <= n);
    if (k < 0 || k > m || k > take || take - k > n - m) return 0;
    return exp(log_choose(m, k) + log_choose(n - m, take - k) - log_choose(n, take));
}
