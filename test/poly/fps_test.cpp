#include "code/poly/fps.hpp"
int main()
{
    mt19937 gen(20261016);
    using namespace poly998;
    Poly f({1, 2, 3});
    assert((f * f.inv(10)).mod_xk(10) == Poly(1));
    {
        using namespace poly998;
        for (int t = 0; t < 150; t++)
        {
            int n = gen() % 50 + 1;
            vector<int> a(n);
            for (int &x : a)
            {
                x = gen() % 100;
            }
            a[0] = 1;
            Poly f(a);
            assert((f * f.inv(n)).mod_xk(n) == Poly(1));
            assert(f.log(n).exp(n) == f.mod_xk(n));
            Poly p(1);
            for (int k = 0; k < 6; k++)
            {
                assert(f.pow(k, n) == p.mod_xk(n));
                p = (p * f).mod_xk(n);
            }
            auto rt = (f * f).mod_xk(n).sqrt(n);
            assert(rt && ((*rt) * (*rt)).mod_xk(n) == (f * f).mod_xk(n));
            a[0] = 0;
            Poly h(a);
            vector<int> ref(n);
            ref[0] = 1;
            for (int i = 1; i < n; i++)
            {
                ll z = 0;
                for (int j = 1; j <= i; j++)
                {
                    z = (z + 1LL * j * h[j] % MOD * ref[i - j]) % MOD;
                }
                ref[i] = z * invmod(i) % MOD;
            }
            assert(h.exp(n) == Poly(ref));
            assert((h.sin(n) * h.sin(n) + h.cos(n) * h.cos(n)).mod_xk(n) ==
                   Poly(1));
            assert(h.asin(n).sin(n) == h.mod_xk(n));
            assert(h.atan(n).tan(n) == h.mod_xk(n));
        }
        assert(!Poly({0, 1}).sqrt(4));
        assert(!Poly(3).sqrt(4));
        assert(Poly().sqrt(0));
    }
    cout << "PASS fps\n";
}
