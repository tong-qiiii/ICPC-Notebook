#include "code/poly/poly_core.hpp"
int main()
{
    mt19937 gen(20261016);
    using namespace poly998;
    Poly f({1, 2, 3}), g({4, 5});
    assert((f * g)[2] == 22);
    assert(Poly({-1, MOD, 0}) == Poly(MOD - 1));
    assert(f[-1] == 0 && f[100] == 0 && f.deg() == 2);
    assert(f.rev(5) == Poly({0, 0, 3, 2, 1}));
    assert(f.rev(2) == Poly({2, 1}));
    assert(f.mul_xk(3).div_xk(3) == f);
    assert(f.div_xk(20).empty() && f.mod_xk(0).empty());
    assert(f + g - g == f && f * -7 / -7 == f && -(-f) == f);
    assert(poly998::operator+(f, g) == f + g);
    assert(poly998::operator*(f, 3) == 3 * f);
    Poly z = f;
    z += g; z -= g; z *= 2; z /= 2;
    assert(z == f);
    ensure_fact(0);
    ensure_fact(100);
    ensure_fact(3);
    ensure_fact(200);
    for (int i = 0; i <= 200; i++) assert(1LL * fac[i] * ifac[i] % MOD == 1);
    auto iv = batch_inv({0, 2, -3, MOD});
    assert(iv[0] == 0 && iv[3] == 0 && 2LL * iv[1] % MOD == 1);
    assert(1LL * (MOD - 3) * iv[2] % MOD == 1);
    assert(batch_inv({}).empty());
    assert(mod_sqrt(0) == 0 && !mod_sqrt(G));
    for (int i = 0; i < 300; i++)
    {
        int a = gen() % MOD;
        auto r = mod_sqrt(1LL * a * a % MOD);
        assert(r && 1LL * *r * *r % MOD == 1LL * a * a % MOD);
    }
    {
        using namespace poly998;
        for (int t = 0; t < 300; t++)
        {
            int n = gen() % 150, m = gen() % 150;
            vector<int> a(n), b(m);
            for (int &x : a)
            {
                x = int(gen() % 2001) - 1000;
            }
            for (int &x : b)
            {
                x = int(gen() % 2001) - 1000;
            }
            auto got = convolution(a, b);
            vector<int> ref(n && m ? n + m - 1 : 0);
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    ref[i + j] =
                        (ref[i + j] + 1LL * poly998::norm(a[i]) * poly998::norm(b[j])) % MOD;
                }
            }
            assert(got == ref);
        }
        for (int k = 0; k <= 12; k++)
        {
            vector<int> a(1 << k);
            for (int &x : a)
            {
                x = gen() % MOD;
            }
            auto b = a;
            ntt(b, false);
            ntt(b, true);
            assert(a == b);
        }
        assert(Poly({0, 0}).empty());
    }
    // Exercise the exact modulus capacity, not just the small convolution path.
    {
        using namespace poly998;
        const int n = 1 << MAX_LOG;
        vector<int> a(n, 1);
        ntt(a, false);
        assert(a[0] == n);
        for (int i = 1; i < n; ++i)
        {
            assert(a[i] == 0);
        }
        ntt(a, true);
        for (int v : a)
        {
            assert(v == 1);
        }
    }
    cout << "PASS poly_core\n";
}
