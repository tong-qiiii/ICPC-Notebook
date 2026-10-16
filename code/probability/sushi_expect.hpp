#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef db
#define db double
#endif

inline db sushi_expect(const vector<int> &a)
{
    int n = max(0, (int)a.size() - 1), c1 = 0, c2 = 0, c3 = 0;
    for (int i = 1; i <= n; i++)
    {
        assert(0 <= a[i] && a[i] <= 3);
        c1 += a[i] == 1; c2 += a[i] == 2; c3 += a[i] == 3;
    }
    vector<vector<db>> pre(n + 2, vector<db>(n + 2)), dp = pre;
    for (int k = 0; k <= c3; k++)
    {
        for (auto &row : dp) fill(row.begin(), row.end(), 0);
        for (int j = 0; j + k <= n; j++)
        {
            for (int i = 0; i + j + k <= n; i++)
            {
                if (i + j + k == 0) continue;
                db val = n;
                if (i) val += i * dp[j][i - 1];
                if (j) val += j * dp[j - 1][i + 1];
                if (k) val += k * pre[j + 1][i];
                dp[j][i] = val / (i + j + k);
            }
        }
        if (k == c3) return dp[c2][c1];
        pre.swap(dp);
    }
    return 0;
}
