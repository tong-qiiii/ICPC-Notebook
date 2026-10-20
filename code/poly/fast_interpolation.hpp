#pragma once
#include "vector_core.hpp"

struct ProductTree
{
    int n;
    vector<int> x;
    vector<Poly> prod;

    ProductTree(vector<int> x_)
        : n(x_.size()), x(move(x_)), prod(max(1, 4 * n))
    {
        for (int &v : x)
        {
            v %= MOD;
            if (v < 0)
            {
                v += MOD;
            }
        }

        if (n)
        {
            build(1, 0, n);
        }
    }

    void build(int p, int l, int r)
    {
        if (r - l == 1)
        {
            prod[p] = {(MOD - x[l]) % MOD, 1};
            return;
        }

        int mid = (l + r) >> 1;

        build(p << 1, l, mid);
        build(p << 1 | 1, mid, r);

        prod[p] = mul(prod[p << 1], prod[p << 1 | 1]);
    }

    void evalDfs(int p, int l, int r, const Poly &f, vector<int> &ans)
    {
        if (r - l == 1)
        {
            ans[l] = f.empty() ? 0 : f[0];
            return;
        }

        int mid = (l + r) >> 1;

        Poly lf = polyMod(f, prod[p << 1]);
        Poly rf = polyMod(f, prod[p << 1 | 1]);

        evalDfs(p << 1, l, mid, lf, ans);
        evalDfs(p << 1 | 1, mid, r, rf, ans);
    }

    vector<int> eval(const Poly &f)
    {
        vector<int> ans(n);

        if (!n)
        {
            return ans;
        }

        Poly r = polyMod(f, prod[1]);
        evalDfs(1, 0, n, r, ans);

        return ans;
    }

    Poly interpolateDfs(int p, int l, int r, const vector<int> &w)
    {
        if (r - l == 1)
        {
            return {w[l]};
        }

        int mid = (l + r) >> 1;

        Poly L = interpolateDfs(p << 1, l, mid, w);
        Poly R = interpolateDfs(p << 1 | 1, mid, r, w);

        Poly a = mul(L, prod[p << 1 | 1]);
        Poly b = mul(R, prod[p << 1]);

        return addPoly(a, b);
    }
};

inline Poly fastInterpolation(vector<int> x, const vector<int> &y)
{
    int n = x.size();
    assert(y.size() == x.size());
    if (!n)
    {
        return {};
    }

    ProductTree tree(x);

    Poly P = tree.prod[1];
    Poly dP(max(0, (int)P.size() - 1));

    for (int i = 1; i < (int)P.size(); ++i)
    {
        dP[i - 1] = (ll)i * P[i] % MOD;
    }

    vector<int> d = tree.eval(dP);
    vector<int> w(n);

    for (int i = 0; i < n; ++i)
    {
        assert(d[i] != 0);
        ll v = y[i] % MOD;
        if (v < 0)
        {
            v += MOD;
        }
        w[i] = v * modPow(d[i], MOD - 2) % MOD;
    }

    Poly ans = tree.interpolateDfs(1, 0, n, w);

    ans.resize(n);
    return ans;
}
