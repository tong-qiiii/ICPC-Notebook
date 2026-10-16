#include "code/counting/divisor_transform.hpp"
#include "code/math/multi_sieve.hpp"
#include "code/math/modint.hpp"

int main()
{
    mt19937 gen(1202);
    MultiSieve sieve(200);
    for (int n = 0; n <= 200; n++)
    {
        vector<ll> a(n + 1), div(n + 1), mul(n + 1);
        for (int i = 1; i <= n; i++) a[i] = (int)(gen() % 2001) - 1000;
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (i % j == 0) div[i] += a[j];
                if (j % i == 0) mul[i] += a[j];
            }
        }
        assert(divisor_transform(a) == div);
        assert(multiple_transform(a) == mul);
        assert(divisor_transform(div, true) == a);
        assert(multiple_transform(mul, true) == a);
        for (int i = 1; i <= n; i++)
        {
            ll ref = 0;
            for (int d = 1; d <= i; d++) if (i % d == 0) ref += sieve.mu[d] * div[i / d];
            assert(ref == a[i]);
        }
        vector<Mint<998244353>> am(a.begin(), a.end());
        assert(divisor_transform(divisor_transform(am), true) == am);
        assert(multiple_transform(multiple_transform(am), true) == am);
    }
    cout << "PASS divisor/multiple transforms: direct enumeration, mu formula, modular and empty cases\n";
}
