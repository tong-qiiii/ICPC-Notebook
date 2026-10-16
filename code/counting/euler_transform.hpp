#pragma once
#include <bits/stdc++.h>
using namespace std;

// a[0] 不用；b[0]=1，B(z)=prod(1-z^i)^(-a[i])。
template<class T>
vector<T> euler_transform(const vector<T> &a)
{
    assert(!a.empty());
    int n = (int)a.size() - 1;
    vector<T> c(n + 1), b(n + 1);
    b[0] = T(1);
    for (int d = 1; d <= n; d++)
    {
        for (int j = d; j <= n; j += d) c[j] = c[j] + T(d) * a[d];
    }
    for (int i = 1; i <= n; i++)
    {
        for (int k = 1; k <= i; k++) b[i] = b[i] + c[k] * b[i - k];
        b[i] = b[i] / T(i);
    }
    return b;
}

template<class T>
vector<T> euler_inverse(const vector<T> &b)
{
    assert(!b.empty() && b[0] == T(1));
    int n = (int)b.size() - 1;
    vector<T> c(n + 1);
    for (int i = 1; i <= n; i++)
    {
        c[i] = T(i) * b[i];
        for (int k = 1; k < i; k++) c[i] = c[i] - c[k] * b[i - k];
    }
    // c[n]=sum(d*a[d])，原地做约数和反演后再除以 n。
    for (int d = 1; d <= n; d++)
    {
        for (int j = d + d; j <= n; j += d) c[j] = c[j] - c[d];
    }
    for (int i = 1; i <= n; i++) c[i] = c[i] / T(i);
    return c;
}
