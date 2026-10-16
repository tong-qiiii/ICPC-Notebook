#include "code/math/crt.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(
        (crt(vector<ll>{2, 3}, vector<ll>{3, 5}) == pair<ll, ll>{8, 15}));
    {
        for (int t = 0; t < 500; t++)
        {
            vector<ll> m{3, 5, 7}, r;
            for (ll x : m)
            {
                r.push_back(int(gen() % 101) - 50);
            }
            auto [a, mod] = crt(r, m);
            assert(mod == 105 && a >= 0 && a < mod);
            for (int i = 0; i < 3; i++)
            {
                assert((a - r[i]) % m[i] == 0);
            }
        }
        assert(crt({}, {}).first == 0);
    }
    cout << "PASS crt\n";
}
