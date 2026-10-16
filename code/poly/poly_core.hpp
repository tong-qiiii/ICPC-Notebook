#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include <bits/stdc++.h>
using namespace std;

namespace poly998
{

constexpr int MOD = 998244353;
constexpr int G = 3;
constexpr int MAX_LOG = 23; // MOD - 1 = 119 * 2^23

inline int norm(long long x)
{
    x %= MOD;
    if (x < 0)
    {
        x += MOD;
    }
    return (int)x;
}

inline int qpow(long long a, long long e)
{
    a = norm(a);
    long long r = 1;
    while (e)
    {
        if (e & 1)
        {
            r = r * a % MOD;
        }
        a = a * a % MOD;
        e >>= 1;
    }
    return (int)r;
}

inline int invmod(long long x)
{
    int a = norm(x);
    assert(a != 0);
    return qpow(a, MOD - 2);
}

inline int inv_int(int x)
{
    return invmod(x);
}

inline vector<int> fac{1}, ifac{1};

inline void ensure_fact(int n)
{
    assert(0 <= n && n < MOD);
    int old = (int)fac.size() - 1;
    if (old >= n)
    {
        return;
    }

    fac.resize(n + 1);
    for (int i = old + 1; i <= n; ++i)
    {
        fac[i] = (int)(1LL * fac[i - 1] * i % MOD);
    }

    ifac.resize(n + 1);
    ifac[n] = invmod(fac[n]);
    for (int i = n; i > old; --i)
    {
        ifac[i - 1] = (int)(1LL * ifac[i] * i % MOD);
    }
}

inline vector<int> batch_inv(vector<int> a)
{
    int n = (int)a.size();
    vector<int> pre(n + 1, 1), res(n);

    for (int i = 0; i < n; ++i)
    {
        a[i] = norm(a[i]);
        pre[i + 1] = pre[i];
        if (a[i])
        {
            pre[i + 1] = (int)(1LL * pre[i + 1] * a[i] % MOD);
        }
    }

    int suf = invmod(pre[n]);
    for (int i = n - 1; i >= 0; --i)
    {
        if (!a[i])
        {
            continue;
        }
        res[i] = (int)(1LL * pre[i] * suf % MOD);
        suf = (int)(1LL * suf * a[i] % MOD);
    }

    return res;
}

inline optional<int> mod_sqrt(int a)
{
    a = norm(a);
    if (a == 0)
    {
        return 0;
    }
    if (qpow(a, (MOD - 1) / 2) != 1)
    {
        return nullopt;
    }

    int s = 0, q = MOD - 1;
    while ((q & 1) == 0)
    {
        ++s;
        q >>= 1;
    }

    int z = 2;
    while (qpow(z, (MOD - 1) / 2) != MOD - 1)
    {
        ++z;
    }

    long long c = qpow(z, q);
    long long x = qpow(a, (q + 1) / 2);
    long long t = qpow(a, q);
    int m = s;

    while (t != 1)
    {
        int i = 1;
        long long tt = t * t % MOD;

        while (i < m && tt != 1)
        {
            tt = tt * tt % MOD;
            ++i;
        }

        assert(i < m);

        long long b = qpow(c, 1LL << (m - i - 1));
        x = x * b % MOD;
        t = t * b % MOD * b % MOD;
        c = b * b % MOD;
        m = i;
    }

    return (int)x;
}

inline void ntt(vector<int> &a, bool invert)
{
    int n = (int)a.size();
    if (n <= 1)
    {
        return;
    }

    assert((n & (n - 1)) == 0);
    assert(n <= (1 << MAX_LOG));

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
        int wlen = qpow(G, (MOD - 1) / len);
        if (invert)
        {
            wlen = invmod(wlen);
        }

        for (int i = 0; i < n; i += len)
        {
            long long w = 1;
            int half = len >> 1;

            for (int j = 0; j < half; ++j)
            {
                int u = a[i + j];
                int v = (int)(w * a[i + j + half] % MOD);

                int x = u + v;
                if (x >= MOD)
                {
                    x -= MOD;
                }
                a[i + j] = x;

                x = u - v;
                if (x < 0)
                {
                    x += MOD;
                }
                a[i + j + half] = x;

                w = w * wlen % MOD;
            }
        }
    }

