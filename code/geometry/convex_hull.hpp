#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "geometry_base.hpp"
inline vector<PointI> convex_hull(vector<PointI> p,
                                  bool keep_collinear = false)
{
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());

    int n = (int)p.size();
    if (n <= 2)
    {
        return p;
    }

    bool all_collinear = true;

    for (int i = 2; i < n; ++i)
    {
        if (orient(p[0], p[1], p[i]) != 0)
        {
            all_collinear = false;
            break;
        }
    }

    if (all_collinear)
    {
        if (keep_collinear)
        {
            return p;
        }

        return {p.front(), p.back()};
    }

    vector<PointI> lo, up;

    for (PointI x : p)
    {
        while ((int)lo.size() >= 2)
        {
            int o = orient(lo[(int)lo.size() - 2], lo.back(), x);

            if (keep_collinear ? o < 0 : o <= 0)
            {
                lo.pop_back();
            }
            else
            {
                break;
            }
        }

        lo.push_back(x);
    }

    for (int i = n - 1; i >= 0; --i)
    {
        PointI x = p[i];

        while ((int)up.size() >= 2)
        {
            int o = orient(up[(int)up.size() - 2], up.back(), x);

            if (keep_collinear ? o < 0 : o <= 0)
            {
                up.pop_back();
            }
            else
            {
                break;
            }
        }

        up.push_back(x);
    }

    lo.pop_back();
    up.pop_back();

    lo.insert(lo.end(), up.begin(), up.end());

    return lo;
}
