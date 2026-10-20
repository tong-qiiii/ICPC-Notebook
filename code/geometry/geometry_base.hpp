#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

using i64 = long long;
using i128 = __int128_t;
#ifndef db
#define db double
#endif

const db EPS = 1e-9;
const db PI = acos(-1.0);

inline int sgn(db x)
{
    return (x > EPS) - (x < -EPS);
}

inline int sgn(i128 x)
{
    return (x > 0) - (x < 0);
}

inline i128 abs128(i128 x)
{
    return x >= 0 ? x : -x;
}

template <class T> struct Point
{
    T x{}, y{};

    Point() = default;
    Point(T x_, T y_) : x(x_), y(y_)
    {
    }

    Point operator+(const Point &p) const
    {
        return {x + p.x, y + p.y};
    }

    Point operator-(const Point &p) const
    {
        return {x - p.x, y - p.y};
    }

    Point operator*(T k) const
    {
        return {x * k, y * k};
    }

    Point &operator+=(const Point &p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }

    Point &operator-=(const Point &p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    bool operator==(const Point &p) const
    {
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point &p) const
    {
        return !(*this == p);
    }

    bool operator<(const Point &p) const
    {
        return x != p.x ? x < p.x : y < p.y;
    }
};

template <class T> Point<T> operator*(T k, const Point<T> &p)
{
    return p * k;
}

using PointI = Point<i64>;
using PointD = Point<db>;

inline PointD operator/(PointD p, db k)
{
    return {p.x / k, p.y / k};
}

inline PointD toD(PointI p)
{
    return {(db)p.x, (db)p.y};
}

inline i128 dot(PointI a, PointI b)
{
    return (i128)a.x * b.x + (i128)a.y * b.y;
}

inline db dot(PointD a, PointD b)
{
    return a.x * b.x + a.y * b.y;
}

inline i128 cross(PointI a, PointI b)
{
    return (i128)a.x * b.y - (i128)a.y * b.x;
}

inline db cross(PointD a, PointD b)
{
    return a.x * b.y - a.y * b.x;
}

// Avoid long long subtraction before promotion.
inline i128 cross(PointI a, PointI b, PointI c)
{
    return ((i128)b.x - a.x) * ((i128)c.y - a.y) -
           ((i128)b.y - a.y) * ((i128)c.x - a.x);
}

inline db cross(PointD a, PointD b, PointD c)
{
    return cross(b - a, c - a);
}

template <class T> auto len2(Point<T> a)
{
    return dot(a, a);
}

template <class T> auto norm(Point<T> a)
{
    return len2(a);
}

inline i128 dist2(PointI a, PointI b)
{
    i128 dx = (i128)a.x - b.x;
    i128 dy = (i128)a.y - b.y;
    return dx * dx + dy * dy;
}

inline db dist2(PointD a, PointD b)
{
    return len2(a - b);
}

inline long double len(PointI a)
{
    return sqrt((long double)len2(a));
}

inline db len(PointD a)
{
    return sqrt(len2(a));
}

inline long double dist(PointI a, PointI b)
{
    return sqrt((long double)dist2(a, b));
}

inline db dist(PointD a, PointD b)
{
    return sqrt(dist2(a, b));
}

template <class T> Point<T> perp(Point<T> a)
{
    return {-a.y, a.x};
}

template <class T> bool same_point(Point<T> a, Point<T> b)
{
    return a == b;
}

inline bool same_point(PointD a, PointD b)
{
    return sgn(a.x - b.x) == 0 && sgn(a.y - b.y) == 0;
}

// pre: |a| > 0
inline PointD unit(PointD a)
{
    return a / len(a);
}

inline PointD rotate(PointD a, db rad)
{
    db c = cos(rad), s = sin(rad);
    return {a.x * c - a.y * s, a.x * s + a.y * c};
}

// pre: a,b != 0
inline db angle(PointD a, PointD b)
{
    return atan2(fabs(cross(a, b)), dot(a, b));
}

template <class T> auto orient(Point<T> a, Point<T> b, Point<T> c)
{
    return sgn(cross(a, b, c));
}
