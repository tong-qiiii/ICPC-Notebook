#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

template <class Cost> ll wqs(int n, int K, ll LIM, Cost cost)
{
    struct State
    {
        ll val;
        int cnt;
    };

    auto better = [](const State &a, const State &b)
    {
        if (a.val != b.val)
        {
            return a.val < b.val;
        }
        return a.cnt > b.cnt;
    };

    auto check = [&](ll lambda)
    {
        const ll INF = 1LL << 62;

        vector<State> dp(n + 1, {INF, -1000000000});
        dp[0] = {0, 0};

        for (int i = 1; i <= n; ++i)
        {
            for (int j = 0; j < i; ++j)
            {
                if (dp[j].val >= INF / 2)
                {
                    continue;
                }

                State cur{dp[j].val + cost(j + 1, i) + lambda,
                          dp[j].cnt + 1};

                if (better(cur, dp[i]))
                {
                    dp[i] = cur;
                }
            }
        }

        return dp[n];
    };

    ll lo = -LIM, hi = LIM;

    while (lo < hi)
    {
        ll mid = lo + (ll)(((__int128)hi - lo + 1) / 2);

        if (check(mid).cnt >= K)
        {
            lo = mid;
        }
        else
        {
            hi = mid - 1;
        }
    }

    State res = check(lo);
    ll ans = (ll)((__int128)res.val - (__int128)lo * K);
    return ans;
}
