#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "polygon.hpp"
struct HalfPlane
{
    LineD l;

    HalfPlane() = default;

    // 保留 a -> b 左侧
    HalfPlane(PointD a, PointD b) : l(LineD::through(a, b))
    {
    }

    explicit HalfPlane(LineD l_) : l(l_)
    {
    }

    // true <=> q 严格位于被删除的一侧
    bool outside(PointD q) const
    {
        return sgn(cross(l.v, q - l.p)) < 0;
    }
};

inline bool halfplane_parallel(const HalfPlane &a, const HalfPlane &b)
{
    return sgn(cross(a.l.v, b.l.v)) == 0;
}

inline bool halfplane_same_direction(const HalfPlane &a, const HalfPlane &b)
{
    return halfplane_parallel(a, b) && sgn(dot(a.l.v, b.l.v)) > 0;
}

// comparator 内不用 EPS，保证 strict weak ordering
inline int halfplane_half(PointD v)
{
    return v.y < 0 || (v.y == 0 && v.x < 0);
}

inline bool halfplane_angle_less(const HalfPlane &a, const HalfPlane &b)
{
    int A = halfplane_half(a.l.v);
    int B = halfplane_half(b.l.v);

    if (A != B)
    {
        return A < B;
    }

    db c = cross(a.l.v, b.l.v);

    if (c != 0)
    {
        return c > 0;
    }

    return false;
}

// pre:
//   所有直线非退化
//   若交集非空，则交集应有界
inline vector<PointD> halfplane_intersection(vector<HalfPlane> h)
{
    if (h.empty())
    {
        return {};
    }

    sort(h.begin(), h.end(), halfplane_angle_less);

    // 同方向只保留更严格的一条
    vector<HalfPlane> a;

    for (const HalfPlane &x : h)
    {
        if (!a.empty() && halfplane_same_direction(a.back(), x))
        {

            if (x.outside(a.back().l.p))
            {
                a.back() = x;
            }
        }
        else
        {
            a.push_back(x);
        }
    }

    deque<HalfPlane> q;

    for (const HalfPlane &x : a)
    {
        while (q.size() >= 2)
        {
            if (halfplane_parallel(q[q.size() - 2], q.back()))
            {
                return {};
            }

            PointD p = line_intersection(q[q.size() - 2].l, q.back().l);

            if (x.outside(p))
            {
                q.pop_back();
            }
            else
            {
                break;
            }
        }

        while (q.size() >= 2)
        {
            if (halfplane_parallel(q[0], q[1]))
            {
                return {};
            }

            PointD p = line_intersection(q[0].l, q[1].l);

            if (x.outside(p))
            {
                q.pop_front();
            }
            else
            {
                break;
            }
        }

        if (!q.empty() && halfplane_parallel(q.back(), x))
        {
            return {};
        }

        q.push_back(x);
    }

    while (q.size() >= 3)
    {
        PointD p = line_intersection(q[q.size() - 2].l, q.back().l);

        if (q.front().outside(p))
        {
            q.pop_back();
        }
        else
        {
            break;
        }
    }

    while (q.size() >= 3)
    {
        PointD p = line_intersection(q[0].l, q[1].l);

        if (q.back().outside(p))
        {
            q.pop_front();
        }
        else
        {
            break;
        }
    }

    if (q.size() < 3)
    {
        return {};
    }

    vector<PointD> poly;

    for (int i = 0; i < (int)q.size(); ++i)
    {
        const HalfPlane &A = q[i];
        const HalfPlane &B = q[(i + 1) % q.size()];

        if (halfplane_parallel(A, B))
        {
            return {};
        }

        poly.push_back(line_intersection(A.l, B.l));
    }

    db area2 = polygon_area2(poly);

    if (sgn(area2) == 0)
    {
        return {};
    }

    if (sgn(area2) < 0)
    {
        reverse(poly.begin(), poly.end());
    }

    return poly;
}
