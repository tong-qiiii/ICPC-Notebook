#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int G = 3;

using Poly = vector<int>;

inline int modPow(int a, int e)
{
    ll res = 1;

    while (e)
    {
        if (e & 1)
        {
            res = res * a % MOD;
        }
        a = (ll)a * a % MOD;
        e >>= 1;
    }

    return res;
}

inline void ntt(Poly &a, bool inv)
{
    int n = a.size();
    assert(n > 0 && (n & (n - 1)) == 0 && n <= (1 << 23));

    for (int i = 1, j = 0; i < n; ++i)
    {
        int bit = n >> 1;

        for (; j & bit; bit >>= 1)
        {
            j ^= bit;
        }

        j ^= bit;

        if (i < j)
        {
            swap(a[i], a[j]);
        }
    }

    for (int len = 2; len <= n; len <<= 1)
    {
        int wlen = modPow(G, (MOD - 1) / len);

        if (inv)
        {
            wlen = modPow(wlen, MOD - 2);
        }

        for (int i = 0; i < n; i += len)
        {
            ll w = 1;

            for (int j = 0; j < len / 2; ++j)
            {
                int u = a[i + j];
                int v = w * a[i + j + len / 2] % MOD;

                a[i + j] = u + v;
                if (a[i + j] >= MOD)
                {
                    a[i + j] -= MOD;
                }

                a[i + j + len / 2] = u - v;
                if (a[i + j + len / 2] < 0)
                {
                    a[i + j + len / 2] += MOD;
                }

                w = w * wlen % MOD;
            }
        }
    }

    if (inv)
    {
        int invN = modPow(n, MOD - 2);

        for (int &x : a)
        {
            x = (ll)x * invN % MOD;
        }
    }
}

inline void norm(Poly &a)
{
    while (!a.empty() && a.back() == 0)
    {
        a.pop_back();
    }
}

inline Poly mul(Poly a, Poly b)
{
    if (a.empty() || b.empty())
    {
        return {};
    }

    if ((ll)a.size() * b.size() <= 256)
    {
        Poly c(a.size() + b.size() - 1);

        for (int i = 0; i < (int)a.size(); ++i)
        {
            for (int j = 0; j < (int)b.size(); ++j)
            {
                c[i + j] = (c[i + j] + (ll)a[i] * b[j]) % MOD;
            }
        }

        return c;
    }

    assert(a.size() + b.size() - 1 <= (1U << 23));
    int need = a.size() + b.size() - 1;
    int n = 1;

    while (n < need)
    {
        n <<= 1;
    }

    a.resize(n);
    b.resize(n);

    ntt(a, false);
    ntt(b, false);

    for (int i = 0; i < n; ++i)
    {
        a[i] = (ll)a[i] * b[i] % MOD;
    }

    ntt(a, true);
    a.resize(need);

    return a;
}

inline Poly polyInv(const Poly &a, int n)
{
    if (n <= 0)
    {
        return {};
    }
    assert(!a.empty() && a[0]);
    Poly res(1, modPow(a[0], MOD - 2));

    for (int len = 2; len < 2 * n; len <<= 1)
    {
        int m = min(len, n);

        Poly f(a.begin(), a.begin() + min((int)a.size(), m));
        Poly t = mul(f, res);

        t.resize(m);

        for (int &x : t)
        {
            x = x ? MOD - x : 0;
        }

        t[0] += 2;
        if (t[0] >= MOD)
        {
            t[0] -= MOD;
        }

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

    if (a.size() < b.size())
    {
        return a;
    }

    int k = a.size() - b.size() + 1;

    Poly ra = a, rb = b;

    reverse(ra.begin(), ra.end());
    reverse(rb.begin(), rb.end());

    ra.resize(k);
    rb.resize(k);

    Poly q = mul(ra, polyInv(rb, k));
    q.resize(k);

    reverse(q.begin(), q.end());

    Poly qb = mul(q, b);

    for (int i = 0; i < (int)qb.size(); ++i)
    {
        a[i] -= qb[i];
        if (a[i] < 0)
        {
            a[i] += MOD;
        }
    }

    a.resize(b.size() - 1);
    norm(a);

    return a;
}

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
