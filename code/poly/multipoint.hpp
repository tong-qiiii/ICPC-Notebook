#pragma once
#include "vector_core.hpp"

struct Multipoint
{
    int n;
    vector<int> x;
    vector<Poly> prod;

    Multipoint(vector<int> x_)
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

    void dfs(int p, int l, int r, const Poly &f, vector<int> &ans)
    {
        if (r - l == 1)
        {
            ans[l] = f.empty() ? 0 : f[0];
            return;
        }

        int mid = (l + r) >> 1;

        Poly lf = polyMod(f, prod[p << 1]);
        Poly rf = polyMod(f, prod[p << 1 | 1]);

        dfs(p << 1, l, mid, lf, ans);
        dfs(p << 1 | 1, mid, r, rf, ans);
    }

    vector<int> eval(const Poly &f)
    {
        if (!n)
        {
            return {};
        }

        vector<int> ans(n);
        Poly r = polyMod(f, prod[1]);

        dfs(1, 0, n, r, ans);

        return ans;
    }
};
