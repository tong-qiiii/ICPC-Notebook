#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "line_segment.hpp"
inline i128 polygon_area2(const vector<PointI> &p)
{
    i128 s = 0;
    int n = (int)p.size();

    for (int i = 0; i < n; ++i)
    {
        s += cross(p[i], p[(i + 1) % n]);
    }

    return s;
}

inline db polygon_area2(const vector<PointD> &p)
{
    db s = 0;
    int n = (int)p.size();

    for (int i = 0; i < n; ++i)
    {
        s += cross(p[i], p[(i + 1) % n]);
    }

    return s;
}

inline long double polygon_area(const vector<PointI> &p)
{
    return (long double)abs128(polygon_area2(p)) / 2;
}

inline db polygon_area(const vector<PointD> &p)
{
    return fabs(polygon_area2(p)) / 2;
}

enum class PolygonDir
{
    CW = -1,
    DEGENERATE = 0,
    CCW = 1
};

inline PolygonDir polygon_direction(const vector<PointI> &p)
{
    return static_cast<PolygonDir>(sgn(polygon_area2(p)));
}

inline PolygonDir polygon_direction(const vector<PointD> &p)
{
    return static_cast<PolygonDir>(sgn(polygon_area2(p)));
}

enum class PointInPolygon
{
    OUT,
    ON,
    IN
};

inline PointInPolygon point_in_polygon(const vector<PointI> &p, PointI q)
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

    int winding = 0;

    for (int i = 0; i < n; ++i)
    {
        PointI a = p[i];
        PointI b = p[(i + 1) % n];

        if (on_segment(q, SegmentI{a, b}))
        {
            return PointInPolygon::ON;
        }

        // Half-open y interval avoids counting vertices twice.
        if (a.y <= q.y && q.y < b.y && orient(a, b, q) > 0)
        {
            ++winding;
        }

        if (b.y <= q.y && q.y < a.y && orient(a, b, q) < 0)
        {
            --winding;
        }
    }

    return winding ? PointInPolygon::IN : PointInPolygon::OUT;
}

inline PointInPolygon point_in_polygon(const vector<PointD> &p, PointD q)
{
    int n = (int)p.size();

    if (n == 0)
    {
        return PointInPolygon::OUT;
    }

    if (n == 1)
    {
        return same_point(q, p[0]) ? PointInPolygon::ON
                                   : PointInPolygon::OUT;
    }

    if (n == 2)
    {
        return on_segment(q, SegmentD{p[0], p[1]}) ? PointInPolygon::ON
                                                   : PointInPolygon::OUT;
    }

    int winding = 0;

    for (int i = 0; i < n; ++i)
    {
        PointD a = p[i];
        PointD b = p[(i + 1) % n];

        if (on_segment(q, SegmentD{a, b}))
        {
            return PointInPolygon::ON;
        }

        int ya = sgn(a.y - q.y);
        int yb = sgn(b.y - q.y);

        if (ya <= 0 && yb > 0 && orient(a, b, q) > 0)
        {
            ++winding;
        }

        if (yb <= 0 && ya > 0 && orient(a, b, q) < 0)
        {
            --winding;
        }
    }

    return winding ? PointInPolygon::IN : PointInPolygon::OUT;
}

inline optional<PointD> polygon_centroid(const vector<PointD> &p)
{
    int n = (int)p.size();

    db s = 0;
    db sx = 0, sy = 0;

    for (int i = 0; i < n; ++i)
    {
        PointD a = p[i];
        PointD b = p[(i + 1) % n];

        db c = cross(a, b);

        s += c;
        sx += (a.x + b.x) * c;
        sy += (a.y + b.y) * c;
    }

    if (sgn(s) == 0)
    {
        return nullopt;
    }

    return PointD{sx / (3 * s), sy / (3 * s)};
}

inline optional<PointD> polygon_centroid(const vector<PointI> &p)
{
    int n = (int)p.size();

    i128 s = 0;
    long double sx = 0, sy = 0;

    for (int i = 0; i < n; ++i)
    {
        PointI a = p[i];
        PointI b = p[(i + 1) % n];

        i128 c = cross(a, b);

        s += c;
        sx += ((long double)a.x + b.x) * (long double)c;
        sy += ((long double)a.y + b.y) * (long double)c;
    }

    if (s == 0)
    {
        return nullopt;
    }

    return PointD{(db)(sx / (3 * (long double)s)),
                  (db)(sy / (3 * (long double)s))};
}
