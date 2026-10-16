#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "geometry_base.hpp"
struct Point3
{
    db x{}, y{}, z{};

    Point3() = default;

    Point3(db x_, db y_, db z_) : x(x_), y(y_), z(z_)
    {
    }

    Point3 operator+(const Point3 &p) const
    {
        return {x + p.x, y + p.y, z + p.z};
    }

    Point3 operator-(const Point3 &p) const
    {
        return {x - p.x, y - p.y, z - p.z};
    }

    Point3 operator*(db k) const
    {
        return {x * k, y * k, z * k};
    }

    Point3 operator/(db k) const
    {
        return {x / k, y / k, z / k};
    }

    Point3 &operator+=(const Point3 &p)
    {
        x += p.x;
        y += p.y;
        z += p.z;
        return *this;
    }

    Point3 &operator-=(const Point3 &p)
    {
        x -= p.x;
        y -= p.y;
        z -= p.z;
        return *this;
    }
};

inline Point3 operator*(db k, Point3 p)
{
    return p * k;
}

inline db dot(Point3 a, Point3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Point3 cross(Point3 a, Point3 b)
{
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}

inline db len2(Point3 a)
{
    return dot(a, a);
}

inline db len(Point3 a)
{
    return sqrt(len2(a));
}

inline db dist(Point3 a, Point3 b)
{
    return len(a - b);
}

// pre: a 非零
inline Point3 unit(Point3 a)
{
    return a / len(a);
}

// [0, pi]
// pre: a,b 非零
inline db angle(Point3 a, Point3 b)
{
    return atan2(len(cross(a, b)), dot(a, b));
}

struct Line3
{
    Point3 p;
    Point3 v; // p + t*v

    Line3() = default;

    Line3(Point3 p_, Point3 v_) : p(p_), v(v_)
    {
    }

    static Line3 through(Point3 a, Point3 b)
    {
        return {a, b - a};
    }
};

struct Plane
{
    Point3 p;
    Point3 n;

    Plane() = default;

    Plane(Point3 p_, Point3 n_) : p(p_), n(n_)
    {
    }
};

// pre: l.v 非零
inline Point3 projection(Point3 q, const Line3 &l)
{
    return l.p + l.v * (dot(q - l.p, l.v) / len2(l.v));
}

inline Point3 closest_point_on_line(Point3 q, const Line3 &l)
{
    return projection(q, l);
}

// pre: l.v 非零
inline db distance_to_line(Point3 q, const Line3 &l)
{
    return len(cross(q - l.p, l.v)) / len(l.v);
}

// pre: pl.n 非零
inline Point3 projection(Point3 q, const Plane &pl)
{
    return q - pl.n * (dot(q - pl.p, pl.n) / len2(pl.n));
}

// pre: pl.n 非零
inline db distance_to_plane(Point3 q, const Plane &pl)
{
    return fabs(dot(q - pl.p, pl.n)) / len(pl.n);
}

enum class LinePlaneRelation
{
    PARALLEL,
    CONTAINED,
    INTERSECT
};

inline LinePlaneRelation line_plane_relation(const Line3 &l,
                                             const Plane &pl)
{
    db d = dot(l.v, pl.n);

    if (sgn(d) != 0)
    {
        return LinePlaneRelation::INTERSECT;
    }

    if (sgn(dot(l.p - pl.p, pl.n)) == 0)
    {
        return LinePlaneRelation::CONTAINED;
    }

    return LinePlaneRelation::PARALLEL;
}

// 唯一交点；PARALLEL / CONTAINED -> nullopt
inline optional<Point3> line_plane_intersection(const Line3 &l,
                                                const Plane &pl)
{
    db d = dot(l.v, pl.n);

    if (sgn(d) == 0)
    {
        return nullopt;
    }

    db t = dot(pl.p - l.p, pl.n) / d;

    return l.p + l.v * t;
}

// [0, pi/2]
// pre: 两方向向量非零
inline db line_angle(const Line3 &a, const Line3 &b)
{
    return atan2(len(cross(a.v, b.v)), fabs(dot(a.v, b.v)));
}

// [0, pi/2]
// pre: 两法向量非零
inline db plane_angle(const Plane &a, const Plane &b)
{
    return atan2(len(cross(a.n, b.n)), fabs(dot(a.n, b.n)));
}

// 向量 v 绕过原点、方向 axis 的轴按右手定则旋转
// pre: axis 非零
inline Point3 rotate_around_axis(Point3 v, Point3 axis, db rad)
{
    Point3 u = unit(axis);

    db c = cos(rad);
    db s = sin(rad);

    return v * c + cross(u, v) * s + u * (dot(u, v) * (1 - c));
}
