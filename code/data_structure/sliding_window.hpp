#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline vector<int> window_min(const vector<int> &a, int k)
{
    int n = (int)a.size() - 1;
    if (k < 1 || k > n)
    {
        return {};
    }
    deque<int> q;
    vector<int> ans;
    ans.reserve(n - k + 1);

    for (int i = 1; i <= n; ++i)
    {
        while (!q.empty() && q.front() <= i - k)
        {
            q.pop_front();
        }

        while (!q.empty() && a[q.back()] >= a[i])
        {
            q.pop_back();
        }

        q.push_back(i);

        if (i >= k)
        {
            ans.push_back(a[q.front()]);
        }
    }

    return ans;
}

inline vector<int> window_max(const vector<int> &a, int k)
{
    int n = (int)a.size() - 1;
    if (k < 1 || k > n)
    {
        return {};
    }
    deque<int> q;
    vector<int> ans;
    ans.reserve(n - k + 1);

    for (int i = 1; i <= n; ++i)
    {
        while (!q.empty() && q.front() <= i - k)
        {
            q.pop_front();
        }

        while (!q.empty() && a[q.back()] <= a[i])
        {
            q.pop_back();
        }

        q.push_back(i);

        if (i >= k)
        {
            ans.push_back(a[q.front()]);
        }
    }

    return ans;
}
