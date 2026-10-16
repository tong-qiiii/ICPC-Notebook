#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct GaussD
{
    static constexpr double EPS = 1e-9;

    int solve(vector<vector<double>> a, vector<double> &ans) const
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
            int sel = row;

            for (int i = row; i < n; ++i)
            {
                if (fabs(a[i][col]) > fabs(a[sel][col]))
                {
                    sel = i;
                }
            }

            if (fabs(a[sel][col]) < EPS)
            {
                continue;
            }

            swap(a[sel], a[row]);
            where[col] = row;

            double inv = 1.0 / a[row][col];

            for (int j = col; j <= m; ++j)
            {
                a[row][j] *= inv;
            }

            for (int i = 0; i < n; ++i)
            {
                if (i == row)
                {
                    continue;
                }

                double factor = a[i][col];

                if (fabs(factor) < EPS)
                {
                    continue;
                }

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
                if (fabs(a[i][j]) >= EPS)
                {
                    zero = false;
                    break;
                }
            }

            if (zero && fabs(a[i][m]) >= EPS)
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
