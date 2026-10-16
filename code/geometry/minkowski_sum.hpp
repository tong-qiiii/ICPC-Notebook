#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "convex_hull.hpp"
inline int polar_half(PointI v)
{
    return v.y < 0 || (v.y == 0 && v.x < 0);
}

// -1: a 极角更小
//  0: 同方向
// +1: b 极角更小
inline int polar_compare(PointI a, PointI b)
{
    int A = polar_half(a);
    int B = polar_half(b);

    if (A != B)
    {
        return A < B ? -1 : 1;
    }

    i128 c = cross(a, b);

    if (c > 0)
    {
        return -1;
    }

    if (c < 0)
    {
        return 1;
    }

    return 0;
}

inline void minkowski_rotate_start(vector<PointI> &p)
{
    if (p.empty())
    {
        return;
    }

    int s = 0;

    for (int i = 1; i < (int)p.size(); ++i)
    {
        if (p[i].y < p[s].y || (p[i].y == p[s].y && p[i].x < p[s].x))
        {
            s = i;
        }
    }

    rotate(p.begin(), p.begin() + s, p.end());
}

inline vector<PointI> minkowski_sum(vector<PointI> a, vector<PointI> b)
{
    if (a.empty() || b.empty())
    {
        return {};
    }

    if (a.size() == 1)
    {
        for (PointI &p : b)
        {
            p += a[0];
        }

        return b;
    }

    if (b.size() == 1)
    {
        for (PointI &p : a)
        {
            p += b[0];
        }

        return a;
    }

    minkowski_rotate_start(a);
    minkowski_rotate_start(b);

    int n = (int)a.size();
    int m = (int)b.size();

    vector<PointI> ea(n), eb(m);

    for (int i = 0; i < n; ++i)
    {
        ea[i] = a[(i + 1) % n] - a[i];
    }

    for (int i = 0; i < m; ++i)
    {
        eb[i] = b[(i + 1) % m] - b[i];
    }

    vector<PointI> res;
    res.push_back(a[0] + b[0]);

    int i = 0, j = 0;

    while (i < n || j < m)
    {
        PointI e;

        if (i == n)
        {
            e = eb[j++];
        }
        else if (j == m)
        {
            e = ea[i++];
        }
        else
        {
            int cmp = polar_compare(ea[i], eb[j]);

            if (cmp < 0)
            {
                e = ea[i++];
            }
            else if (cmp > 0)
            {
                e = eb[j++];
            }
            else
            {
                e = ea[i++] + eb[j++];
            }
        }

        res.push_back(res.back() + e);
    }

    // 最后一条边回到起点
    res.pop_back();

    return res;
}
