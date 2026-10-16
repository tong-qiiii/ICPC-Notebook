#include "code/math/mod_sqrt.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(modSqrt(4, 7) == 2 || modSqrt(4, 7) == 5);
    assert(modSqrt(3, 7) == -1);
    {
        for (ll p = 2; p < 400; p++)
        {
            bool prime = true;
            for (ll d = 2; d * d <= p; d++)
            {
                if (p % d == 0)
                {
                    prime = false;
                }
            }
            if (!prime)
            {
                continue;
            }
            for (ll n = -p; n < p; n++)
            {
                ll r = modSqrt(n, p);
                bool ok = false;
                for (ll x = 0; x < p; x++)
                {
                    ok |= x * x % p == (n % p + p) % p;
                }
                assert((r != -1) == ok);
                if (ok)
                {
                    assert(r * r % p == (n % p + p) % p);
                }
            }
        }
    }
    cout << "PASS mod_sqrt\n";
}
