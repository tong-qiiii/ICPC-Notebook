#include "code/poly/bostan_mori.hpp"
int main()
{
    mt19937 gen(20261016);
    using namespace poly998;
    assert(bostan_mori(Poly(1), Poly({1, -1}), 1000000000000LL) == 1);
    {
        using namespace poly998;
        for (int t = 0; t < 100; t++)
        {
            int d = gen() % 10 + 1;
            vector<int> a(d), b(d + 1);
            for (int &x : a)
            {
                x = gen() % MOD;
            }
            for (int &x : b)
            {
                x = gen() % MOD;
            }
            b[0] = 1;
            Poly P(a), Q(b);
            auto seq = (P * Q.inv(100)).mod_xk(100);
            for (int k = 0; k < 100; k++)
            {
                assert(bostan_mori(P, Q, k) == seq[k]);
            }
        }
        assert(bostan_mori(Poly({1, 2, 3}), Poly(1), 1) == 2);
    }
    cout << "PASS bostan_mori\n";
}
