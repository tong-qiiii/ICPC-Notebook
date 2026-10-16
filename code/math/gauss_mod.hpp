#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "modint.hpp"
template <int MOD> struct GaussM
{
    using Z = Mint<MOD>;

    int solve(vector<vector<Z>> a, vector<Z> &ans) const
    {
        int n = a.size();
        if (!n)
        {
            ans.clear();
            return 0;
        }
        int m = a[0].size() - 1;

        vector<int> where(m, -1);
        int row = 0;

        for (int col = 0; col < m && row < n; ++col)
        {
            int sel = -1;

            for (int i = row; i < n; ++i)
            {
                if (a[i][col] != Z(0))
                {
                    sel = i;
                    break;
                }
            }

            if (sel == -1)
            {
                continue;
            }

            swap(a[sel], a[row]);
            where[col] = row;

            Z inv = a[row][col].inv();

            for (int j = col; j <= m; ++j)
            {
                a[row][j] *= inv;
            }

            for (int i = 0; i < n; ++i)
            {
                if (i == row || a[i][col] == Z(0))
                {
                    continue;
                }

                Z factor = a[i][col];

                for (int j = col; j <= m; ++j)
                {
                    a[i][j] -= factor * a[row][j];
                }
            }

            ++row;
        }

        for (int i = 0; i < n; ++i)
        {
            bool zero = true;

            for (int j = 0; j < m; ++j)
            {
                if (a[i][j] != Z(0))
                {
                    zero = false;
                    break;
                }
            }

            if (zero && a[i][m] != Z(0))
            {
                ans.clear();
                return -1;
            }
        }

        ans.assign(m, 0);

        for (int i = 0; i < m; ++i)
        {
            if (where[i] != -1)
            {
                ans[i] = a[where[i]][m];
            }
        }

        return row;
    }
};
