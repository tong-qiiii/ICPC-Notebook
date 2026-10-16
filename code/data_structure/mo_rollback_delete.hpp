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

template <class Init, class Del, class Undo>
vector<ll> mo_rollback_delete(int n, vector<Query> q, ll &now, Init init,
                              Del del, Undo rollback)
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

             // 同块 r 降序，右端点只向左删除
             return A.r > B_.r;
         });

    vector<ll> ans(m);

    init();

    int last_block = -1;

    int L = 1;
    int R = n;

    // 当前块基准状态 [block_l,n] 的答案
    ll block_now = now;

    for (auto [l, r, id] : q)
    {
        int b = (l - 1) / B;
        int block_l = b * B + 1;

        if (b != last_block)
        {
            if (last_block != -1)
            {
                // 恢复上一块右侧永久删除
                while (R < n)
                {
                    rollback(++R);
                }

                now = block_now;
            }

            last_block = b;

            // 从上一块基准直接推进到新块
            // L 整个算法过程中只向右
            while (L < block_l)
            {
                del(L++);
            }

            block_now = now;
            R = n;
        }

        // 同块 r 降序，右端点永久向左删除
        while (R > r)
        {
            del(R--);
        }

        ll backup = now;

        // 左端点临时删除
        while (L < l)
        {
            del(L++);
        }

        ans[id] = now;

        // 左端点严格逆序回滚
        while (L > block_l)
        {
            rollback(--L);
        }

        now = backup;
    }

    return ans;
}
