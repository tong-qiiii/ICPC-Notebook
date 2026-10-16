#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct IntSet
{

    using i128 = __int128_t;

    set<pair<ll, ll>> seg;

    void clear()
    {
        seg.clear();
    }

    void add(ll l, ll r)
    {
        if (l > r)
        {
            swap(l, r);
        }

        auto it = seg.lower_bound({l, numeric_limits<ll>::lowest()});

        if (it != seg.begin())
        {
            auto pre = prev(it);

            if (touch(pre->second, l))
            {
                it = pre;
            }
        }

        while (it != seg.end() && touch(r, it->first))
        {
            l = min(l, it->first);
            r = max(r, it->second);
            it = seg.erase(it);
        }

        seg.insert({l, r});
    }

    void erase(ll l, ll r)
    {
        if (l > r)
        {
            swap(l, r);
        }

        auto it = seg.lower_bound({l, numeric_limits<ll>::lowest()});

        if (it != seg.begin())
        {
            auto pre = prev(it);

            if (pre->second >= l)
            {
                it = pre;
            }
        }

        vector<pair<ll, ll>> add_back;

        while (it != seg.end() && it->first <= r)
        {
            ll L = it->first;
            ll R = it->second;

            if (R < l)
            {
                ++it;
                continue;
            }

            it = seg.erase(it);

            if (L < l)
            {
                add_back.push_back({L, l - 1});
            }

            if (R > r)
            {
                add_back.push_back({r + 1, R});
            }
        }

        for (auto [L, R] : add_back)
        {
            seg.insert({L, R});
        }
    }

    bool contains(ll x) const
    {
        auto it = seg.upper_bound({x, numeric_limits<ll>::max()});

        if (it == seg.begin())
        {
            return false;
        }

        --it;
        return it->second >= x;
    }

  private:
    static bool touch(ll r1, ll l2)
    {
        return (i128)r1 + 1 >= l2;
    }
};
