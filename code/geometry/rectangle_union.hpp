#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline __int128_t rectangle_union(vector<array<ll, 4>> a)
{
    vector<tuple<ll, ll, ll, int>> e;
    vector<ll> ys;
    for (auto [x1, y1, x2, y2] : a)
    {
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        if (x1 == x2 || y1 == y2) continue;
        e.push_back({x1, y1, y2, 1});
        e.push_back({x2, y1, y2, -1});
        ys.push_back(y1);
        ys.push_back(y2);
    }
    if (e.empty()) return 0;
    sort(e.begin(), e.end());
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    int n = (int)ys.size() - 1;
    vector<int> cnt(4 * n + 4);
    vector<ll> len(4 * n + 4);
    auto add = [&](auto &&self, int p, int l, int r,
                   int ql, int qr, int v) -> void
    {
        if (ql <= l && r <= qr) cnt[p] += v;
        else
        {
            int mid = (l + r) / 2;
            if (ql <= mid) self(self, p * 2, l, mid, ql, qr, v);
            if (qr > mid) self(self, p * 2 + 1, mid + 1, r, ql, qr, v);
        }
        if (cnt[p]) len[p] = ys[r + 1] - ys[l];
        else len[p] = l == r ? 0 : len[p * 2] + len[p * 2 + 1];
    };
    __int128_t ans = 0;
    ll pre = get<0>(e[0]);
    for (auto [x, y1, y2, v] : e)
    {
        ans += (__int128_t)(x - pre) * len[1];
        int l = lower_bound(ys.begin(), ys.end(), y1) - ys.begin();
        int r = lower_bound(ys.begin(), ys.end(), y2) - ys.begin() - 1;
        add(add, 1, 0, n - 1, l, r, v);
        pre = x;
    }
    return ans;
}
