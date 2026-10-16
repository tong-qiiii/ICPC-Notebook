#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include <bits/stdc++.h>
using namespace std;

inline ll dominoTilings(vector<string> a)
{
    if (a.empty() || a[0].empty())
    {
        return 1;
    }

    int n = a.size();
    int m = a[0].size();

    if (m > n)
    {
        vector<string> b(m, string(n, '.'));

        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                b[j][i] = a[i][j];
            }
        }

        a.swap(b);
        swap(n, m);
    }

    assert(m < 31);
    vector<int> block(n);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (a[i][j] == '#')
            {
                block[i] |= 1 << j;
            }
        }
    }

    int S = 1 << m;
    vector<ll> dp(S), ndp(S);
    dp[0] = 1;

    function<void(int, int, int, int, ll)> dfs =
        [&](int row, int col, int occ, int nxt, ll ways)
    {
        if (col == m)
        {
            ndp[nxt] += ways;
            return;
        }

        if (occ >> col & 1)
        {
            dfs(row, col + 1, occ, nxt, ways);
            return;
        }

        if (col + 1 < m && !(occ >> (col + 1) & 1))
        {
            dfs(row, col + 2, occ, nxt, ways);
        }

        if (row + 1 < n && !(block[row + 1] >> col & 1))
        {
            dfs(row, col + 1, occ, nxt | (1 << col), ways);
        }
    };

    for (int row = 0; row < n; ++row)
    {
        fill(ndp.begin(), ndp.end(), 0);

        for (int mask = 0; mask < S; ++mask)
        {
            if (!dp[mask] || (mask & block[row]))
            {
                continue;
            }
            dfs(row, 0, mask | block[row], 0, dp[mask]);
        }

        dp.swap(ndp);
    }

    return dp[0];
}
