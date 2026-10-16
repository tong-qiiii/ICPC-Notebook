#include "code/poly/poly_core.hpp"
int main()
{
    mt19937 gen(20261016);
    using namespace poly998;
    Poly f({1, 2, 3}), g({4, 5});
    assert((f * g)[2] == 22);
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
