#include "code/probability/distributions.hpp"

long double choose(int n, int k)
{
    if (k < 0 || k > n) return 0;
    long double ans = 1;
    for (int i = 1; i <= k; i++) ans = ans * (n - i + 1) / i;
    return ans;
}

int main()
{
    for (int n = 0; n <= 50; n++)
    {
        for (db p : {0.0, 0.01, 0.3, 0.5, 0.99, 1.0})
        {
            db sum = 0, e = 0, e2 = 0;
            for (int k = 0; k <= n; k++)
            {
                db v = binom_pmf(n, k, p);
                long double ref = choose(n, k) * pow((long double)p, k) * pow(1 - (long double)p, n - k);
                assert(abs(v - ref) < 1e-12);
                sum += v; e += k * v; e2 += k * k * v;
            }
            assert(abs(sum - 1) < 1e-11);
            assert(abs(e - n * p) < 1e-10);
            assert(abs(e2 - e * e - n * p * (1 - p)) < 1e-8);
        }
    }
    for (int n = 1; n <= 20; n++)
    {
        for (int m = 0; m <= n; m++)
        {
            for (int take = 0; take <= n; take++)
            {
                db sum = 0;
                for (int k = 0; k <= take; k++)
                {
                    db ref = choose(m, k) * choose(n - m, take - k) / choose(n, take);
                    assert(abs(hyper_pmf(n, m, take, k) - ref) < 1e-12);
                    sum += hyper_pmf(n, m, take, k);
                }
                assert(abs(sum - 1) < 1e-11);
            }
        }
    }
    db e = 0, sum = 0;
    for (int k = 1; k < 1000; k++) e += k * geom_pmf(k, 0.2);
    for (int k = 0; k < 100; k++) sum += poisson_pmf(k, 4);
    assert(abs(e - 5) < 1e-12 && abs(sum - 1) < 1e-12);
    assert(geom_pmf(1, 1) == 1 && geom_pmf(0, 1) == 0);
    assert(poisson_pmf(0, 0) == 1 && poisson_pmf(1, 0) == 0);
    assert(binom_pmf(3, -1, 0.5) == 0);
    assert(abs(binom_pmf(1000, 500, 0.5) - 0.0252250181783608) < 1e-11);
    cout << "PASS distributions: combinatorial PMFs, normalization, moments and endpoints\n";
}
