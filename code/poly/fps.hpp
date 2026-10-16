#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "poly_core.hpp"
namespace poly998
{

inline Poly Poly::deriv() const
{
    if (size() <= 1)
    {
        return Poly();
    }

    vector<int> r(size() - 1);
    for (int i = 1; i < size(); ++i)
    {
        r[i - 1] = (int)(1LL * a[i] * i % MOD);
    }

    return Poly(move(r));
}

inline Poly Poly::integr() const
{
    if (empty())
    {
        return Poly();
    }

    assert(size() < MOD);

    static vector<int> invs{0, 1};
    int n = size();

    if ((int)invs.size() <= n)
    {
        int old = (int)invs.size();
        invs.resize(n + 1);

        for (int i = max(2, old); i <= n; ++i)
        {
            invs[i] = MOD - (int)(1LL * (MOD / i) * invs[MOD % i] % MOD);
        }
    }

    vector<int> r(n + 1);
    for (int i = 0; i < n; ++i)
    {
        r[i + 1] = (int)(1LL * a[i] * invs[i + 1] % MOD);
    }

    return Poly(move(r));
}

inline Poly Poly::inv(int n) const
{
    if (n <= 0)
    {
        return Poly();
    }

    assert(n < MOD);
    assert((*this)[0] != 0);

    Poly x(invmod((*this)[0]));

    for (int m = 2; m < 2 * n; m <<= 1)
    {
        int k = min(m, n);
        x = (x * (Poly(2) - mod_xk(k) * x)).mod_xk(k);
    }

    return x.mod_xk(n);
}

inline Poly Poly::log(int n) const
{
    if (n <= 0)
    {
        return Poly();
    }

    assert(n < MOD);
    assert((*this)[0] == 1);

    Poly f = mod_xk(n);
    return (f.deriv() * f.inv(n)).integr().mod_xk(n);
}

inline Poly Poly::exp(int n) const
{
    if (n <= 0)
    {
        return Poly();
    }

    assert(n < MOD);
    assert((*this)[0] == 0);

    Poly x(1);

    for (int m = 2; m < 2 * n; m <<= 1)
    {
        int k = min(m, n);
        x = (x * (mod_xk(k) - x.log(k) + Poly(1))).mod_xk(k);
    }

    return x.mod_xk(n);
}

inline Poly Poly::pow(long long k, int n) const
{
    assert(k >= 0);

    if (n <= 0)
    {
        return Poly();
    }

    assert(n < MOD);

    if (k == 0)
    {
        return Poly(1);
    }
    if (empty())
    {
        return Poly();
    }

    int t = 0;
    while (t < size() && a[t] == 0)
    {
        ++t;
    }

    if ((__int128)t * k >= n)
    {
        return Poly();
    }

    int shift = (int)((__int128)t * k);
    int lead = a[t];
    int need = n - shift;
    int r = min(size(), t + need);

    Poly g(vector<int>(a.begin() + t, a.begin() + r));
    g *= invmod(lead);

    Poly h = (g.log(need) * (k % MOD)).exp(need);
    h *= qpow(lead, k);

    return h.mul_xk(shift).mod_xk(n);
}

inline optional<Poly> Poly::sqrt_nonzero_constant(int n) const
{
    auto s0 = mod_sqrt((*this)[0]);
    if (!s0)
    {
        return nullopt;
    }

    Poly x(*s0);
    constexpr int INV2 = (MOD + 1) / 2;

    for (int m = 2; m < 2 * n; m <<= 1)
    {
        int k = min(m, n);
        x = ((x + mod_xk(k) * x.inv(k)) * INV2).mod_xk(k);
    }

    return x.mod_xk(n);
}

inline optional<Poly> Poly::sqrt(int n) const
{
    if (n <= 0 || empty())
    {
        return Poly();
    }

    assert(n < MOD);

    int t = 0;
    while (t < size() && a[t] == 0)
    {
        ++t;
    }

    // f == 0 (mod x^n)
    if (t >= n)
    {
        return Poly();
    }

    if (t & 1)
    {
        return nullopt;
    }

    int shift = t / 2;
    int need = n - t;

    Poly g(vector<int>(a.begin() + t, a.begin() + min(size(), n)));

    auto h = g.sqrt_nonzero_constant(need);
    if (!h)
    {
        return nullopt;
    }

    return h->mul_xk(shift).mod_xk(n);
}

inline int imag_unit()
{
    static int I = qpow(G, (MOD - 1) / 4);
    return I;
}

inline Poly Poly::sin(int n) const
{
    if (n <= 0)
    {
        return Poly();
    }

    assert((*this)[0] == 0);

    int I = imag_unit();
    Poly fi = mod_xk(n) * I;

    Poly e1 = fi.exp(n);
    Poly e2 = (-fi).exp(n);

    return (e1 - e2) * invmod(2LL * I);
}

inline Poly Poly::cos(int n) const
{
    if (n <= 0)
    {
        return Poly();
    }

    assert((*this)[0] == 0);

    int I = imag_unit();
    Poly fi = mod_xk(n) * I;

    Poly e1 = fi.exp(n);
    Poly e2 = (-fi).exp(n);

    return (e1 + e2) / 2;
}

inline Poly Poly::tan(int n) const
{
    if (n <= 0)
    {
        return Poly();
    }

    assert((*this)[0] == 0);

    Poly s = sin(n);
    Poly c = cos(n);

    return (s * c.inv(n)).mod_xk(n);
}

inline Poly Poly::asin(int n) const
{
    if (n <= 0)
    {
        return Poly();
    }

    assert((*this)[0] == 0);

    Poly f = mod_xk(n);
    auto rt = (Poly(1) - f * f).mod_xk(n).sqrt(n);

    assert(rt.has_value());

    Poly s = *rt;

    // asin 的形式幂级数分支要求 sqrt(1-f^2)[0] = +1
    if (s[0] != 1)
    {
        s = -s;
    }

    return (f.deriv() * s.inv(n)).integr().mod_xk(n);
}

inline Poly Poly::atan(int n) const
{
    if (n <= 0)
    {
        return Poly();
    }

    assert((*this)[0] == 0);

    Poly f = mod_xk(n);
    Poly den = (Poly(1) + f * f).mod_xk(n);

    return (f.deriv() * den.inv(n)).integr().mod_xk(n);
}

} // namespace poly998
