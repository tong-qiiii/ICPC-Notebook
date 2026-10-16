#include "code/math/div_any.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(div_any(10, vector<ll>{2, 3}) == 7);
    {
        for (int t = 0; t < 300; t++)
        {
            int n = gen() % 200;
            vector<ll> d;
            for (int j = gen() % 8; j; j--)
            {
                d.push_back(gen() % 20 + 1);
            }
            int z = 0;
            for (int i = 1; i <= n; i++)
            {
                bool ok = false;
                for (ll v : d)
                {
                    ok |= i % v == 0;
                }
                z += ok;
            }
            assert(div_any(n, d) == z);
        }
    }
    cout << "PASS div_any\n";
}
