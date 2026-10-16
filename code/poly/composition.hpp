#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "fps.hpp"
namespace poly998
{

inline int lagrange_y_power_coeff(const Poly &phi, int n, int k)
{
    assert(n >= 0);
    assert(k >= 0);

    if (k == 0)
    {
        return n == 0;
    }
    if (n == 0 || k > n)
    {
        return 0;
    }

    assert(n < MOD);
    assert(phi[0] != 0);

    int need = n - k + 1;
    Poly p = phi.pow(n, need);

    return (int)(1LL * k * invmod(n) % MOD * p[n - k] % MOD);
}

inline int lagrange_inverse_coeff(const Poly &f, int n)
{
    assert(n >= 0);

    if (n == 0)
    {
        return 0;
    }

    assert(n < MOD);
    assert(f[0] == 0);
    assert(f[1] != 0);

    Poly h = f.div_xk(1).mod_xk(n);
    Poly phi = h.inv(n);
    Poly p = phi.pow(n, n);

    return (int)(1LL * p[n - 1] * invmod(n) % MOD);
}

inline Poly Poly::compose_slow(const Poly &g, int n) const
{
    if (n <= 0)
    {
        return Poly();
    }

    assert(g[0] == 0);

    Poly h = g.mod_xk(n);
    Poly r;

    int lim = min(size(), n);

    for (int i = lim - 1; i >= 0; --i)
    {
        r = (r * h + Poly(a[i])).mod_xk(n);
    }

    return r;
}

inline Poly Poly::compositional_inverse_slow(int n) const
{
    if (n <= 0)
    {
        return Poly();
    }

    assert((*this)[0] == 0);
    assert((*this)[1] != 0);

    if (n == 1)
    {
        return Poly();
    }

    Poly g({0, invmod((*this)[1])});
    Poly X({0, 1});

    for (int m = 2; m < n; m <<= 1)
    {
        int k = min(m << 1, n);

        Poly fg = mod_xk(k).compose_slow(g, k);
        Poly df = deriv().mod_xk(k).compose_slow(g, k);

        g = (g - (fg - X) * df.inv(k)).mod_xk(k);
    }

    return g.mod_xk(n);
}

} // namespace poly998
