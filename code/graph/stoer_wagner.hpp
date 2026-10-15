#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline ll stoer_wagner(vector<vector<ll>> g)
{
    int n = (int)g.size() - 1;

    if (n <= 1)
        return 0;

    vector<int> v(n);
    iota(v.begin(), v.end(), 1);

    const ll INF = numeric_limits<ll>::max();
    ll ans = INF;

    while ((int)v.size() > 1)
    {
        int sz = v.size();

        vector<ll> dis(n + 1, 0);
        vector<char> vis(n + 1, false);

        int pre = -1;

        for (int i = 0; i < sz; ++i)
        {
            int sel = -1;

            for (int x : v)
            {
                if (!vis[x] && (sel == -1 || dis[x] > dis[sel]))
                {
                    sel = x;
                }
            }

            if (i == sz - 1)
            {
                ans = min(ans, dis[sel]);

                for (int x : v)
                {
                    if (x == sel || x == pre)
                        continue;

                    g[pre][x] += g[sel][x];
                    g[x][pre] = g[pre][x];
                }

                v.erase(find(v.begin(), v.end(), sel));

                break;
            }

            vis[sel] = true;

            for (int x : v)
            {
                if (!vis[x])
                    dis[x] += g[sel][x];
            }

            pre = sel;
        }
    }

    return ans;
}
