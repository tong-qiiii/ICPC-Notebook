#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct IntSet
{
    struct Seg
    {
        int l = 0;
        int r = 0;
    };

    vector<Seg> seg;
    set<pair<int, int>> active; // {l, id}

    IntSet() = default;

    IntSet(const vector<Seg> &seg)
    {
        init(seg);
    }

    void init(const vector<Seg> &seg_)
    {
        seg = seg_;
        active.clear();
    }

    bool overlap(int l, int r)
    {
        auto it = first_cross(l);

        return it != active.end() && seg[it->second].l <= r;
    }

    bool insert(int id)
    {
        int l = seg[id].l;
        int r = seg[id].r;

        if (overlap(l, r))
        {
            return false;
        }

        active.insert({l, id});
        return true;
    }

    bool erase(int id)
    {
        auto it = active.find({seg[id].l, id});

        if (it == active.end())
        {
            return false;
        }

        active.erase(it);
        return true;
    }

    vector<int> ids(int l, int r)
    {
        vector<int> res;

        auto it = first_cross(l);

        while (it != active.end() && seg[it->second].l <= r)
        {
            res.push_back(it->second);
            ++it;
        }

        return res;
    }

    vector<int> erase_cross(int l, int r)
    {
        vector<int> res;

        auto it = first_cross(l);

        while (it != active.end() && seg[it->second].l <= r)
        {
            res.push_back(it->second);
            it = active.erase(it);
        }

        return res;
    }

  private:
    set<pair<int, int>>::iterator first_cross(int l)
    {
        auto it = active.lower_bound({l, INT_MIN});

        if (it != active.begin())
        {
            auto pre = prev(it);

            if (seg[pre->second].r >= l)
            {
                it = pre;
            }
        }

        return it;
    }
};
