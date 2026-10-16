#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "line_segment.hpp"
#include "triangle.hpp"
struct Circle
{
    PointD o;
    db r;
};

enum class PointCircleRelation
{
    INSIDE,
    ON,
    OUTSIDE
};

inline PointCircleRelation point_circle_relation(PointD p, const Circle &c)
{
    int s = sgn(dist(p, c.o) - c.r);

    if (s < 0)
    {
        return PointCircleRelation::INSIDE;
    }

    if (s == 0)
    {
        return PointCircleRelation::ON;
    }

    return PointCircleRelation::OUTSIDE;
}

inline bool in_circle(PointD p, const Circle &c)
{
    return point_circle_relation(p, c) != PointCircleRelation::OUTSIDE;
}

enum class LineCircleRelation
{
    DISJOINT,
    TANGENT,
    SECANT
};

// pre: l 非退化
inline LineCircleRelation line_circle_relation(const LineD &l,
                                               const Circle &c)
{
    int s = sgn(distance_to_line(c.o, l) - c.r);

    if (s > 0)
    {
        return LineCircleRelation::DISJOINT;
    }

    if (s == 0)
    {
        return LineCircleRelation::TANGENT;
    }

    return LineCircleRelation::SECANT;
}

// 0 / 1 / 2 个交点
// pre: l 非退化
inline vector<PointD> line_circle_intersection(const LineD &l,
                                               const Circle &c)
{
    PointD h = projection(c.o, l);

    db h2 = c.r * c.r - dist2(h, c.o);

    int s = sgn(h2);

    if (s < 0)
    {
        return {};
    }

    if (s == 0)
    {
        return {h};
    }

    db d = sqrt(max<db>(0, h2));
    PointD e = unit(l.v);

    return {h - e * d, h + e * d};
}

enum class CircleRelation
{
    DISJOINT,
    EXTERNALLY_TANGENT,
    INTERSECT,
    INTERNALLY_TANGENT,
    CONTAINED,
    COINCIDENT
};

inline CircleRelation circle_relation(const Circle &a, const Circle &b)
{
    db d = dist(a.o, b.o);

    if (sgn(d) == 0 && sgn(a.r - b.r) == 0)
    {
        return CircleRelation::COINCIDENT;
    }

    db sum = a.r + b.r;
    db dif = fabs(a.r - b.r);

    if (sgn(d - sum) > 0)
    {
        return CircleRelation::DISJOINT;
    }

    if (sgn(d - sum) == 0)
    {
        return CircleRelation::EXTERNALLY_TANGENT;
    }

    if (sgn(d - dif) < 0)
    {
        return CircleRelation::CONTAINED;
    }

    if (sgn(d - dif) == 0)
    {
        return CircleRelation::INTERNALLY_TANGENT;
    }

    return CircleRelation::INTERSECT;
}

enum class CircleIntersectionType
{
    NONE,
    ONE,
    TWO,
    INFINITE
};

struct CircleIntersectionResult
{
    CircleIntersectionType type;
    vector<PointD> points;
};

inline CircleIntersectionResult circle_circle_intersection(const Circle &a,
                                                           const Circle &b)
{
    CircleRelation rel = circle_relation(a, b);

    if (rel == CircleRelation::DISJOINT || rel == CircleRelation::CONTAINED)
    {
        return {CircleIntersectionType::NONE, {}};
    }

    if (rel == CircleRelation::COINCIDENT)
    {
        if (sgn(a.r) == 0)
        {
            return {CircleIntersectionType::ONE, {a.o}};
        }

        return {CircleIntersectionType::INFINITE, {}};
    }

    PointD dvec = b.o - a.o;
    db d = len(dvec);

    db x = (d * d + a.r * a.r - b.r * b.r) / (2 * d);

    db h2 = a.r * a.r - x * x;

    PointD e = dvec / d;
    PointD base = a.o + e * x;

    if (sgn(h2) <= 0)
    {
        return {CircleIntersectionType::ONE, {base}};
    }

    PointD v = perp(e) * sqrt(max<db>(0, h2));

    return {CircleIntersectionType::TWO, {base + v, base - v}};
}

inline db circle_intersection_area(const Circle &a, const Circle &b)
{
    db r1 = a.r;
    db r2 = b.r;
    db d = dist(a.o, b.o);

    if (sgn(r1) == 0 || sgn(r2) == 0)
    {
        return 0;
    }

    if (sgn(d) == 0)
    {
        db r = min(r1, r2);
        return PI * r * r;
    }

    if (sgn(d - r1 - r2) >= 0)
    {
        return 0;
    }

    if (sgn(d - fabs(r1 - r2)) <= 0)
    {
        db r = min(r1, r2);
        return PI * r * r;
    }

    db ca = (r1 * r1 + d * d - r2 * r2) / (2 * r1 * d);

    db cb = (r2 * r2 + d * d - r1 * r1) / (2 * r2 * d);

    ca = clamp(ca, -1.0, 1.0);
    cb = clamp(cb, -1.0, 1.0);

    db A = 2 * acos(ca);
    db B = 2 * acos(cb);

    return 0.5 * r1 * r1 * (A - sin(A)) + 0.5 * r2 * r2 * (B - sin(B));
}

inline optional<Circle> circumcircle(PointD a, PointD b, PointD c)
{
    auto o = circumcenter(a, b, c);

    if (!o)
    {
        return nullopt;
    }

    return Circle{*o, dist(*o, a)};
}