    if (invert)
    {
        int inv_n = invmod(n);
        for (int &x : a)
        {
            x = (int)(1LL * x * inv_n % MOD);
        }
    }
}

inline vector<int> convolution(const vector<int> &a, const vector<int> &b)
{
    if (a.empty() || b.empty())
    {
        return {};
    }

    if (1LL * a.size() * b.size() <= 4096)
    {
        vector<int> c(a.size() + b.size() - 1);

        for (int i = 0; i < (int)a.size(); ++i)
        {
            int x = norm(a[i]);
            for (int j = 0; j < (int)b.size(); ++j)
            {
                c[i + j] = (c[i + j] + 1LL * x * norm(b[j])) % MOD;
            }
        }

        return c;
    }

    assert(a.size() + b.size() - 1 <= (1U << MAX_LOG));
    int need = (int)a.size() + (int)b.size() - 1;
    int n = 1;
    while (n < need)
    {
        n <<= 1;
    }

    assert(n <= (1 << MAX_LOG));

    vector<int> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    for (int &x : fa)
    {
        x = norm(x);
    }
    for (int &x : fb)
    {
        x = norm(x);
    }

    fa.resize(n);
    fb.resize(n);

    ntt(fa, false);
    ntt(fb, false);

    for (int i = 0; i < n; ++i)
    {
        fa[i] = (int)(1LL * fa[i] * fb[i] % MOD);
    }

    ntt(fa, true);
    fa.resize(need);
    return fa;
}

struct Poly
{
    vector<int> a;

    Poly() = default;
    Poly(long long c);
    Poly(initializer_list<int> v);
    explicit Poly(vector<int> v);

    void trim();
    void normalize();

    int size() const;
    bool empty() const;
    int deg() const;

    int operator[](int i) const;
    int &raw(int i);

    Poly mod_xk(int k) const;
    Poly mul_xk(int k) const;
    Poly div_xk(int k) const;
    Poly rev(int n = -1) const;

    Poly deriv() const;
    Poly integr() const;

    Poly inv(int n) const;
    Poly log(int n) const;
    Poly exp(int n) const;
    Poly pow(long long k, int n) const;
    optional<Poly> sqrt(int n) const;

    Poly sin(int n) const;
    Poly cos(int n) const;
    Poly tan(int n) const;
    Poly asin(int n) const;
    Poly atan(int n) const;

    pair<Poly, Poly> divmod(const Poly &b) const;

    vector<int> eval(const vector<int> &xs) const;
    static optional<Poly> interpolate(const vector<int> &xs,
                                      const vector<int> &ys);

    Poly shift(long long c) const;

    Poly compose_slow(const Poly &g, int n) const;
    Poly compositional_inverse_slow(int n) const;

    friend bool operator==(const Poly &x, const Poly &y);
    friend bool operator!=(const Poly &x, const Poly &y);

    friend Poly operator+(const Poly &x, const Poly &y);
    friend Poly operator-(const Poly &x, const Poly &y);
    friend Poly operator-(const Poly &x);
    friend Poly operator*(const Poly &x, const Poly &y);
    friend Poly operator*(const Poly &x, long long k);
    friend Poly operator*(long long k, const Poly &x);
    friend Poly operator/(const Poly &x, long long k);
    friend Poly operator/(const Poly &x, const Poly &y);
    friend Poly operator%(const Poly &x, const Poly &y);

    Poly &operator+=(const Poly &o);
    Poly &operator-=(const Poly &o);
    Poly &operator*=(const Poly &o);
    Poly &operator*=(long long k);
    Poly &operator/=(long long k);

  private:
    optional<Poly> sqrt_nonzero_constant(int n) const;
};

inline Poly::Poly(long long c)
{
    c = norm(c);
    if (c)
    {
        a = {(int)c};
    }
}

