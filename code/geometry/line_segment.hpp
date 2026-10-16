#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "geometry_base.hpp"
template <class T> struct Line
{
    Point<T> p, v; // p + t * v

    Line() = default;
    Line(Point<T> p_, Point<T> v_) : p(p_), v(v_)
    {
    }

    static Line through(Point<T> a, Point<T> b)
    {
        return {a, b - a};
    }
};

template <class T> struct Segment
{
    Point<T> a, b;
};

using LineI = Line<i64>;
using LineD = Line<db>;
using SegmentI = Segment<i64>;
using SegmentD = Segment<db>;

inline LineD toD(const LineI &l)
{
    return {toD(l.p), toD(l.v)};
}

inline SegmentD toD(const SegmentI &s)
{
    return {toD(s.a), toD(s.b)};
}

inline bool line_degenerate(const LineI &l)
{
    return l.v == PointI{0, 0};
}

inline bool line_degenerate(const LineD &l)
{
    return len2(l.v) <= EPS * EPS;
}

inline bool segment_degenerate(const SegmentI &s)
{
    return s.a == s.b;
}

inline bool segment_degenerate(const SegmentD &s)
{
    return same_point(s.a, s.b);
}

// cross(v, q-p)
inline i128 line_cross(const LineI &l, PointI q)
{
    return (i128)l.v.x * ((i128)q.y - l.p.y) -
           (i128)l.v.y * ((i128)q.x - l.p.x);
}

inline db line_cross(const LineD &l, PointD q)
{
    return cross(l.v, q - l.p);
}

inline bool collinear(PointI a, PointI b, PointI c)
{
    return orient(a, b, c) == 0;
}

inline bool collinear(PointD a, PointD b, PointD c)
{
    return orient(a, b, c) == 0;
}

inline bool on_line(PointI q, const LineI &l)
{
    return !line_degenerate(l) && sgn(line_cross(l, q)) == 0;
}

inline bool on_line(PointD q, const LineD &l)
{
    return !line_degenerate(l) && sgn(line_cross(l, q)) == 0;
}

// pre: l 非退化
inline int side(const LineI &l, PointI q)
{
    return sgn(line_cross(l, q));
}

inline int side(const LineD &l, PointD q)
{
    return sgn(line_cross(l, q));
}

inline bool same_side_strict(const LineI &l, PointI a, PointI b)
{
    int x = side(l, a), y = side(l, b);
    return x != 0 && x == y;
}

inline bool same_side_strict(const LineD &l, PointD a, PointD b)
{
    int x = side(l, a), y = side(l, b);
    return x != 0 && x == y;
}

inline bool opposite_side_strict(const LineI &l, PointI a, PointI b)
{
    return side(l, a) * side(l, b) < 0;
}

inline bool opposite_side_strict(const LineD &l, PointD a, PointD b)
{
    return side(l, a) * side(l, b) < 0;
}

// 含端点；退化线段也支持
inline bool on_segment(PointI p, const SegmentI &s)
{
    if (orient(s.a, s.b, p) != 0)
    {
        return false;
    }

    return min(s.a.x, s.b.x) <= p.x && p.x <= max(s.a.x, s.b.x) &&
           min(s.a.y, s.b.y) <= p.y && p.y <= max(s.a.y, s.b.y);
}

inline bool on_segment(PointD p, const SegmentD &s)
{
    if (orient(s.a, s.b, p) != 0)
    {
        return false;
    }

    return p.x >= min(s.a.x, s.b.x) - EPS &&
           p.x <= max(s.a.x, s.b.x) + EPS &&
           p.y >= min(s.a.y, s.b.y) - EPS && p.y <= max(s.a.y, s.b.y) + EPS;
}

inline bool on_segment_strict(PointI p, const SegmentI &s)
{
    return on_segment(p, s) && p != s.a && p != s.b;
}

inline bool on_segment_strict(PointD p, const SegmentD &s)
{
    return on_segment(p, s) && !same_point(p, s.a) && !same_point(p, s.b);
}

enum class LineRelation
{
    PARALLEL,
    COINCIDENT,
    INTERSECT
};

// pre: 两条直线均非退化
inline LineRelation line_relation(const LineI &a, const LineI &b)
{
    if (sgn(cross(a.v, b.v)) != 0)
    {
        return LineRelation::INTERSECT;
    }

    return sgn(line_cross(a, b.p)) == 0 ? LineRelation::COINCIDENT
                                        : LineRelation::PARALLEL;
}

inline LineRelation line_relation(const LineD &a, const LineD &b)
{
    if (sgn(cross(a.v, b.v)) != 0)
    {
        return LineRelation::INTERSECT;
    }

    return sgn(line_cross(a, b.p)) == 0 ? LineRelation::COINCIDENT
                                        : LineRelation::PARALLEL;
}

// pre: line_relation(a,b) == INTERSECT
inline PointD line_intersection(const LineD &a, const LineD &b)
{
    db t = cross(b.p - a.p, b.v) / cross(a.v, b.v);
    return a.p + a.v * t;
}

// 整数关系先精确判断，交点最终转浮点
// pre: line_relation(a,b) == INTERSECT
inline PointD line_intersection(const LineI &a, const LineI &b)
{
    long double den =
        (long double)a.v.x * b.v.y - (long double)a.v.y * b.v.x;

    long double num = ((long double)b.p.x - a.p.x) * b.v.y -
                      ((long double)b.p.y - a.p.y) * b.v.x;

    long double t = num / den;

    return {(db)((long double)a.p.x + (long double)a.v.x * t),
            (db)((long double)a.p.y + (long double)a.v.y * t)};
}

enum class SegmentRelation
{
    DISJOINT,
    TOUCH,
    PROPER,
    OVERLAP
};

