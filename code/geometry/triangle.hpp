#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "geometry_base.hpp"
inline i128 triangle_area2(PointI a, PointI b, PointI c)
{
    return cross(a, b, c);
}

inline db triangle_area2(PointD a, PointD b, PointD c)
{
    return cross(a, b, c);
}

inline long double triangle_area(PointI a, PointI b, PointI c)
{
    return (long double)abs128(cross(a, b, c)) / 2;
}

inline db triangle_area(PointD a, PointD b, PointD c)
{
    return fabs(cross(a, b, c)) / 2;
}

inline optional<PointD> circumcenter(PointD a, PointD b, PointD c)
{
    PointD u = b - a;
    PointD v = c - a;

    db d = 2 * cross(u, v);
    if (sgn(d) == 0)
    {
        return nullopt;
    }

    return a + (perp(u) * len2(v) - perp(v) * len2(u)) / d;
}

inline optional<PointD> incenter(PointD a, PointD b, PointD c)
{
    if (orient(a, b, c) == 0)
    {
        return nullopt;
    }

    db A = dist(b, c);
    db B = dist(c, a);
    db C = dist(a, b);

    return (a * A + b * B + c * C) / (A + B + C);
}

inline optional<PointD> orthocenter(PointD a, PointD b, PointD c)
{
    auto o = circumcenter(a, b, c);

    if (!o)
    {
        return nullopt;
    }

    return a + b + c - (*o) * 2.0;
}

inline optional<db> circumradius(PointD a, PointD b, PointD c)
{
    auto o = circumcenter(a, b, c);

    if (!o)
    {
        return nullopt;
    }

    return dist(*o, a);
}

inline optional<db> inradius(PointD a, PointD b, PointD c)
{
    db S = triangle_area(a, b, c);

    if (sgn(S) == 0)
    {
        return nullopt;
    }

    db s = (dist(a, b) + dist(b, c) + dist(c, a)) / 2;

    return S / s;
}
