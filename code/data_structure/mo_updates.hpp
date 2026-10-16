#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct Query
{
    int l, r, t, id;
};

struct Change
{
    int pos, pre, now;
};

template <class Add, class Del>
vector<ll> mo_updates(vector<int> a, const vector<Change> &chg,
                      vector<Query> q, ll &now, Add add_value,
                      Del del_value)
{
    int n = (int)a.size() - 1;
    int B = max(1, (int)cbrt(1LL * n * n));

    sort(q.begin(), q.end(),
         [&](const Query &A, const Query &B_)
         {
             int al = (A.l - 1) / B;
             int bl = (B_.l - 1) / B;

             if (al != bl)
             {
                 return al < bl;
             }

             int ar = (A.r - 1) / B;
             int br = (B_.r - 1) / B;

             if (ar != br)
             {
                 if (al & 1)
                 {
                     return ar > br;
                 }
                 return ar < br;
             }

             // 时间维奇偶优化
             if (ar & 1)
             {
                 return A.t > B_.t;
             }

             return A.t < B_.t;
         });

    vector<int> cur = a;
    auto add = [&](int pos) { add_value(cur[pos]); };
    auto del = [&](int pos) { del_value(cur[pos]); };

    int L = 1, R = 0, T = 0;

    auto apply = [&](int t)
    {
        int pos = chg[t].pos;

        if (L <= pos && pos <= R)
        {
            del(pos);
            cur[pos] = chg[t].now;
            add(pos);
        }
        else
        {
            cur[pos] = chg[t].now;
        }
    };

    auto undo = [&](int t)
    {
        int pos = chg[t].pos;

        if (L <= pos && pos <= R)
        {
            del(pos);
            cur[pos] = chg[t].pre;
            add(pos);
        }
        else
        {
            cur[pos] = chg[t].pre;
        }
    };

    vector<ll> ans(q.size());

    for (auto [l, r, t, id] : q)
    {
        while (L > l)
        {
            add(--L);
        }
        while (R < r)
        {
            add(++R);
        }
        while (L < l)
        {
            del(L++);
        }
        while (R > r)
        {
            del(R--);
        }

        while (T < t)
        {
            apply(++T);
        }
        while (T > t)
        {
            undo(T--);
        }

        ans[id] = now;
    }

    return ans;
}
