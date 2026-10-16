#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "polygon.hpp"
#include "convex_hull.hpp"
struct DiameterResult
{
    PointI a, b;
    i128 d2;
};

inline optional<DiameterResult> convex_diameter(const vector<PointI> &p)
{
    int n = (int)p.size();

    if (n == 0)
    {
        return nullopt;
    }

    if (n == 1)
    {
        return DiameterResult{p[0], p[0], 0};
    }

    if (n == 2)
    {
        return DiameterResult{p[0], p[1], dist2(p[0], p[1])};
    }

    // 额外容错：直接传入多个共线点
    if (sgn(polygon_area2(p)) == 0)
    {
        auto [mn, mx] = minmax_element(p.begin(), p.end());

        return DiameterResult{*mn, *mx, dist2(*mn, *mx)};
    }

    DiameterResult ans{p[0], p[0], 0};

    auto update = [&](int i, int j)
    {
        i128 d = dist2(p[i], p[j]);

        if (d > ans.d2)
        {
            ans = {p[i], p[j], d};
        }
    };

    auto area = [&](int i, int j) -> i128
    { return abs128(cross(p[i], p[(i + 1) % n], p[j])); };

    int j = 1;

    for (int i = 0; i < n; ++i)
    {
        int ni = (i + 1) % n;

        while (area(i, (j + 1) % n) > area(i, j))
        {
            j = (j + 1) % n;
        }

        update(i, j);
        update(ni, j);

        // 平行支撑边：检查面积平台的另一个端点
        int nj = (j + 1) % n;

        if (area(i, nj) == area(i, j))
        {
            update(i, nj);
            update(ni, nj);
        }
    }

    return ans;
}
