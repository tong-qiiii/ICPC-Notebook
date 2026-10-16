#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "fps.hpp"
namespace poly998
{

inline pair<Poly, Poly> Poly::divmod(const Poly &b) const
{
    assert(!b.empty());

    if (size() < b.size())
    {
        return {Poly(), *this};
    }

    int qn = size() - b.size() + 1;

    Poly q = (rev(size()).mod_xk(qn) * b.rev(b.size()).inv(qn))
                 .mod_xk(qn)
                 .rev(qn);

    Poly r = (*this - b * q).mod_xk(b.size() - 1);

    return {q, r};
}

inline Poly operator/(const Poly &x, const Poly &y)
{
    return x.divmod(y).first;
}

inline Poly operator%(const Poly &x, const Poly &y)
{
    return x.divmod(y).second;
}

inline vector<int> Poly::eval(const vector<int> &xs) const
{
    int n = (int)xs.size();
    if (n == 0)
    {
        return {};
    }

    int base = 1;
    while (base < n)
    {
        base <<= 1;
    }

    vector<Poly> prod(base << 1, Poly(1));

    for (int i = 0; i < n; ++i)
    {
        prod[base + i] = Poly({norm(-1LL * xs[i]), 1});
    }

    for (int i = base - 1; i >= 1; --i)
    {
        prod[i] = prod[i << 1] * prod[i << 1 | 1];
    }

    vector<Poly> rem(base << 1);
    rem[1] = *this % prod[1];

    for (int i = 1; i < base; ++i)
    {
        rem[i << 1] = rem[i] % prod[i << 1];
        rem[i << 1 | 1] = rem[i] % prod[i << 1 | 1];
    }

    vector<int> ans(n);

    for (int i = 0; i < n; ++i)
    {
        ans[i] = rem[base + i][0];
    }

    return ans;
}

inline optional<Poly> Poly::interpolate(const vector<int> &xs,
                                        const vector<int> &ys)
{
    int n = (int)xs.size();

    assert((int)ys.size() == n);

    if (n == 0)
    {
        return Poly();
    }

    int base = 1;
    while (base < n)
    {
        base <<= 1;
    }

    vector<Poly> prod(base << 1, Poly(1));

    for (int i = 0; i < n; ++i)
    {
        prod[base + i] = Poly({norm(-1LL * xs[i]), 1});
    }

    for (int i = base - 1; i >= 1; --i)
    {
        prod[i] = prod[i << 1] * prod[i << 1 | 1];
    }

    vector<int> den = prod[1].deriv().eval(xs);
    vector<Poly> val(base << 1);

    for (int i = 0; i < n; ++i)
    {
        if (den[i] == 0)
        {
            return nullopt;
        }
        val[base + i] = Poly(1LL * norm(ys[i]) * invmod(den[i]) % MOD);
    }

    for (int i = base - 1; i >= 1; --i)
    {
        val[i] =
            val[i << 1] * prod[i << 1 | 1] + val[i << 1 | 1] * prod[i << 1];
    }

    return val[1].mod_xk(n);
}

} // namespace poly998
