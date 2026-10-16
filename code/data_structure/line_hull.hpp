#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct LineHull
{

    using i128 = __int128_t;

    static constexpr ll NEG_INF = std::numeric_limits<ll>::lowest();

    struct Line
    {
        ll k = 0;
        ll b = 0;

        i128 eval(ll x) const
        {
            return (i128)k * x + b;
        }
    };

    std::vector<Line> hull;
    int ptr = 0;

    void clear()
    {
        hull.clear();
        ptr = 0;
    }

    void add_line(ll k, ll b)
    {
        Line line{k, b};

        if (!hull.empty() && hull.back().k == k)
        {
            if (hull.back().b >= b)
            {
                return;
            }
            pop_back();
        }

        while ((int)hull.size() >= 2 &&
               bad(hull[(int)hull.size() - 2], hull.back(), line))
        {
            pop_back();
        }

        hull.push_back(line);
    }

    ll query(ll x) const
    {
        if (hull.empty())
        {
            return NEG_INF;
        }

        int l = 0, r = (int)hull.size() - 1;

        while (l < r)
        {
            int mid = (l + r) >> 1;

            if (hull[mid].eval(x) <= hull[mid + 1].eval(x))
            {
                l = mid + 1;
            }
            else
            {
                r = mid;
            }
        }

        return (ll)hull[l].eval(x);
    }

    ll query_mono(ll x)
    {
        if (hull.empty())
        {
            return NEG_INF;
        }

        ptr = std::min(ptr, (int)hull.size() - 1);

        while (ptr + 1 < (int)hull.size() &&
               hull[ptr + 1].eval(x) >= hull[ptr].eval(x))
        {
            ++ptr;
        }

        return (ll)hull[ptr].eval(x);
    }

  private:
    static bool bad(const Line &a, const Line &b, const Line &c)
    {
        return ((i128)a.b - b.b) * ((i128)c.k - b.k) >=
               ((i128)b.b - c.b) * ((i128)b.k - a.k);
    }

    void pop_back()
    {
        hull.pop_back();

        if (hull.empty())
        {
            ptr = 0;
        }
        else
        {
            ptr = std::min(ptr, (int)hull.size() - 1);
        }
    }
};
