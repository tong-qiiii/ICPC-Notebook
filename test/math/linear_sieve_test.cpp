#include "code/math/linear_sieve.hpp"
int main()
{
    mt19937 gen(20261016);
    LinearSieve s(10);
    assert((s.primes == vector<int>{2, 3, 5, 7}) && s.minp[9] == 3);
    {
        LinearSieve s;
        for (int n = 0; n < 300; n++)
        {
            s.init(n);
            for (int i = 2; i <= n; i++)
            {
                int p = 2;
                while (i % p)
                {
                    p++;
                }
                assert(s.minp[i] == p);
            }
        }
    }
    cout << "PASS linear_sieve\n";
}
