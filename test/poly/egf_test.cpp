#include "code/poly/egf.hpp"
int main()
{
    mt19937 gen(20261016);
    using namespace poly998;
    assert(egf_from_sequence({1, 2, 6}) == Poly({1, 2, 3}));
    {
        using namespace poly998;
        for (int n = 0; n < 200; n++)
        {
            vector<int> a(n);
            for (int &x : a)
            {
                x = gen() % MOD;
            }
            auto f = egf_from_sequence(a);
            ensure_fact(n);
            for (int i = 0; i < n; i++)
            {
                assert(1LL * f[i] * fac[i] % MOD == a[i]);
            }
        }
    }
    cout << "PASS egf\n";
}
