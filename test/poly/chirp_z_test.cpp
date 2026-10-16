#include "code/poly/chirp_z.hpp"
int main()
{
    mt19937 gen(20261016);
    using namespace poly998;
    assert((chirp_z(Poly({1, 2}), 2, 3) == vector<int>{3, 5, 9}));
    {
        using namespace poly998;
        auto eval = [](const Poly &f, int x)
        {
            ll z = 0;
            for (int i = f.size() - 1; i >= 0; i--)
            {
                z = (z * x + f[i]) % MOD;
            }
            return int(z);
        };
        for (int t = 0; t < 300; t++)
        {
            vector<int> a(gen() % 70);
            for (int &x : a)
            {
                x = gen() % MOD;
            }
            Poly f(a);
            int q = gen() % 20, m = gen() % 80;
            auto z = chirp_z(f, q, m);
            int x = 1;
            for (int i = 0; i < m; i++)
            {
                assert(z[i] == eval(f, x));
                x = 1LL * x * q % MOD;
            }
        }
    }
    cout << "PASS chirp_z\n";
}
