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

template <class Add, class Del>
vector<ll> mo(int n, vector<Query> q, ll &now, Add add, Del del)
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

    vector<ll> ans(m);

    int L = 1, R = 0;

    for (auto [l, r, id] : q)
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

        ans[id] = now;
    }

    return ans;
}
