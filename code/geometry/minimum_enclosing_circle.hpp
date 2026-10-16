#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "circle.hpp"
inline Circle circle_from_diameter(PointD a, PointD b)
{
    PointD o = (a + b) / 2.0;

    return {o, dist(a, b) / 2};
}

// 三个点的真正最小覆盖圆
inline Circle circle_from_three_mec(PointD a, PointD b, PointD c)
{
    array<Circle, 3> cand{circle_from_diameter(a, b),
                          circle_from_diameter(a, c),
                          circle_from_diameter(b, c)};

    bool found = false;
    Circle ans{};

    for (const Circle &x : cand)
    {
        if (!in_circle(a, x) || !in_circle(b, x) || !in_circle(c, x))
        {
            continue;
        }

        if (!found || x.r < ans.r)
        {
            ans = x;
            found = true;
        }
    }

    if (found)
    {
        return ans;
    }

    auto cc = circumcircle(a, b, c);

    if (cc)
    {
        return *cc;
    }

    // EPS 下近似共线时的兜底
    return *max_element(cand.begin(), cand.end(),
                        [](const Circle &x, const Circle &y)
                        { return x.r < y.r; });
}

inline Circle minimum_enclosing_circle(vector<PointD> p)
{
    if (p.empty())
    {
        return {{0, 0}, 0};
    }

    static mt19937 rng(
        chrono::steady_clock::now().time_since_epoch().count());

    shuffle(p.begin(), p.end(), rng);

    Circle c{p[0], 0};

    for (int i = 0; i < (int)p.size(); ++i)
    {
        if (in_circle(p[i], c))
        {
            continue;
        }

        c = {p[i], 0};

        for (int j = 0; j < i; ++j)
        {
            if (in_circle(p[j], c))
            {
                continue;
            }

            c = circle_from_diameter(p[i], p[j]);

            for (int k = 0; k < j; ++k)
            {
                if (in_circle(p[k], c))
                {
                    continue;
                }

                c = circle_from_three_mec(p[i], p[j], p[k]);
            }
        }
    }

    return c;
}
