#pragma once
#include <bits/stdc++.h>
using namespace std;

// 独立 0/1 序列，返回所有极大连续 1 段长度的一、二、三次方和的期望。
template<class T>
array<T, 3> run_moments(const vector<T> &p)
{
    T a = 0, b = 0;
    array<T, 3> ans{T(0), T(0), T(0)};
    for (int i = 1; i < (int)p.size(); i++)
    {
        ans[0] = ans[0] + p[i];
        ans[1] = ans[1] + p[i] * (T(2) * a + T(1));
        ans[2] = ans[2] + p[i] * (T(3) * b + T(3) * a + T(1));
        // b 依赖旧 a，不能先覆盖 a。
        b = p[i] * (b + T(2) * a + T(1));
        a = p[i] * (a + T(1));
    }
    return ans;
}
