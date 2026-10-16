#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#ifndef pll
#define pll pair<long long, long long>
#endif
inline pair<int, vector<int>> interval_group(const vector<pll> &segs)
{
    int n = segs.size() - 1;
    vector<int> ord(n + 1);
    iota(ord.begin() + 1, ord.end(), 1);

    sort(ord.begin() + 1, ord.end(),
         [&](int a, int b)
         {
             if (segs[a].first != segs[b].first)
             {
                 return segs[a].first < segs[b].first;
             }
             return segs[a].second < segs[b].second;
         });

    priority_queue<pll, vector<pll>, greater<pll>> q;
    vector<int> bel(n + 1);
    int cnt = 0;

    for (int i = 1; i <= n; i++)
    {
        int idx = ord[i];
        auto [l, r] = segs[idx];
        int id;

        if (!q.empty() && q.top().first < l)
        {
            id = q.top().second;
            q.pop();
        }
        else
        {
            id = ++cnt;
        }

        bel[idx] = id;
        q.push({r, id});
    }

    return {cnt, bel};
}
