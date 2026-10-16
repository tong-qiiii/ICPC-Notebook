#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline int cartesian_tree(const vector<int> &a, vector<int> &ls,
                          vector<int> &rs)
{
    int n = (int)a.size() - 1;
    vector<int> stk;
    ls.assign(n + 1, 0);
    rs.assign(n + 1, 0);

    for (int i = 1; i <= n; ++i)
    {
        int last = 0;
        while (!stk.empty() && a[stk.back()] > a[i])
        {
            last = stk.back();
            stk.pop_back();
        }

        ls[i] = last;
        if (!stk.empty())
        {
            rs[stk.back()] = i;
        }
        stk.push_back(i);
    }

    return stk.empty() ? 0 : stk[0];
}
