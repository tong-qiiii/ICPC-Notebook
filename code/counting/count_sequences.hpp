#pragma once
#include <bits/stdc++.h>
using namespace std;

template<class T>
vector<T> derangements(int n)
{
    assert(n >= 0);
    vector<T> d(n + 1);
    d[0] = T(1);
    for (int i = 2; i <= n; i++) d[i] = T(i - 1) * (d[i - 1] + d[i - 2]);
    return d;
}

// 卷积递推不除 n+1，可用于合数模的环类型。
template<class T>
vector<T> catalan_numbers(int n)
{
    assert(n >= 0);
    vector<T> c(n + 1);
    c[0] = T(1);
    for (int i = 1; i <= n; i++)
    {
        for (int j = 0; j < i; j++) c[i] = c[i] + c[j] * c[i - j - 1];
    }
    return c;
}

// 返回第 n 行，a[k] 为恰有 k 个下降位置的排列数。
template<class T>
vector<T> eulerian_numbers(int n)
{
    assert(n >= 0);
    vector<T> a(n + 1);
    a[0] = T(1);
    for (int i = 1; i <= n; i++)
    {
        for (int k = i - 1; k >= 0; k--)
        {
            a[k] = T(k + 1) * a[k] + (k ? T(i - k) * a[k - 1] : T(0));
        }
    }
    return a;
}
