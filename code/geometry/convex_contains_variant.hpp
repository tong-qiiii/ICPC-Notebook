#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "polygon.hpp"
#include "convex_hull.hpp"
inline PointInPolygon point_in_convex_polygon(const vector<PointI> &p,
                                              PointI q)
{
    int n = (int)p.size();

    if (n == 0)
    {
        return PointInPolygon::OUT;
    }

    if (n == 1)
    {
        return q == p[0] ? PointInPolygon::ON : PointInPolygon::OUT;
    }

    if (n == 2)
    {
        return on_segment(q, SegmentI{p[0], p[1]}) ? PointInPolygon::ON
                                                   : PointInPolygon::OUT;
    }

    // convex_hull(..., true) 的全共线输出
    if (sgn(polygon_area2(p)) == 0)
    {
        return on_segment(q, SegmentI{p.front(), p.back()})
                   ? PointInPolygon::ON
                   : PointInPolygon::OUT;
    }

    // 找首射线上离 p[0] 最远的边界点
    int lo = 1, hi = n - 1;

    while (lo < hi)
    {
        int mid = (lo + hi + 1) / 2;

        if (orient(p[0], p[1], p[mid]) == 0)
        {
            lo = mid;
        }
        else
        {
            hi = mid - 1;
        }
    }

    int L = lo;

    // 找尾射线上离 p[0] 最远的边界点
    lo = 1;
    hi = n - 1;

    while (lo < hi)
    {
        int mid = (lo + hi) / 2;

        if (orient(p[0], p[n - 1], p[mid]) == 0)
        {
            hi = mid;
        }
        else
        {
            lo = mid + 1;
        }
    }

    int R = lo;

    int a = orient(p[0], p[L], q);
    int b = orient(p[0], p[R], q);

    if (a < 0 || b > 0)
    {
        return PointInPolygon::OUT;
    }

    if (a == 0)
    {
        return on_segment(q, SegmentI{p[0], p[L]}) ? PointInPolygon::ON
                                                   : PointInPolygon::OUT;
    }

    if (b == 0)
    {
        return on_segment(q, SegmentI{p[0], p[R]}) ? PointInPolygon::ON
                                                   : PointInPolygon::OUT;
    }

    int l = L, r = R;

    while (r - l > 1)
    {
        int mid = (l + r) / 2;

        if (orient(p[0], p[mid], q) >= 0)
        {
            l = mid;
        }
        else
        {
            r = mid;
        }
    }

    int o = orient(p[l], p[l + 1], q);

    if (o < 0)
    {
        return PointInPolygon::OUT;
    }

    if (o == 0)
    {
        return on_segment(q, SegmentI{p[l], p[l + 1]})
                   ? PointInPolygon::ON
                   : PointInPolygon::OUT;
    }

    return PointInPolygon::IN;
}
