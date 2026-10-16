#include "code/basic/ternary_search_real.hpp"

int main()
{
    mt19937_64 gen(20261018);
    for (int t = 0; t < 10000; t++)
    {
        db l = (int)(gen() % 2001) - 1000;
        db r = l + 1 + gen() % 1000;
        db target = l - 100 + (r - l + 200) * (gen() % 10001) / 10000;
        db expected = clamp(target, l, r);
        auto f = [&](db p)
        {
            return (p - target) * (p - target);
        };
        db ans = ternary_search_real(l, r, f);
        assert(ans >= l && ans <= r && abs(ans - expected) < 1e-6);
        auto peak = [&](db p)
        {
            return -f(p);
        };
        ans = ternary_search_real(l, r, peak, greater<>());
        assert(ans >= l && ans <= r && abs(ans - expected) < 1e-6);
        db a = l + (r - l) / 3, b = l + 2 * (r - l) / 3;
        ans = ternary_search_real(l, r, [&](db p)
        {
            return max({a - p, 0.0, p - b});
        });
        assert(ans >= a - 1e-8 && ans <= b + 1e-8);
    }
    int cnt = 0;
    assert(ternary_search_real(7, 7, [&](db p)
    {
        cnt++;
        return p;
    }) == 7);
    assert(cnt == 0);
    auto identity = [](db p)
    {
        return p;
    };
    db r = nextafter(1.0, 2.0);
    db ans = ternary_search_real(1, r, identity);
    assert(ans >= 1 && ans <= r);
    ans = ternary_search_real(-5, 8, [](db) { return 0; });
    assert(ans >= -5 && ans <= 8);
    ans = ternary_search_real(1e308, 1.7e308, identity, greater<>(), 120);
    assert(isfinite(ans) && abs(ans / 1.7e308 - 1) < 1e-14);
    ans = ternary_search_real(0, 9, identity, less<>(), 1);
    assert(ans == 3);
    cout << "PASS real ternary: 10000 analytic min/max and plateau cases, endpoints and precision\n";
}
