#pragma once
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

template <class T> bool segment_degenerate(const Segment<T> &s)
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

template <class T> bool collinear(Point<T> a, Point<T> b, Point<T> c)
{
    return orient(a, b, c) == 0;
}

template <class T> bool on_line(Point<T> q, const Line<T> &l)
{
    return !line_degenerate(l) && sgn(line_cross(l, q)) == 0;
}

// pre: l 非退化
template <class T> int side(const Line<T> &l, Point<T> q)
{
    return sgn(line_cross(l, q));
}

template <class T> bool same_side_strict(const Line<T> &l, Point<T> a, Point<T> b)
{
    int x = side(l, a), y = side(l, b);
    return x != 0 && x == y;
}

template <class T> bool opposite_side_strict(const Line<T> &l, Point<T> a, Point<T> b)
{
    return side(l, a) * side(l, b) < 0;
}

// 含端点；退化线段也支持
template <class T> bool on_segment(Point<T> p, const Segment<T> &s)
{
    auto between = [](T x, T a, T b)
    {
        if constexpr (is_integral_v<T>)
            return min(a, b) <= x && x <= max(a, b);
        else
            return min(a, b) - EPS <= x && x <= max(a, b) + EPS;
    };
    return orient(s.a, s.b, p) == 0 &&
           between(p.x, s.a.x, s.b.x) && between(p.y, s.a.y, s.b.y);
}

template <class T> bool on_segment_strict(Point<T> p, const Segment<T> &s)
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
template <class T> LineRelation line_relation(const Line<T> &a, const Line<T> &b)
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

template <class T> SegmentRelation segment_relation(const Segment<T> &s,
                                                    const Segment<T> &t)
{
    using R = SegmentRelation;
    if (segment_degenerate(s) && segment_degenerate(t))
        return same_point(s.a, t.a) ? R::TOUCH : R::DISJOINT;
    if (segment_degenerate(s))
        return on_segment(s.a, t) ? R::TOUCH : R::DISJOINT;
    if (segment_degenerate(t))
        return on_segment(t.a, s) ? R::TOUCH : R::DISJOINT;

    int a = orient(s.a, s.b, t.a), b = orient(s.a, s.b, t.b);
    int c = orient(t.a, t.b, s.a), d = orient(t.a, t.b, s.b);
    if (a * b < 0 && c * d < 0)
        return R::PROPER;

    if (a == 0 && b == 0 && c == 0 && d == 0)
    {
        bool use_x;
        if constexpr (is_integral_v<T>)
            use_x = abs128((i128)s.a.x - s.b.x) >= abs128((i128)s.a.y - s.b.y);
        else
            use_x = fabs(s.a.x - s.b.x) >= fabs(s.a.y - s.b.y);
        T s1 = use_x ? s.a.x : s.a.y, s2 = use_x ? s.b.x : s.b.y;
        T t1 = use_x ? t.a.x : t.a.y, t2 = use_x ? t.b.x : t.b.y;
        T l = max(min(s1, s2), min(t1, t2));
        T r = min(max(s1, s2), max(t1, t2));
        int z;
        if constexpr (is_integral_v<T>)
            z = (r > l) - (r < l); // 不做可能溢出的 r-l。
        else
            z = sgn(r - l);
        return z > 0 ? R::OVERLAP : z == 0 ? R::TOUCH : R::DISJOINT;
    }

    if ((a == 0 && on_segment(t.a, s)) || (b == 0 && on_segment(t.b, s)) ||
        (c == 0 && on_segment(s.a, t)) || (d == 0 && on_segment(s.b, t)))
        return R::TOUCH;
    return R::DISJOINT;
}

template <class T> bool segment_intersect(const Segment<T> &a, const Segment<T> &b)
{
    return segment_relation(a, b) != SegmentRelation::DISJOINT;
}

template <class T> bool proper_segment_intersect(const Segment<T> &a, const Segment<T> &b)
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
