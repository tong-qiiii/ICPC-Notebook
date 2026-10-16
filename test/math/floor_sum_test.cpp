#include "code/math/floor_sum.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(floorSum(4, 3, 2, -1) == 1);
    {
        ll n = LLONG_MAX;
        i128 expected = (i128)n * (n - 1) / 2 * 3;
        assert(floorSum(n, 1, 3, 0) == expected);
        assert(floorSum(n, 1, -3, 0) == -expected);
    }
    {
        for (ll n = 0; n < 20; n++)
        {
            for (ll m = 1; m < 15; m++)
            {
                for (ll a = -15; a < 15; a++)
                {
                    for (ll b = -15; b < 15; b++)
                    {
                        i128 z = 0;
                        for (ll i = 0; i < n; i++)
                        {
                            ll x = a * i + b, q = x / m;
                            if (x < 0 && x % m)
                            {
                                q--;
                            }
                            z += q;
                        }
                        assert(floorSum(n, m, a, b) == z);
                    }
                }
            }
        }
    }
    cout << "PASS floor_sum\n";
}
