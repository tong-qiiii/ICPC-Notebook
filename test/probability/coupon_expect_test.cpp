#include "code/probability/coupon_expect.hpp"

int main()
{
    mt19937 gen(1108);
    for (int n = 1; n <= 8; n++)
    {
        vector<db> p(n + 1, 1.0 / n);
        assert(abs(coupon_expect(p) - coupon_uniform(n)) < 1e-10);
        for (int t = 0; t < 30; t++)
        {
            db sum = 1;
            for (int i = 1; i <= n; i++) p[i] = 1 + gen() % 10, sum += p[i];
            for (int i = 1; i <= n; i++) p[i] /= sum;
            long double ref = 0;
            for (int s = 1; s < (1 << n); s++)
            {
                long double den = 0;
                for (int i = 1; i <= n; i++) if (s >> (i - 1) & 1) den += p[i];
                ref += (__builtin_popcount((unsigned)s) & 1 ? 1 : -1) / den;
            }
            assert(abs(coupon_expect(p) - ref) < 1e-9);
        }
    }
    assert(coupon_uniform(0) == 0 && coupon_expect({}) == 0);
    assert(isinf(coupon_expect({0, 0, 1})));
    assert(abs(coupon_expect({0, 1e-12}) / 1e12 - 1) < 1e-12);
    cout << "PASS coupon: uniform formulas, 240 inclusion-exclusion comparisons, zero/tiny chances\n";
}
