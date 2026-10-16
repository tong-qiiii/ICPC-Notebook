#include "code/math/miller_rabin.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(isPrime(18446744073709551557ULL) &&
           !isPrime(341550071728321ULL));
    {
        for (uint64_t n = 0; n < 30000; n++)
        {
            bool ok = n >= 2;
            for (uint64_t d = 2; d * d <= n; d++)
            {
                if (n % d == 0)
                {
                    ok = false;
                }
            }
            assert(isPrime(n) == ok);
        }
    }
    cout << "PASS miller_rabin\n";
}
