#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

template <class Next, class Accept>
__int128 digit_count(ll x, int S, int initial, Next next, Accept accept)
{
    if (x < 0)
    {
        return 0;
    }
    string dig = to_string(x);
    vector<vector<array<__int128, 2>>> memo(
        dig.size(), vector<array<__int128, 2>>(S, {-1, -1}));
    auto dfs = [&](auto &&self, int pos, int st, bool lim,
                   bool started) -> __int128
    {
        if (pos == (int)dig.size())
        {
            return accept(st, started);
        }
        if (!lim && memo[pos][st][started] != -1)
        {
            return memo[pos][st][started];
        }
        int up = lim ? dig[pos] - '0' : 9;
        __int128 res = 0;
        for (int d = 0; d <= up; d++)
        {
            bool nlim = lim && d == dig[pos] - '0';
            if (!started && d == 0)
            {
                res += self(self, pos + 1, st, nlim, false);
                continue;
            }
            int ns = next(st, d, started);
            if (ns < 0)
            {
                continue;
            }
            assert(ns < S);
            res += self(self, pos + 1, ns, nlim, true);
        }
        if (!lim)
        {
            memo[pos][st][started] = res;
        }
        return res;
    };
    return dfs(dfs, 0, initial, true, false);
}
template <class Next, class Accept>
__int128 digit_query(ll l, ll r, int S, int initial, Next next,
                     Accept accept)
{
    if (l > r)
    {
        return 0;
    }
    return digit_count(r, S, initial, next, accept) -
           (l <= 0 ? 0 : digit_count(l - 1, S, initial, next, accept));
}
