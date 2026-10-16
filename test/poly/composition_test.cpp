#include "code/poly/composition.hpp"
int main()
{
    mt19937 gen(20261016);
    using namespace poly998;
    Poly f({0, 1, 1});
    auto g = f.compositional_inverse_slow(10);
    assert(f.compose_slow(g, 10) == Poly({0, 1}));
    {
        using namespace poly998;
        for (int t = 0; t < 100; t++)
        {
            int n = gen() % 20 + 2;
            vector<int> a(n);
            for (int &x : a)
            {
                x = gen() % 20;
            }
            a[0] = 0;
            a[1] = gen() % 100 + 1;
            Poly f(a);
            auto g = f.compositional_inverse_slow(n);
            assert(f.compose_slow(g, n) == Poly({0, 1}));
            for (int k = 1; k < n; k++)
            {
                assert(lagrange_inverse_coeff(f, k) == g[k]);
            }
        }
        assert(lagrange_y_power_coeff(Poly({1, 1}), 6, 1) == 1);
    }
    cout << "PASS composition\n";
}
