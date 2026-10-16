#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "mo_rollback_add.hpp"
inline vector<ll> mo_max_span(const vector<int> &a, vector<Query> q, int V)
{
    int n = (int)a.size() - 1;
    vector<int> lmn(V, n + 1);
    vector<int> rmx(V, -1);

    vector<array<int, 3>> stk;

    ll now = 0;

    auto add_right = [&](int pos)
    {
        int x = a[pos];

        lmn[x] = min(lmn[x], pos);
        rmx[x] = max(rmx[x], pos);

        now = max(now, 1LL * rmx[x] - lmn[x]);
    };

    auto add_left = [&](int pos)
    {
        int x = a[pos];

        stk.push_back({x, lmn[x], rmx[x]});

        lmn[x] = min(lmn[x], pos);
        rmx[x] = max(rmx[x], pos);

        now = max(now, 1LL * rmx[x] - lmn[x]);
    };

    auto rollback_left = [&](int)
    {
        auto [x, old_l, old_r] = stk.back();
        stk.pop_back();

        lmn[x] = old_l;
        rmx[x] = old_r;
    };
    auto clear = [&]()
    {
        fill(lmn.begin(), lmn.end(), n + 1);
        fill(rmx.begin(), rmx.end(), -1);
        stk.clear();
        now = 0;
    };
    return mo_rollback_add(n, move(q), now, add_right, add_left,
                           rollback_left, clear);
}
