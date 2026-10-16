#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#ifndef pll
#define pll pair<long long, long long>
#endif
inline ll count_pairs_small(int n, const vector<pll> &segs)
{
    int m = segs.size() - 1;
    vector<vector<int>> by_l(n + 1);

    for (int i = 1; i <= m; i++)
    {
        auto [l, r] = segs[i];
        by_l[l].push_back(r);
    }

    ll ans = 0;
    int mn = n + 1;

    for (int l = n; l >= 1; l--)
    {
        for (int r : by_l[l])
        {
            mn = min(mn, r);
        }

        if (mn <= n)
        {
            ans += n - mn + 1;
        }
    }

    return ans;
}
