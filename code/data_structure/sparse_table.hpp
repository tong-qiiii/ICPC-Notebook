#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct STMax
{
    int n = 0;
    vector<int> lg;
    vector<vector<int>> st;

    STMax() = default;

    STMax(const vector<int> &a)
    {
        build(a);
    }

    void build(const vector<int> &a)
    {
        n = (int)a.size() - 1;

        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; ++i)
        {
            lg[i] = lg[i >> 1] + 1;
        }

        int LOG = lg[n] + 1;
        st.assign(LOG, vector<int>(n + 1));

        for (int i = 1; i <= n; ++i)
        {
            st[0][i] = a[i];
        }

        for (int k = 1; k < LOG; ++k)
        {
            int len = 1 << k;
            int half = len >> 1;

            for (int i = 1; i + len - 1 <= n; ++i)
            {
                st[k][i] = max(st[k - 1][i], st[k - 1][i + half]);
            }
        }
    }

    int query(int l, int r) const
    {
        int k = lg[r - l + 1];
        return max(st[k][l], st[k][r - (1 << k) + 1]);
    }
};
