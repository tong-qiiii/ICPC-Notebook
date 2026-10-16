#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline vector<int> nearest_smaller(const vector<int> &a)
{
    int n = (int)a.size() - 1;
    vector<int> res(n + 1);
    vector<int> stk;

    for (int i = 1; i <= n; ++i)
    {
        // 左侧第一个严格小于 a[i] 的位置
        while (!stk.empty() && a[stk.back()] >= a[i])
        {
            stk.pop_back();
        }

        res[i] = stk.empty() ? 0 : stk.back();
        stk.push_back(i);
    }

    return res;
}
