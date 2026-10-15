#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline void floyd(vector<vector<ll>>& dis)
{
    int n = dis.size() - 1;
    const ll INF = 1LL << 62;

    for (int k = 1; k <= n; ++k)
    {
        for (int i = 1; i <= n; ++i)
        {
            if (dis[i][k] == INF)
                continue;

            for (int j = 1; j <= n; ++j)
            {
                if (dis[k][j] == INF)
                    continue;

                __int128 sum = (__int128)dis[i][k] + dis[k][j];
                dis[i][j] = min(dis[i][j], (ll)max(sum, -(__int128)INF));
            }
        }
    }
}
