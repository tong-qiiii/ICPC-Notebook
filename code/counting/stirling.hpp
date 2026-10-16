#pragma once
#include <bits/stdc++.h>
using namespace std;

template<class T>
struct Stirling
{
    int n = 0;
    vector<vector<T>> s1, s2;

    Stirling(int n = 0)
    {
        init(n);
    }

    void init(int m)
    {
        assert(m >= 0);
        n = m;
        s1.assign(n + 1, vector<T>(n + 1));
        s2 = s1;
        s1[0][0] = s2[0][0] = T(1);
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= i; j++)
            {
                s1[i][j] = s1[i - 1][j - 1] + T(i - 1) * s1[i - 1][j];
                s2[i][j] = s2[i - 1][j - 1] + T(j) * s2[i - 1][j];
            }
        }
    }
};
