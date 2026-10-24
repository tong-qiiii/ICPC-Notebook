#pragma once
#include <bits/stdc++.h>
using namespace std;

// conv 为 T 上的线性卷积；fac / ifac 至少覆盖 0..a.size()-1。
template<class T, class Conv>
vector<T> binom_fast(const vector<T> &a, const vector<T> &fac,
                     const vector<T> &ifac, Conv conv, bool inv = false, bool upper = false)
{
    int n = (int)a.size();
    if (n == 0) return {};
    if ((int)fac.size() < n || (int)ifac.size() < n) throw out_of_range("factorial table too small");
    vector<T> u(n), v(n);
    for (int i = 0; i < n; i++)
    {
        u[upper ? n - 1 - i : i] = a[i] * (upper ? fac[i] : ifac[i]);
        v[i] = inv && (i & 1) ? -ifac[i] : ifac[i];
    }
    auto w = conv(u, v);
    w.resize(n);
    vector<T> b(n);
    for (int i = 0; i < n; i++)
    {
        b[i] = w[upper ? n - 1 - i : i] * (upper ? ifac[i] : fac[i]);
    }
    return b;
}
