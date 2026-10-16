#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef db
#define db double
#endif

// tail[k] = P(T>=k)，tail[0] 不用；返回 E[T], E[T^2], Var(T)。
template<class T>
array<T, 3> tail_moments(const vector<T> &tail)
{
    T e = 0, e2 = 0;
    for (int k = 1; k < (int)tail.size(); k++)
    {
        e = e + tail[k];
        e2 = e2 + T(2LL * k - 1) * tail[k];
    }
    return {e, e2, e2 - e * e};
}

// 从 n 种等概率元素独立抽样，第一次重复时停止，包含重复的那一次。
inline db birthday_expect(int n)
{
    assert(n >= 1);
    db ans = 1, survive = 1;
    for (int k = 1; k <= n; k++)
    {
        survive *= (db)(n - k + 1) / n;
        ans += survive;
    }
    return ans;
}
