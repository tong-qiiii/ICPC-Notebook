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

template <class AddRight, class AddLeft, class Undo, class Clear>
vector<ll> mo_rollback_add(int n, vector<Query> q, ll &now,
                           AddRight add_right, AddLeft add_left,
                           Undo rollback_left, Clear clear)
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

             return A.r < B_.r;
         });

    // 永久右扩，不需要记录回滚信息

    // 临时左扩，需要支持回滚

    vector<ll> ans(m);

    int last_block = -1;
    int R = 0;

    for (auto [l, r, id] : q)
    {
        int b = (l - 1) / B;
        int block_r = min(n, (b + 1) * B);

        if (b != last_block)
        {
            last_block = b;

            clear();
            R = block_r;
        }

        // 完全位于左端点块内：直接暴力
        if (r <= block_r)
        {
            ll backup = now;

            for (int i = l; i <= r; ++i)
            {
                add_left(i);
            }

            ans[id] = now;

            for (int i = r; i >= l; --i)
            {
                rollback_left(i);
            }

            now = backup;
            continue;
        }

        // 右侧永久状态，只增不退
        while (R < r)
        {
            add_right(++R);
        }

        ll backup = now;

        // 左侧临时加入
        for (int i = block_r; i >= l; --i)
        {
            add_left(i);
        }

        ans[id] = now;

        // 严格逆序撤销左扩
        for (int i = l; i <= block_r; ++i)
        {
            rollback_left(i);
        }

        now = backup;
    }

    return ans;
}
