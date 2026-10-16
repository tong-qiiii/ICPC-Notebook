#include "code/math/pollard_rho.hpp"
int main()
{
    mt19937 gen(20261016);
    auto f = factorize(360);
    assert((f == vector<u64>{2, 2, 2, 3, 3, 5}));
    {
        rng.seed(123);
        for (u64 n :
             vector<u64>{1, 2, 4, 1000000007ULL * 1000000009ULL,
                         18446744073709551615ULL, 18446744073709551557ULL})
        {
            auto f = factorize(n);
            u128 prod = 1;
            for (auto p : f)
            {
                assert(isPrime(p));
                prod *= p;
            }
            assert(prod == n);
        }
        for (int t = 0; t < 100; t++)
        {
            u64 n = gen() % 10000000 + 1;
            auto f = factorize(n);
            u128 prod = 1;
            for (auto p : f)
            {
                assert(isPrime(p));
                prod *= p;
            }
            assert(prod == n);
        }
    }
    cout << "PASS pollard_rho\n";
}
