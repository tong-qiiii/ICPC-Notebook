#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

namespace poly998
{
constexpr int MOD = 998244353, G = 3, MAX_LOG = 23;

inline int norm(long long x)
{
    x %= MOD;
    return int(x < 0 ? x + MOD : x);
}

inline int qpow(long long a, long long e)
{
    a = norm(a);
    long long res = 1;
    for (; e; e >>= 1, a = a * a % MOD)
        if (e & 1) res = res * a % MOD;
    return int(res);
}

inline int invmod(long long x)
{
    assert(norm(x) != 0);
    return qpow(x, MOD - 2);
}

inline int inv_int(int x)
{
    return invmod(x);
}

// 原地变换：系数须在 [0,MOD)，长度为 2 的幂且不超过 2^23。
inline void ntt(vector<int> &a, bool invert)
{
    int n = (int)a.size();
    if (n <= 1) return;
    assert((n & (n - 1)) == 0 && n <= (1 << MAX_LOG));
    for (int i = 1, j = 0; i < n; i++)
    {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1)
    {
        int wn = qpow(G, (MOD - 1) / len);
        if (invert) wn = invmod(wn);
        for (int i = 0; i < n; i += len)
        {
            long long w = 1;
            for (int j = 0; j < len / 2; j++, w = w * wn % MOD)
            {
                int u = a[i + j], v = int(w * a[i + j + len / 2] % MOD);
                a[i + j] = u + v < MOD ? u + v : u + v - MOD;
                a[i + j + len / 2] = u >= v ? u - v : u - v + MOD;
            }
        }
    }
    if (invert)
    {
        int iv = invmod(n);
        for (int &x : a) x = int(1LL * x * iv % MOD);
    }
}

inline vector<int> convolution(const vector<int> &a, const vector<int> &b)
{
    if (a.empty() || b.empty()) return {};
    if (1LL * a.size() * b.size() <= 4096)
    {
        vector<int> c(a.size() + b.size() - 1);
        for (int i = 0; i < (int)a.size(); i++)
            for (int j = 0; j < (int)b.size(); j++)
                c[i + j] = int((c[i + j] + 1LL * norm(a[i]) * norm(b[j])) % MOD);
        return c;
    }
    assert(a.size() + b.size() - 1 <= (1U << MAX_LOG));
    int need = (int)a.size() + (int)b.size() - 1, n = 1;
    while (n < need) n <<= 1;
    vector<int> x(a.begin(), a.end()), y(b.begin(), b.end());
    for (int &v : x) v = norm(v);
    for (int &v : y) v = norm(v);
    x.resize(n); y.resize(n);
    ntt(x, false); ntt(y, false);
    for (int i = 0; i < n; i++) x[i] = int(1LL * x[i] * y[i] % MOD);
    ntt(x, true);
    x.resize(need);
    return x;
}
}
