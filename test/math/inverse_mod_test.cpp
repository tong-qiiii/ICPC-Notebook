#include "code/math/inverse_mod.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(inv_mod(3, 7) == 5 && inv_mod(2, 6) == -1);
    {
        for (ll m = 1; m < 200; m++)
        {
            for (ll a = -100; a < 100; a++)
            {
                ll x = inv_mod(a, m);
                if (gcd(a, m) != 1)
                {
                    assert(x == -1);
                }
                else
                {
                    assert(((__int128)a * x % m + m) % m == 1 % m);
                }
            }
        }
    }
    cout << "PASS inverse_mod\n";
}
