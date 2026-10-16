#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct Query
{
    int l, r, id;
};

struct Event
{
    int l, r, id, op;
};

template <class InitPre, class InsertPre, class QueryPre, class InitSuf,
          class InsertSuf, class QuerySuf>
vector<ll> mo_second_offline(int n, vector<Query> q, const vector<ll> &pre,
                             const vector<ll> &suf, InitPre init_pre,
                             InsertPre insert_pre, QueryPre query_pre,
                             InitSuf init_suf, InsertSuf insert_suf,
                             QuerySuf query_suf)
{
    int m = (int)q.size();
    int B = max(1, n / max(1, (int)sqrt(m)));

    sort(q.begin(), q.end(),
         [&](const Query &A, const Query &B_)
         {
             int ba = (A.l - 1) / B;
             int bb = (B_.l - 1) / B;

             if (ba != bb)
             {
                 return ba < bb;
             }

             // 奇偶块优化
             if (ba & 1)
             {
                 return A.r > B_.r;
             }

             return A.r < B_.r;
         });

    vector<vector<Event>> pre_event(n + 1);
    vector<vector<Event>> suf_event(n + 2);

    // delta[id]：莫队排序后第 id 个询问相对前一个询问的答案变化
    vector<ll> delta(m);

    int L = 1, R = 0;

    for (int id = 0; id < m; ++id)
    {
        int l = q[id].l;
        int r = q[id].r;

        if (R < r)
        {
            delta[id] += pre[r] - pre[R];

            if (L > 1)
            {
                pre_event[L - 1].push_back({R + 1, r, id, -1});
            }
        }
        else if (R > r)
        {
            delta[id] -= pre[R] - pre[r];

            if (L > 1)
            {
                pre_event[L - 1].push_back({r + 1, R, id, 1});
            }
        }

        R = r;

        if (L > l)
        {
            delta[id] += suf[l] - suf[L];

            if (R < n)
            {
                suf_event[R + 1].push_back({l, L - 1, id, -1});
            }
        }
        else if (L < l)
        {
            delta[id] -= suf[L] - suf[l];

            if (R < n)
            {
                suf_event[R + 1].push_back({L, l - 1, id, 1});
            }
        }

        L = l;
    }

    /*
        前缀第二次离线：

        扫描到 x 时维护
        query_pre(i) = sum_{j <= x} w(j,i)
    */

    init_pre();

    for (int x = 1; x <= n; ++x)
    {
        insert_pre(x);

        for (auto [l, r, id, op] : pre_event[x])
        {
            ll res = 0;

            for (int i = l; i <= r; ++i)
            {
                res += query_pre(i);
            }

            delta[id] += 1LL * op * res;
        }
    }

    /*
        后缀第二次离线：

        扫描到 x 时维护
        query_suf(i) = sum_{j >= x} w(i,j)
    */

    init_suf();

    for (int x = n; x >= 1; --x)
    {
        insert_suf(x);

        for (auto [l, r, id, op] : suf_event[x])
        {
            ll res = 0;

            for (int i = l; i <= r; ++i)
            {
                res += query_suf(i);
            }

            delta[id] += 1LL * op * res;
        }
    }

    vector<ll> ans(m);

    ll now = 0;

    for (int id = 0; id < m; ++id)
    {
        now += delta[id];
        ans[q[id].id] = now;
    }

    return ans;
}
