#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#ifndef pll
#define pll pair<long long, long long>
#endif
inline int min_cover(vector<pll> segs, ll L, ll R)
{
    if (L > R)
    {
        return -1;
    }

    int n = segs.size() - 1;

    if (L == R)
    {
        for (int i = 1; i <= n; i++)
        {
            auto [l, r] = segs[i];

            if (l <= L && L <= r)
            {
                return 1;
            }
        }
        return -1;
    }

    sort(segs.begin() + 1, segs.end());

    int i = 1;
    int cnt = 0;
    ll cur = L;

    while (cur < R)
    {
        ll far = cur;

        while (i <= n && segs[i].first <= cur)
        {
            far = max(far, segs[i].second);
            i++;
        }

        if (far <= cur)
        {
            return -1;
        }

        cur = far;
        cnt++;
    }

    return cnt;
}
