#include "code/basic/binary_search_real.hpp"

int main()
{
    mt19937_64 gen(20261016);
    for (int t = 0; t < 10000; t++)
    {
        db l = (int)(gen() % 200001) - 100000;
        db r = l + 1 + gen() % 100000;
        db target = l + (r - l) * (gen() % 10001) / 10000;
        auto check = [&](db p)
        {
            return p >= target;
        };
        db ans = binary_search_real(l, r, check);
        assert(check(ans) && ans >= l && ans <= r);
        assert(abs(ans - target) <= 1e-9);
    }
    auto square = [](db p)
    {
        return p * p >= 2;
    };
    db ans = binary_search_real(0, 2, square);
    assert(square(ans) && abs(ans - sqrt(2.0)) < 1e-12);
    assert(binary_search_real(-5, 8, [](db) { return true; }) == -5);
    assert(binary_search_real(7, 7, [](db p) { return p >= 7; }) == 7);
    assert(binary_search_real(-3, 4, [](db p) { return p >= 4; }) == 4);
    assert(binary_search_real(0, 8, [](db p) { return p >= 3; }, 1) == 4);

    for (db l : {0.0, 1.0, -1.0, 1e200})
    {
        db r = nextafter(l, numeric_limits<db>::infinity());
        int cnt = 0;
        ans = binary_search_real(l, r, [&](db p)
        {
            cnt++;
            return p >= r;
        });
        assert(ans == r && cnt <= 3);
    }
    db target = 1.5e308;
    ans = binary_search_real(1e308, 1.7e308, [&](db p)
    {
        return p >= target;
    });
    assert(isfinite(ans) && ans >= target);
    assert(abs(ans / target - 1) < 1e-14);
    cout << "PASS real binary: 10000 thresholds, endpoints, precision and repeated calls\n";
}
