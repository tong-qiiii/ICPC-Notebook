#include "code/basic/ternary_search_int.hpp"

int main()
{
    mt19937_64 gen(20261017);
    for (int t = 0; t < 10000; t++)
    {
        int n = 1 + gen() % 100;
        int a = gen() % n, b = a + gen() % (n - a);
        ll offset = (int)(gen() % 201) - 100;
        vector<ll> val(n);
        for (int i = a - 1; i >= 0; i--)
        {
            val[i] = val[i + 1] + 1 + gen() % 100;
        }
        for (int i = b + 1; i < n; i++)
        {
            val[i] = val[i - 1] + 1 + gen() % 100;
        }
        auto f = [&](ll p)
        {
            return val.at(p - offset);
        };
        ll expected = offset;
        for (int i = 1; i < n; i++)
        {
            if (val[i] < val[expected - offset]) expected = offset + i;
        }
        assert(ternary_search_int(offset, offset + n - 1, f) == expected);
        auto peak = [&](ll p)
        {
            return -f(p);
        };
        assert(ternary_search_int(offset, offset + n - 1, peak, greater<>()) == expected);
    }
    auto identity = [](ll p)
    {
        return p;
    };
    assert(ternary_search_int(LLONG_MIN, LLONG_MAX, identity) == LLONG_MIN);
    assert(ternary_search_int(LLONG_MIN, LLONG_MAX, identity, greater<>()) == LLONG_MAX);
    for (ll target : {LLONG_MIN, LLONG_MIN + 1, -7LL, 0LL, 7LL, LLONG_MAX - 1, LLONG_MAX})
    {
        auto distance = [&](ll p)
        {
            __int128 d = (__int128)p - target;
            return d < 0 ? -d : d;
        };
        assert(ternary_search_int(LLONG_MIN, LLONG_MAX, distance) == target);
        assert(ternary_search_int(target, target, identity) == target);
    }
    auto constant = [](ll)
    {
        return LLONG_MIN;
    };
    assert(ternary_search_int(LLONG_MIN, LLONG_MAX, constant) == LLONG_MIN);
    assert(ternary_search_int(LLONG_MAX - 3, LLONG_MAX, constant, greater<>()) == LLONG_MAX - 3);
    cout << "PASS integer ternary: 10000 brute-force min/max cases, plateaus and full ll range\n";
}
