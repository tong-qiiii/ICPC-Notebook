#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "geometry_base.hpp"
struct ClosestPairResult
{
    PointI a, b;
    i128 d2;
};

inline optional<ClosestPairResult> closest_pair(vector<PointI> a)
{
    int n = (int)a.size();

    if (n < 2)
    {
        return nullopt;
    }

    sort(a.begin(), a.end());

    ClosestPairResult best{a[0], a[1], dist2(a[0], a[1])};

    vector<PointI> tmp(n);
    vector<PointI> strip;
    strip.reserve(n);

    auto cmp_y = [](PointI a, PointI b)
    { return a.y != b.y ? a.y < b.y : a.x < b.x; };

    auto update = [&](PointI x, PointI y)
    {
        i128 d = dist2(x, y);

        if (d < best.d2)
        {
            best = {x, y, d};
        }
    };

    function<void(int, int)> solve = [&](int l, int r)
    {
        // [l, r)
        if (r - l <= 3)
        {
            for (int i = l; i < r; ++i)
            {
                for (int j = i + 1; j < r; ++j)
                {
                    update(a[i], a[j]);
                }
            }

            sort(a.begin() + l, a.begin() + r, cmp_y);

            return;
        }

        int m = (l + r) / 2;

        // 递归会把区间改成按 y 排序，先保存分割线
        i64 mid_x = a[m].x;

        solve(l, m);
        solve(m, r);

        merge(a.begin() + l, a.begin() + m, a.begin() + m, a.begin() + r,
              tmp.begin() + l, cmp_y);

        copy(tmp.begin() + l, tmp.begin() + r, a.begin() + l);

        strip.clear();

        for (int i = l; i < r; ++i)
        {
            i128 dx = (i128)a[i].x - mid_x;

            if (dx * dx < best.d2)
            {
                strip.push_back(a[i]);
            }
        }

        // strip 已按 y 排序
        for (int i = 0; i < (int)strip.size(); ++i)
        {
            for (int j = i - 1; j >= 0; --j)
            {
                i128 dy = (i128)strip[i].y - strip[j].y;

                if (dy * dy >= best.d2)
                {
                    break;
                }

                update(strip[i], strip[j]);
            }
        }
    };

    solve(0, n);

    return best;
}
