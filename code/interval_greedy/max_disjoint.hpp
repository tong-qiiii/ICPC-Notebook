#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#ifndef pll
#define pll pair<long long, long long>
#endif
inline int max_disjoint(vector<pll> segs)
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

    int ans = 0;
    ll last = 0;
    bool has = false;

    for (int i = 1; i <= n; i++)
    {
        auto [l, r] = segs[i];

        if (!has || l > last)
        {
            ans++;
            last = r;
            has = true;
        }
    }

    return ans;
}
