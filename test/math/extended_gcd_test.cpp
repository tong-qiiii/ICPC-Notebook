#include "code/math/extended_gcd.hpp"
int main()
{
    mt19937 gen(20261016);
    ll x, y;
    ll g = exgcd(30, 18, x, y);
    assert(g == 6 && 30 * x + 18 * y == 6);
    {
        for (ll a = 0; a < 200; a++)
        {
            for (ll b = 0; b < 200; b++)
            {
                ll x, y;
                ll g = exgcd(a, b, x, y);
                assert(g == gcd(a, b) && a * x + b * y == g);
            }
        }
    }
    cout << "PASS extended_gcd\n";
}
