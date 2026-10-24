#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// a,b 均为 1-index，等长非空，元素在 [0,2^62)；允许任意重排 b。
inline ll max_pair_and(const vector<ll> &a, const vector<ll> &b)
{
    assert(a.size() == b.size() && a.size() >= 2);
    struct Group
    {
        vector<ll> a, b;
    };
    vector<Group> groups(1);
    groups[0].a.assign(a.begin() + 1, a.end());
    groups[0].b.assign(b.begin() + 1, b.end());
    ll ans = 0;
    for (int bit = 61; bit >= 0; bit--)
    {
        bool ok = true;
        for (const auto &g : groups)
        {
            int zeros = 0, ones = 0;
            for (ll v : g.a)
            {
                zeros += !(v >> bit & 1);
            }
            for (ll v : g.b)
            {
                ones += v >> bit & 1;
            }
            if (zeros != ones)
            {
                ok = false;
            }
        }
        if (!ok)
        {
            continue;
        }
        ans |= 1LL << bit;
        vector<Group> next;
        for (const auto &g : groups)
        {
            Group z, o;
            for (ll v : g.a)
            {
                if (v >> bit & 1)
                {
                    o.a.push_back(v);
                }
                else
                {
                    z.a.push_back(v);
                }
            }
            for (ll v : g.b)
            {
                if (v >> bit & 1)
                {
                    z.b.push_back(v);
                }
                else
                {
                    o.b.push_back(v);
                }
            }
            if (!z.a.empty())
            {
                next.push_back(move(z));
            }
            if (!o.a.empty())
            {
                next.push_back(move(o));
            }
        }
        groups = move(next);
    }
    return ans;
}