inline Poly::Poly(initializer_list<int> v) : a(v)
{
    for (int &x : a)
    {
        x = norm(x);
    }
    trim();
}

inline Poly::Poly(vector<int> v) : a(move(v))
{
    for (int &x : a)
    {
        x = norm(x);
    }
    trim();
}

inline void Poly::trim()
{
    while (!a.empty() && a.back() == 0)
    {
        a.pop_back();
    }
}

inline void Poly::normalize()
{
    trim();
}

inline int Poly::size() const
{
    return (int)a.size();
}

inline bool Poly::empty() const
{
    return a.empty();
}

inline int Poly::deg() const
{
    return size() - 1;
}

inline int Poly::operator[](int i) const
{
    return 0 <= i && i < size() ? a[i] : 0;
}

inline int &Poly::raw(int i)
{
    return a[i];
}

inline Poly Poly::mod_xk(int k) const
{
    if (k <= 0 || empty())
    {
        return Poly();
    }
    if (k >= size())
    {
        return *this;
    }
    return Poly(vector<int>(a.begin(), a.begin() + k));
}

inline Poly Poly::mul_xk(int k) const
{
    assert(k >= 0);
    if (empty())
    {
        return Poly();
    }

    vector<int> r(k);
    r.insert(r.end(), a.begin(), a.end());
    return Poly(move(r));
}

inline Poly Poly::div_xk(int k) const
{
    if (k <= 0)
    {
        return *this;
    }
    if (k >= size())
    {
        return Poly();
    }
    return Poly(vector<int>(a.begin() + k, a.end()));
}

inline Poly Poly::rev(int n) const
{
    if (n < 0)
    {
        n = size();
    }

    vector<int> r = a;
    r.resize(n);
    reverse(r.begin(), r.end());

    return Poly(move(r));
}

inline bool operator==(const Poly &x, const Poly &y)
{
    return x.a == y.a;
}

inline bool operator!=(const Poly &x, const Poly &y)
{
    return !(x == y);
}

inline Poly operator+(const Poly &x, const Poly &y)
{
    vector<int> r(max(x.size(), y.size()));

    for (int i = 0; i < (int)r.size(); ++i)
    {
        int v = x[i] + y[i];
        if (v >= MOD)
        {
            v -= MOD;
        }
        r[i] = v;
    }

    return Poly(move(r));
}

inline Poly operator-(const Poly &x, const Poly &y)
{
    vector<int> r(max(x.size(), y.size()));

    for (int i = 0; i < (int)r.size(); ++i)
    {
        int v = x[i] - y[i];
        if (v < 0)
        {
            v += MOD;
        }
        r[i] = v;
    }

    return Poly(move(r));
}

inline Poly operator-(const Poly &x)
{
    vector<int> r = x.a;

    for (int &v : r)
    {
        if (v)
        {
            v = MOD - v;
        }
    }

    return Poly(move(r));
}

inline Poly operator*(const Poly &x, const Poly &y)
{
    return Poly(convolution(x.a, y.a));
}

inline Poly operator*(const Poly &x, long long k)
{
    int c = norm(k);
    if (c == 0 || x.empty())
    {
        return Poly();
    }

    vector<int> r = x.a;
    for (int &v : r)
    {
        v = (int)(1LL * v * c % MOD);
    }

    return Poly(move(r));
}

inline Poly operator*(long long k, const Poly &x)
{
    return x * k;
}

inline Poly operator/(const Poly &x, long long k)
{
    return x * invmod(k);
}

inline Poly &Poly::operator+=(const Poly &o)
{
    return *this = *this + o;
}

inline Poly &Poly::operator-=(const Poly &o)
{
    return *this = *this - o;
}

inline Poly &Poly::operator*=(const Poly &o)
{
    return *this = *this * o;
}

inline Poly &Poly::operator*=(long long k)
{
    return *this = *this * k;
}

inline Poly &Poly::operator/=(long long k)
{
    return *this = *this / k;
}

} // namespace poly998
