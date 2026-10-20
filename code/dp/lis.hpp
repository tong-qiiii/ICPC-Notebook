#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// 输入 1-index；返回一个最优子序列的原数组下标。
inline vector<int> lis(const vector<ll> &a, bool strict = true)
{
    int n = max(0, (int)a.size() - 1);
    vector<ll> tail;
    vector<int> pos, pre(n + 1);
    for (int i = 1; i <= n; i++)
    {
        auto it = strict ? lower_bound(tail.begin(), tail.end(), a[i])
                         : upper_bound(tail.begin(), tail.end(), a[i]);
        int j = it - tail.begin();
        pre[i] = j ? pos[j - 1] : 0;
        if (it == tail.end())
        {
            tail.push_back(a[i]);
            pos.push_back(i);
        }
        else
        {
            *it = a[i];
            pos[j] = i;
        }
    }
    vector<int> ans;
    for (int p = pos.empty() ? 0 : pos.back(); p; p = pre[p]) ans.push_back(p);
    reverse(ans.begin(), ans.end());
    return ans;
}
