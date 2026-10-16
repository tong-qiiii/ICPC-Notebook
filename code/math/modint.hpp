#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

template <int MOD> struct Mint
{
    int v;

    Mint(long long x = 0)
    {
        x %= MOD;
        if (x < 0)
        {
            x += MOD;
        }
        v = int(x);
    }

    Mint &operator+=(const Mint &o)
    {
        long long x = (long long)v + o.v;
        v = int(x >= MOD ? x - MOD : x);
        return *this;
    }

    Mint &operator-=(const Mint &o)
    {
        v -= o.v;
        if (v < 0)
        {
            v += MOD;
        }
        return *this;
    }

    Mint &operator*=(const Mint &o)
    {
        v = (long long)v * o.v % MOD;
        return *this;
    }

    Mint &operator/=(const Mint &o)
    {
        return *this *= o.inv();
    }

    Mint operator-() const
    {
        return Mint(-v);
    }

    Mint pow(long long b) const
    {
        Mint a = *this, res = 1;

        while (b)
        {
            if (b & 1)
            {
                res *= a;
            }
            a *= a;
            b >>= 1;
        }

        return res;
    }

    Mint inv() const
    {
        return pow(MOD - 2);
    }

    friend Mint operator+(Mint a, const Mint &b)
    {
        return a += b;
    }

    friend Mint operator-(Mint a, const Mint &b)
    {
        return a -= b;
    }

    friend Mint operator*(Mint a, const Mint &b)
    {
        return a *= b;
    }

    friend Mint operator/(Mint a, const Mint &b)
    {
        return a /= b;
    }

    friend bool operator==(const Mint &a, const Mint &b)
    {
        return a.v == b.v;
    }

    friend bool operator!=(const Mint &a, const Mint &b)
    {
        return a.v != b.v;
    }

    friend ostream &operator<<(ostream &os, const Mint &x)
    {
        return os << x.v;
    }

    friend istream &operator>>(istream &is, Mint &x)
    {
        long long v;
        is >> v;
        x = Mint(v);
        return is;
    }
};
