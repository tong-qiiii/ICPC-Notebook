#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "polygon.hpp"
#include "convex_hull.hpp"
inline db min_area_bounding_rectangle(const vector<PointI> &p)
{
    int n = (int)p.size();

    if (n <= 2)
    {
        return 0;
    }

    if (sgn(polygon_area2(p)) == 0)
    {
        return 0;
    }

    // j: cross 最大
    // k: dot 最大
    // l: dot 最小
    int j = 0;
    int k = 0;
    int l = 0;

    // 第一条边先找到三个真实极值
    {
        int i = 0;
        int ni = 1;

        i128 ex = (i128)p[ni].x - p[i].x;
        i128 ey = (i128)p[ni].y - p[i].y;

        auto cr = [&](int t) -> i128 {
            return ex * ((i128)p[t].y - p[i].y) -
                   ey * ((i128)p[t].x - p[i].x);
        };

        auto pr = [&](int t) -> i128 {
            return ex * ((i128)p[t].x - p[i].x) +
                   ey * ((i128)p[t].y - p[i].y);
        };

        for (int t = 1; t < n; ++t)
        {
            if (cr(t) > cr(j))
            {
                j = t;
            }

            if (pr(t) > pr(k))
            {
                k = t;
            }

            if (pr(t) < pr(l))
            {
                l = t;
            }
        }
    }

    db ans = numeric_limits<db>::infinity();

    for (int i = 0; i < n; ++i)
    {
        int ni = (i + 1) % n;

        i128 ex = (i128)p[ni].x - p[i].x;
        i128 ey = (i128)p[ni].y - p[i].y;

        auto cr = [&](int t) -> i128 {
            return ex * ((i128)p[t].y - p[i].y) -
                   ey * ((i128)p[t].x - p[i].x);
        };

        auto pr = [&](int t) -> i128 {
            return ex * ((i128)p[t].x - p[i].x) +
                   ey * ((i128)p[t].y - p[i].y);
        };

        while (cr((j + 1) % n) > cr(j))
        {
            j = (j + 1) % n;
        }

        while (pr((k + 1) % n) > pr(k))
        {
            k = (k + 1) % n;
        }

        while (pr((l + 1) % n) < pr(l))
        {
            l = (l + 1) % n;
        }

        i128 height_num = cr(j);
        i128 width_num = pr(k) - pr(l);
        i128 e2 = ex * ex + ey * ey;

        db cur = (db)((long double)height_num * (long double)width_num /
                      (long double)e2);

        ans = min(ans, cur);
    }

    return ans;
}
