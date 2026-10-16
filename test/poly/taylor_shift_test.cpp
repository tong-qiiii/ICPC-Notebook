#include "code/poly/taylor_shift.hpp"
int main()
{
    mt19937 gen(20261016);
    using namespace poly998;
    assert(Poly({1, 2, 3}).shift(1) == Poly({6, 8, 3}));
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
            vector<int> a(gen() % 40);
            for (int &x : a)
            {
                x = gen() % MOD;
            }
            Poly f(a);
            int c = gen() % 100;
            auto g = f.shift(c);
            for (int x = 0; x < 20; x++)
            {
                assert(eval(g, x) == eval(f, x + c));
            }
        }
    }
    cout << "PASS taylor_shift\n";
}
