#pragma once
#include <cassert>

template <int MOD>
struct Mint {
    static_assert(MOD > 1);
    int v;

    Mint(long long x = 0) {
        x %= MOD;
        if (x < 0) x += MOD;
        v = int(x);
    }

    Mint& operator+=(const Mint& other) {
        long long sum = static_cast<long long>(v) + other.v;
        v = int(sum >= MOD ? sum - MOD : sum);
        return *this;
    }

    Mint& operator-=(const Mint& other) {
        v -= other.v;
        if (v < 0) v += MOD;
        return *this;
    }

    Mint& operator*=(const Mint& other) {
        v = int(static_cast<long long>(v) * other.v % MOD);
        return *this;
    }

    Mint pow(long long n) const {
        assert(n >= 0);
        Mint a = *this, result = 1;
        while (n) {
            if (n & 1) result *= a;
            a *= a;
            n >>= 1;
        }
        return result;
    }

    Mint inv() const {
        // Fermat: MOD must be prime, and v must be nonzero.
        assert(v != 0);
        return pow(MOD - 2);
    }

    Mint& operator/=(const Mint& other) { return *this *= other.inv(); }
    Mint operator-() const { return Mint(-v); }

    friend Mint operator+(Mint a, const Mint& b) { return a += b; }
    friend Mint operator-(Mint a, const Mint& b) { return a -= b; }
    friend Mint operator*(Mint a, const Mint& b) { return a *= b; }
    friend Mint operator/(Mint a, const Mint& b) { return a /= b; }
    friend bool operator==(Mint a, Mint b) { return a.v == b.v; }
    friend bool operator!=(Mint a, Mint b) { return a.v != b.v; }
};
