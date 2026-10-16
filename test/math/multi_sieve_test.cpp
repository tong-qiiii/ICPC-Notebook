#include "code/math/multi_sieve.hpp"
int main()
{
    mt19937 gen(20261016);
    MultiSieve s(10);
    assert(s.phi[10] == 4 && s.mu[6] == 1 && s.mu[4] == 0);
    {
        MultiSieve s;
        for (int n = 0; n < 150; n++)
        {
            s.init(n);
            for (int i = 1; i <= n; i++)
            {
                int phi = 0;
                for (int j = 1; j <= i; j++)
                {
                    phi += gcd(i, j) == 1;
                }
                assert(s.phi[i] == phi);
                int sum = 0;
                for (int d = 1; d <= i; d++)
                {
                    if (i % d == 0)
                    {
                        sum += s.mu[d];
                    }
                }
                assert(sum == (i == 1));
            }
        }
    }
    cout << "PASS multi_sieve\n";
}
