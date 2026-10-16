#pragma once
#include <bits/stdc++.h>
using namespace std;

template<class T>
vector<T> partition_numbers(int n)
{
    assert(n >= 0);
    vector<T> p(n + 1);
    p[0] = T(1);
    for (int i = 1; i <= n; i++)
    {
        for (long long k = 1; k * (3 * k - 1) / 2 <= i; k++)
        {
            long long a = k * (3 * k - 1) / 2, b = k * (3 * k + 1) / 2;
            if (k & 1)
            {
                p[i] = p[i] + p[i - a];
                if (b <= i) p[i] = p[i] + p[i - b];
            }
            else
            {
                p[i] = p[i] - p[i - a];
                if (b <= i) p[i] = p[i] - p[i - b];
            }
        }
    }
    return p;
}
