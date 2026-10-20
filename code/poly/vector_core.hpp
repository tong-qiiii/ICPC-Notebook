#pragma once
#include "ntt.hpp"

// 独立 vector 版接口；完整类另名 poly998::Poly，混用时显式限定类型。
using Poly = vector<int>;
using poly998::MOD;
using poly998::G;
using poly998::ntt;

inline int modPow(int a, int e)
{
    return poly998::qpow(a, e);
}

inline void norm(Poly &a)
{
    while (!a.empty() && a.back() == 0) a.pop_back();
}

inline Poly mul(Poly a, Poly b)
{
    return poly998::convolution(a, b);
}

inline Poly addPoly(Poly a, const Poly &b)
{
    a.resize(max(a.size(), b.size()));
    for (int i = 0; i < (int)b.size(); i++)
    {
        a[i] += b[i];
        if (a[i] >= MOD) a[i] -= MOD;
    }
    norm(a);
    return a;
}

inline Poly polyInv(const Poly &a, int n)
{
    if (n <= 0) return {};
    assert(!a.empty() && a[0]);
    Poly res(1, modPow(a[0], MOD - 2));
    for (int len = 2; len < 2 * n; len <<= 1)
    {
        int m = min(len, n);
        Poly f(a.begin(), a.begin() + min((int)a.size(), m));
        Poly t = mul(f, res);
        t.resize(m);
        for (int &x : t) x = x ? MOD - x : 0;
        t[0] = (t[0] + 2) % MOD;
        res = mul(res, t);
        res.resize(m);
    }
    res.resize(n);
    return res;
}

inline Poly polyMod(Poly a, const Poly &b)
{
    norm(a);
    assert(!b.empty() && b.back());
    if (a.size() < b.size()) return a;
    int k = int(a.size() - b.size() + 1);
    Poly ra = a, rb = b;
    reverse(ra.begin(), ra.end()); reverse(rb.begin(), rb.end());
    ra.resize(k); rb.resize(k);
    Poly q = mul(ra, polyInv(rb, k));
    q.resize(k);
    reverse(q.begin(), q.end());
    Poly qb = mul(q, b);
    for (int i = 0; i < (int)qb.size(); i++)
    {
        a[i] -= qb[i];
        if (a[i] < 0) a[i] += MOD;
    }
    a.resize(b.size() - 1);
    norm(a);
    return a;
}
