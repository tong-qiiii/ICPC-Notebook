#pragma once
#include <bits/stdc++.h>
using namespace std;

// 1-index；正变换为约数和，逆变换不需要显式 mu。
template<class T>
vector<T> divisor_transform(vector<T> a, bool inv = false)
{
    int n = max(0, (int)a.size() - 1);
    if (inv)
    {
        for (int d = 1; d <= n; d++)
        {
            for (int j = d + d; j <= n; j += d) a[j] = a[j] - a[d];
        }
        return a;
    }
    vector<T> b = a;
    for (int d = 1; d <= n; d++)
    {
        for (int j = d + d; j <= n; j += d) b[j] = b[j] + a[d];
    }
    return b;
}

// 正变换为有界倍数和，逆变换要从大到小。
template<class T>
vector<T> multiple_transform(vector<T> a, bool inv = false)
{
    int n = max(0, (int)a.size() - 1);
    if (inv)
    {
        for (int d = n; d >= 1; d--)
        {
            for (int j = d + d; j <= n; j += d) a[d] = a[d] - a[j];
        }
        return a;
    }
    vector<T> b = a;
    for (int d = 1; d <= n; d++)
    {
        for (int j = d + d; j <= n; j += d) b[d] = b[d] + a[j];
    }
    return b;
}
