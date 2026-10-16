#include "code/math/qpow.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(qpow(-2, 5, 101) == 69 && qpow(0, 0, 1) == 0);
    {
        for (ll m = 1; m < 150; m++)
        {
            for (ll a = -10; a <= 10; a++)
            {
                ll z = 1 % m;
                for (int b = 0; b < 30; b++)
                {
                    assert(qpow(a, b, m) == z);
                    z = (__int128)z * ((a % m + m) % m) % m;
                }
            }
        }
    }
    cout << "PASS qpow\n";
}
