#pragma once
#include "ntt.hpp"

namespace poly998
{

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

struct Poly
{
    vector<int> a;

    Poly() = default;
    Poly(long long c)
    {
        c = norm(c);
        if (c)
        {
            a = {(int)c};
        }
    }
    Poly(initializer_list<int> v) : a(v)
    {
        for (int &x : a)
        {
            x = norm(x);
        }
        trim();
    }
    explicit Poly(vector<int> v) : a(move(v))
    {
        for (int &x : a)
        {
            x = norm(x);
        }
        trim();
    }

    void trim()
    {
        while (!a.empty() && a.back() == 0)
        {
            a.pop_back();
        }
    }
    void normalize()
    {
        trim();
    }

    int size() const
    {
        return (int)a.size();
    }
    bool empty() const
    {
        return a.empty();
    }
    int deg() const
    {
        return size() - 1;
    }

    int operator[](int i) const
    {
        return 0 <= i && i < size() ? a[i] : 0;
    }
    int &raw(int i)
    {
        return a[i];
    }

    Poly mod_xk(int k) const
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
    Poly mul_xk(int k) const
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
    Poly div_xk(int k) const
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
    Poly rev(int n = -1) const
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

    friend bool operator==(const Poly &x, const Poly &y)
    {
        return x.a == y.a;
    }
    friend bool operator!=(const Poly &x, const Poly &y)
    {
        return !(x == y);
    }

    friend Poly operator+(const Poly &x, const Poly &y)
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
    friend Poly operator-(const Poly &x, const Poly &y)
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
    friend Poly operator-(const Poly &x)
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
    friend Poly operator*(const Poly &x, const Poly &y)
    {
        return Poly(convolution(x.a, y.a));
    }
    friend Poly operator*(const Poly &x, long long k)
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
    friend Poly operator*(long long k, const Poly &x)
    {
        return x * k;
    }
    friend Poly operator/(const Poly &x, long long k)
    {
        return x * invmod(k);
    }
    friend Poly operator/(const Poly &x, const Poly &y);
    friend Poly operator%(const Poly &x, const Poly &y);

    Poly &operator+=(const Poly &o)
    {
        return *this = *this + o;
    }
    Poly &operator-=(const Poly &o)
    {
        return *this = *this - o;
    }
    Poly &operator*=(const Poly &o)
    {
        return *this = *this * o;
    }
    Poly &operator*=(long long k)
    {
        return *this = *this * k;
    }
    Poly &operator/=(long long k)
    {
        return *this = *this / k;
    }

  private:
    optional<Poly> sqrt_nonzero_constant(int n) const;
};

// 保留 poly998::operator+ 等显式限定调用。
inline bool operator==(const Poly &x, const Poly &y);
inline bool operator!=(const Poly &x, const Poly &y);
inline Poly operator+(const Poly &x, const Poly &y);
inline Poly operator-(const Poly &x, const Poly &y);
inline Poly operator-(const Poly &x);
inline Poly operator*(const Poly &x, const Poly &y);
inline Poly operator*(const Poly &x, long long k);
inline Poly operator*(long long k, const Poly &x);
inline Poly operator/(const Poly &x, long long k);

} // namespace poly998
