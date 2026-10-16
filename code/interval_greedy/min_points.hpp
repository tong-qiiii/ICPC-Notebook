#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#ifndef pll
#define pll pair<long long, long long>
#endif
inline vector<ll> min_points(vector<pll> segs)
{
    int n = segs.size() - 1;

    sort(segs.begin() + 1, segs.end(),
         [](const pll &a, const pll &b)
         {
             if (a.second != b.second)
             {
                 return a.second < b.second;
             }
             return a.first < b.first;
         });

    vector<ll> points(1);
    ll last = 0;
    bool has = false;

    for (int i = 1; i <= n; i++)
    {
        auto [l, r] = segs[i];

        if (!has || last < l)
        {
            last = r;
            has = true;
            points.push_back(r);
        }
    }

    return points;
}
