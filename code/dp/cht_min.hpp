#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include <bits/stdc++.h>
using namespace std;

using i128 = __int128_t;

struct CHTMin
{
    struct Line
    {
        ll k, b;
    };

    deque<Line> q;

    i128 eval(const Line &l, ll x)
    {
        return (i128)l.k * x + l.b;
    }

    bool bad(const Line &a, const Line &b, const Line &c)
    {
        return ((i128)b.b - a.b) * ((i128)b.k - c.k) >=
               ((i128)c.b - b.b) * ((i128)a.k - b.k);
    }

    void add(ll k, ll b)
    {
        Line c{k, b};

        while (!q.empty() && q.back().k == k)
        {
            if (q.back().b <= b)
            {
                return;
            }
            q.pop_back();
        }

        while (q.size() >= 2 && bad(q[q.size() - 2], q.back(), c))
        {
            q.pop_back();
        }
        q.push_back(c);
    }

    ll query(ll x)
    {
        while (q.size() >= 2 && eval(q[0], x) >= eval(q[1], x))
        {
            q.pop_front();
        }
        return (ll)eval(q[0], x);
    }
};
