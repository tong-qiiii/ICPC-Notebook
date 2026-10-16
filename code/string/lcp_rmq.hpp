#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "suffix_array.hpp"
struct LCPRMQ
{
    int n;
    string s;
    vector<int> sa, rk, height, lg;
    vector<vector<int>> st;

    LCPRMQ(const string &s_) : n(s_.size()), s(s_)
    {
        sa = buildSA(s);

        if (!n)
        {
            return;
        }

        rk.resize(n);
        height.resize(n);

        for (int i = 0; i < n; ++i)
        {
            rk[sa[i]] = i;
        }

        for (int i = 0, h = 0; i < n; ++i)
        {
            int r = rk[i];

            if (!r)
            {
                h = 0;
                continue;
            }

            int j = sa[r - 1];

            while (i + h < n && j + h < n && s[i + h] == s[j + h])
            {
                ++h;
            }

            height[r] = h;

            if (h)
            {
                --h;
            }
        }

        lg.resize(n + 1);

        for (int i = 2; i <= n; ++i)
        {
            lg[i] = lg[i >> 1] + 1;
        }

        int K = lg[n] + 1;

        st.assign(K, vector<int>(n));
        st[0] = height;

        for (int k = 1; k < K; ++k)
        {
            for (int i = 0; i + (1 << k) <= n; ++i)
            {
                st[k][i] = min(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
            }
        }
    }

    int query(int i, int j) const
    {
        if (i == j)
        {
            return n - i;
        }

        int l = rk[i], r = rk[j];

        if (l > r)
        {
            swap(l, r);
        }

        ++l;

        int len = r - l + 1;
        int k = lg[len];

        return min(st[k][l], st[k][r - (1 << k) + 1]);
    }
};