inline SegmentRelation segment_relation(const SegmentI &s,
                                        const SegmentI &t)
{
    if (segment_degenerate(s) && segment_degenerate(t))
    {
        return s.a == t.a ? SegmentRelation::TOUCH
                          : SegmentRelation::DISJOINT;
    }

    if (segment_degenerate(s))
    {
        return on_segment(s.a, t) ? SegmentRelation::TOUCH
                                  : SegmentRelation::DISJOINT;
    }

    if (segment_degenerate(t))
    {
        return on_segment(t.a, s) ? SegmentRelation::TOUCH
                                  : SegmentRelation::DISJOINT;
    }

    int a = orient(s.a, s.b, t.a);
    int b = orient(s.a, s.b, t.b);
    int c = orient(t.a, t.b, s.a);
    int d = orient(t.a, t.b, s.b);

    if (a * b < 0 && c * d < 0)
    {
        return SegmentRelation::PROPER;
    }

    if (a == 0 && b == 0 && c == 0 && d == 0)
    {
        bool use_x =
            abs128((i128)s.a.x - s.b.x) >= abs128((i128)s.a.y - s.b.y);

        i64 s1 = use_x ? s.a.x : s.a.y;
        i64 s2 = use_x ? s.b.x : s.b.y;
        i64 t1 = use_x ? t.a.x : t.a.y;
        i64 t2 = use_x ? t.b.x : t.b.y;

        i64 L = max(min(s1, s2), min(t1, t2));
        i64 R = min(max(s1, s2), max(t1, t2));

        if (L < R)
        {
            return SegmentRelation::OVERLAP;
        }
        if (L == R)
        {
            return SegmentRelation::TOUCH;
        }
        return SegmentRelation::DISJOINT;
    }

    if ((a == 0 && on_segment(t.a, s)) || (b == 0 && on_segment(t.b, s)) ||
        (c == 0 && on_segment(s.a, t)) || (d == 0 && on_segment(s.b, t)))
    {
        return SegmentRelation::TOUCH;
    }

    return SegmentRelation::DISJOINT;
}

inline SegmentRelation segment_relation(const SegmentD &s,
                                        const SegmentD &t)
{
    if (segment_degenerate(s) && segment_degenerate(t))
    {
        return same_point(s.a, t.a) ? SegmentRelation::TOUCH
                                    : SegmentRelation::DISJOINT;
    }

    if (segment_degenerate(s))
    {
        return on_segment(s.a, t) ? SegmentRelation::TOUCH
                                  : SegmentRelation::DISJOINT;
    }

    if (segment_degenerate(t))
    {
        return on_segment(t.a, s) ? SegmentRelation::TOUCH
                                  : SegmentRelation::DISJOINT;
    }

    int a = orient(s.a, s.b, t.a);
    int b = orient(s.a, s.b, t.b);
    int c = orient(t.a, t.b, s.a);
    int d = orient(t.a, t.b, s.b);

    if (a * b < 0 && c * d < 0)
    {
        return SegmentRelation::PROPER;
    }

    if (a == 0 && b == 0 && c == 0 && d == 0)
    {
        bool use_x = fabs(s.a.x - s.b.x) >= fabs(s.a.y - s.b.y);

        db s1 = use_x ? s.a.x : s.a.y;
        db s2 = use_x ? s.b.x : s.b.y;
        db t1 = use_x ? t.a.x : t.a.y;
        db t2 = use_x ? t.b.x : t.b.y;

        db L = max(min(s1, s2), min(t1, t2));
        db R = min(max(s1, s2), max(t1, t2));

        int z = sgn(R - L);

        if (z > 0)
        {
            return SegmentRelation::OVERLAP;
        }
        if (z == 0)
        {
            return SegmentRelation::TOUCH;
        }
        return SegmentRelation::DISJOINT;
    }

    if ((a == 0 && on_segment(t.a, s)) || (b == 0 && on_segment(t.b, s)) ||
        (c == 0 && on_segment(s.a, t)) || (d == 0 && on_segment(s.b, t)))
    {
        return SegmentRelation::TOUCH;
    }

    return SegmentRelation::DISJOINT;
}

inline bool segment_intersect(const SegmentI &a, const SegmentI &b)
{
    return segment_relation(a, b) != SegmentRelation::DISJOINT;
}

inline bool segment_intersect(const SegmentD &a, const SegmentD &b)
{
    return segment_relation(a, b) != SegmentRelation::DISJOINT;
}

inline bool proper_segment_intersect(const SegmentI &a, const SegmentI &b)
{
    return segment_relation(a, b) == SegmentRelation::PROPER;
}

inline bool proper_segment_intersect(const SegmentD &a, const SegmentD &b)
{
    return segment_relation(a, b) == SegmentRelation::PROPER;
}

// 以下均为浮点构造；直线要求非退化

inline PointD projection(PointD q, const LineD &l)
{
    return l.p + l.v * (dot(q - l.p, l.v) / len2(l.v));
}

inline PointD reflection(PointD q, const LineD &l)
{
    return projection(q, l) * 2.0 - q;
}

inline db distance_to_line(PointD q, const LineD &l)
{
    return fabs(cross(l.v, q - l.p)) / len(l.v);
}

inline PointD closest_point_on_line(PointD q, const LineD &l)
{
    return projection(q, l);
}

inline PointD closest_point_on_segment(PointD q, const SegmentD &s)
{
    PointD v = s.b - s.a;

    if (len2(v) <= EPS * EPS)
    {
        return s.a;
    }

    db t = dot(q - s.a, v) / len2(v);
    t = clamp(t, 0.0, 1.0);

    return s.a + v * t;
}

inline db distance_to_segment(PointD q, const SegmentD &s)
{
    return dist(q, closest_point_on_segment(q, s));
}
