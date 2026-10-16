#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline constexpr ll NEG = LLONG_MIN;

// true : 严格 <
// false: 允许 <=

struct Node
{
    ll x, y, z;
    ll w, dp;
    int id, rz;
};

struct BIT
{
    int n;
    vector<ll> t;
    vector<int> touched;

    void init(int n_)
    {
        n = n_;
        t.assign(n + 1, NEG);
        touched.clear();
    }

    void modify(int x, ll v)
    {
        for (; x <= n; x += x & -x)
        {
            if (t[x] == NEG)
            {
                touched.push_back(x);
            }
            t[x] = max(t[x], v);
        }
    }

    ll query(int x)
    {
        ll res = NEG;
        for (; x > 0; x -= x & -x)
        {
            res = max(res, t[x]);
        }
        return res;
    }

    void clear()
    {
        for (int x : touched)
        {
            t[x] = NEG;
        }
        touched.clear();
    }
};

template <bool STRICT_X = true, bool STRICT_Y = true, bool STRICT_Z = true>
struct CDQDominance
{

    int n;
    vector<Node> a;
    BIT bit;

    void trans(int l, int mid, int r)
    {
        vector<int> L, R;

        for (int i = l; i <= mid; ++i)
        {
            L.push_back(i);
        }

        for (int i = mid + 1; i <= r; ++i)
        {
            R.push_back(i);
        }

        auto cmp_y = [&](int i, int j)
        {
            if (a[i].y != a[j].y)
            {
                return a[i].y < a[j].y;
            }
            if (a[i].z != a[j].z)
            {
                return a[i].z < a[j].z;
            }
            return i < j;
        };

        sort(L.begin(), L.end(), cmp_y);
        sort(R.begin(), R.end(), cmp_y);

        int p = 0;

        for (int v : R)
        {
            // 第二维：
            // strict    -> y[left] <  y[right]
            // nonstrict -> y[left] <= y[right]
            while (p < (int)L.size() &&
                   (STRICT_Y ? a[L[p]].y < a[v].y : a[L[p]].y <= a[v].y))
            {
                bit.modify(a[L[p]].rz, a[L[p]].dp);
                ++p;
            }

            // 第三维：
            // strict    -> query(rz - 1)
            // nonstrict -> query(rz)
            int q = a[v].rz - (STRICT_Z ? 1 : 0);

            ll best = bit.query(q);

            if (best != NEG)
            {
                a[v].dp = max(a[v].dp, best + a[v].w);
            }
        }

        bit.clear();
    }

    void cdq(int l, int r)
    {
        if (l >= r)
        {
            return;
        }

        int mid = (l + r) >> 1;

        if constexpr (STRICT_X)
        {
            // 严格 x：相同 x 不能跨 CDQ 两侧。

            int rr = mid;

            while (rr < r && a[rr + 1].x == a[mid].x)
            {
                ++rr;
            }

            if (rr < r)
            {
                mid = rr;
            }
            else
            {
                int left = mid;

                while (left > l && a[left - 1].x == a[mid].x)
                {
                    --left;
                }

                // 整段 x 完全相同，不存在严格 x 转移
                if (left == l)
                {
                    return;
                }

                mid = left - 1;
            }
        }

        cdq(l, mid);

        // 左半部分 dp 已经求完，用它更新右半部分
        trans(l, mid, r);

        cdq(mid + 1, r);
    }

    ll work(vector<Node> src)
    {
        a = move(src);
        n = (int)a.size() - 1;
        if (n <= 0)
        {
            return 0;
        }
        for (int i = 1; i <= n; i++)
        {
            a[i].id = i;
        }
        // x 允许取等时，用 y,z,id 固定合法转移顺序。
        sort(a.begin() + 1, a.end(),
             [](const Node &A, const Node &B)
             {
                 if (A.x != B.x)
                 {
                     return A.x < B.x;
                 }
                 if (A.y != B.y)
                 {
                     return A.y < B.y;
                 }
                 if (A.z != B.z)
                 {
                     return A.z < B.z;
                 }
                 return A.id < B.id;
             });

        vector<ll> zs;

        for (int i = 1; i <= n; ++i)
        {
            zs.push_back(a[i].z);
        }

        sort(zs.begin(), zs.end());

        zs.erase(unique(zs.begin(), zs.end()), zs.end());

        for (int i = 1; i <= n; ++i)
        {
            a[i].rz =
                lower_bound(zs.begin(), zs.end(), a[i].z) - zs.begin() + 1;

            a[i].dp = a[i].w;
        }

        bit.init(zs.size());

        cdq(1, n);

        ll ans = NEG;

        for (int i = 1; i <= n; ++i)
        {
            ans = max(ans, a[i].dp);
        }

        return ans;
    }
};
