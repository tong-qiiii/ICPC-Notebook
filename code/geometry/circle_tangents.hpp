#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "circle.hpp"
// pre: c.r > 0
inline vector<PointD> tangent_points_from_point(PointD p, const Circle &c)
{
    PointD v = p - c.o;

    db d2 = len2(v);
    db r2 = c.r * c.r;

    int s = sgn(d2 - r2);

    if (s < 0)
    {
        return {};
    }

    if (s == 0)
    {
        return {p};
    }

    PointD base = c.o + v * (r2 / d2);

    db h = c.r * sqrt(max<db>(0, d2 - r2)) / d2;

    PointD off = perp(v) * h;

    return {base + off, base - off};
}

// pre: c.r > 0
inline vector<LineD> tangents_from_point(PointD p, const Circle &c)
{
    auto tp = tangent_points_from_point(p, c);

    if (tp.empty())
    {
        return {};
    }

    if (tp.size() == 1)
    {
        return {LineD{p, perp(p - c.o)}};
    }

    return {LineD::through(p, tp[0]), LineD::through(p, tp[1])};
}

struct CommonTangent
{
    LineD line;
    PointD a;
    PointD b;
};

struct CommonTangentsResult
{
    bool infinite = false;
    vector<CommonTangent> tangents;
};

// pre: a.r > 0 && b.r > 0
inline CommonTangentsResult common_tangents(const Circle &a,
                                            const Circle &b)
{
    CommonTangentsResult res;

    PointD d = b.o - a.o;
    db z = len2(d);

    if (sgn(z) == 0)
    {
        if (sgn(a.r - b.r) == 0)
        {
            res.infinite = true;
        }

        return res;
    }

    // s = +1: 外公切线
    // s = -1: 内公切线
    for (int s : {-1, 1})
    {
        db r = a.r - s * b.r;
        db h2 = z - r * r;

        if (sgn(h2) < 0)
        {
            continue;
        }

        db h = sqrt(max<db>(0, h2));

        for (int t : {-1, 1})
        {
            // h = 0 时两种符号是同一条线
            if (t == -1 && sgn(h) == 0)
            {
                continue;
            }

            PointD n = (d * r + perp(d) * (h * t)) / z;

            PointD p1 = a.o + n * a.r;

            PointD p2 = b.o + n * (s * b.r);

            res.tangents.push_back({LineD{p1, perp(n)}, p1, p2});
        }
    }

    return res;
}
