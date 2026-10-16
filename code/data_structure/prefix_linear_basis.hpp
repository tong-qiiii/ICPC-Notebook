#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct LinearBasis
{
    static constexpr int LOG = 30;

    array<int, LOG + 1> p{};
    array<int, LOG + 1> pos{};

    void insert(int x, int id)
    {
        for (int i = LOG; i >= 0; --i)
        {
            if (!(x >> i & 1))
            {
                continue;
            }

            if (!p[i])
            {
                p[i] = x;
                pos[i] = id;
                return;
            }

            if (id > pos[i])
            {
                swap(x, p[i]);
                swap(id, pos[i]);
            }

            x ^= p[i];
        }
    }

    int query(int l) const
    {
        int res = 0;

        for (int i = LOG; i >= 0; --i)
        {
            if (pos[i] >= l)
            {
                res = max(res, res ^ p[i]);
            }
        }

        return res;
    }
};
