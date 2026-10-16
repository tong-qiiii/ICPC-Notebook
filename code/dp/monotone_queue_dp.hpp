#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline vector<ll> monotone_queue_dp(const vector<ll> &a, int k)
{
    const ll INF = LLONG_MAX / 4;

    int n = (int)a.size() - 1;
    vector<ll> dp(n + 1, INF);
    assert(k >= 1);

    deque<int> q;
    dp[0] = 0;
    q.push_back(0);

    for (int i = 1; i <= n; ++i)
    {
        while (!q.empty() && q.front() < i - k)
        {
            q.pop_front();
        }

        dp[i] = dp[q.front()] + a[i];

        while (!q.empty() && dp[q.back()] >= dp[i])
        {
            q.pop_back();
        }
        q.push_back(i);
    }

    return dp;
}
