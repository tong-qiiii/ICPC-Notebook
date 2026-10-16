#pragma once
#include <bits/stdc++.h>
using namespace std;

// lower: b[n]=sum C(n,k)a[k]；upper: b[k]=sum C(n,k)a[n]。
template<class T, class C>
vector<T> binom_transform(const vector<T> &a, C choose,
                          bool inv = false, bool upper = false)
{
    int n = (int)a.size();
    vector<T> b(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            T v = T(choose(i, j)) * (upper ? a[i] : a[j]);
            int p = upper ? j : i;
            if (inv && ((i - j) & 1)) b[p] = b[p] - v;
            else b[p] = b[p] + v;
        }
    }
    return b;
}
