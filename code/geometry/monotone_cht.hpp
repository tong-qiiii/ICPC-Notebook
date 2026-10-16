#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "geometry_base.hpp"
struct CHTLine
{
    i64 k, b;

    i128 eval(i64 x) const
    {
        return (i128)k * x + b;
    }
};

struct MonotoneCHTMax
{
    deque<CHTLine> q;

    // pre: a.k < b.k < c.k
    static bool bad(const CHTLine &a, const CHTLine &b, const CHTLine &c)
    {
        return ((i128)a.b - b.b) * ((i128)c.k - b.k) >=
               ((i128)b.b - c.b) * ((i128)b.k - a.k);
    }

    // pre: k 非递减
    void add_line(i64 k, i64 b)
    {
        CHTLine cur{k, b};

        // MAX：同斜率只留最大截距
        if (!q.empty() && q.back().k == k)
        {

            if (q.back().b >= b)
            {
                return;
            }

            q.pop_back();
        }

        while (q.size() >= 2 && bad(q[q.size() - 2], q.back(), cur))
        {
            q.pop_back();
        }

        q.push_back(cur);
    }

    // pre: q 非空，x 非递减
    i128 query(i64 x)
    {
        while (q.size() >= 2 && q[1].eval(x) >= q[0].eval(x))
        {
            q.pop_front();
        }

        return q.front().eval(x);
    }

    bool empty() const
    {
        return q.empty();
    }

    void clear()
    {
        q.clear();
    }
};
