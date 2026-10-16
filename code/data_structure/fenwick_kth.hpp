#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct BITKth
{
    int n = 0;
    vector<int> tree;

    BITKth(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        tree.assign(n + 1, 0);
    }

    void add(int x, int v)
    {
        for (; x <= n; x += x & -x)
        {
            tree[x] += v;
        }
    }

    int sum(int x) const
    {
        int res = 0;

        for (; x; x -= x & -x)
        {
            res += tree[x];
        }

        return res;
    }

    int total() const
    {
        return sum(n);
    }

    int kth(int k) const
    {
        int pos = 0;
        int cur = 0;

        int step = 1;
        while (step <= n / 2)
        {
            step <<= 1;
        }

        for (; step; step >>= 1)
        {
            int nxt = pos + step;

            if (nxt <= n && cur + tree[nxt] < k)
            {
                pos = nxt;
                cur += tree[nxt];
            }
        }

        return pos + 1;
    }
};
