#include "code/dp/sos_dp.hpp"
int main()
{
    mt19937 gen(20261016);
    vector<ll> a{1, 2, 3, 4};
    sosSubset(a, 2);
    assert((a == vector<ll>{1, 3, 4, 10}));
    mobiusSubset(a, 2);
    assert((a == vector<ll>{1, 2, 3, 4}));
    {
        for (int n = 0; n <= 9; n++)
        {
            vector<ll> a(1 << n);
            for (auto &v : a)
            {
                v = int(gen() % 101) - 50;
            }
            auto b = a, c = a;
            sosSubset(b, n);
            sosSuperset(c, n);
            for (int mask = 0; mask < (1 << n); mask++)
            {
                ll s = 0, t = 0;
                for (int sub = 0; sub < (1 << n); sub++)
                {
                    if ((sub & mask) == sub)
                    {
                        s += a[sub];
                    }
                    if ((sub & mask) == mask)
                    {
                        t += a[sub];
                    }
                }
                assert(b[mask] == s && c[mask] == t);
            }
            mobiusSubset(b, n);
            mobiusSuperset(c, n);
            assert(a == b && a == c);
        }
    }
    cout << "PASS sos_dp\n";
}
