#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "polygon.hpp"
inline i128 gcd128(i128 a, i128 b)
{
    a = abs128(a);
    b = abs128(b);

    while (b)
    {
        i128 t = a % b;
        a = b;
        b = t;
    }

    return a;
}

// 闭线段 [a,b]，包含两个端点
inline i128 lattice_points_on_segment(PointI a, PointI b)
{
    i128 dx = (i128)b.x - a.x;
    i128 dy = (i128)b.y - a.y;

    return gcd128(dx, dy) + 1;
}

inline i128 boundary_lattice_points(const vector<PointI> &p)
{
    i128 B = 0;
    int n = (int)p.size();

    for (int i = 0; i < n; ++i)
    {
        PointI a = p[i];
        PointI b = p[(i + 1) % n];

        B += gcd128((i128)b.x - a.x, (i128)b.y - a.y);
    }

    return B;
}

inline i128 interior_lattice_points(const vector<PointI> &p)
{
    i128 A2 = abs128(polygon_area2(p));
    i128 B = boundary_lattice_points(p);

    return (A2 - B + 2) / 2;
}
