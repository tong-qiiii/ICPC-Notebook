#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#ifndef pll
#define pll pair<long long, long long>
#endif
inline ll count_pairs_large(ll n, vector<pll> segs)
{
    int m = segs.size() - 1;
    sort(segs.begin() + 1, segs.end(), greater<pll>());

    ll ans = 0;
    ll mn = n;
    bool has = false;

    for (int i = 1; i <= m;)
    {
        ll l = segs[i].first;
        int j = i;

        while (j <= m && segs[j].first == l)
        {
            mn = min(mn, segs[j].second);
            has = true;
            j++;
        }

        ll nl = (j <= m ? segs[j].first : 0);

        if (has)
        {
            ans += (l - nl) * (n - mn + 1);
        }

        i = j;
    }

    return ans;
}
