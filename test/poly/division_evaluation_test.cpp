#include "code/poly/division_evaluation.hpp"
int main()
{
    mt19937 gen(20261016);
    using namespace poly998;
    Poly f({1, 2, 3}), g({1, 1});
    auto [q, r] = f.divmod(g);
    assert(q * g + r == f);
    auto ys = f.eval({0, 1, 2});
    assert(Poly::interpolate({0, 1, 2}, ys) == f);
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
        for (int t = 0; t < 250; t++)
        {
            int n = gen() % 40, m = gen() % 20 + 1;
            vector<int> a(n), b(m), xs;
            for (int &x : a)
            {
                x = gen() % MOD;
            }
            for (int &x : b)
            {
                x = gen() % MOD;
            }
            b.back() = 1;
            Poly f(a), g(b);
            auto [q, r] = f.divmod(g);
            assert(q * g + r == f && r.size() < g.size());
            for (int i = 0; i < n + 3; i++)
            {
                xs.push_back(i);
            }
            auto ys = f.eval(xs);
            for (int i = 0; i < (int)xs.size(); i++)
            {
                assert(ys[i] == eval(f, xs[i]));
            }
            assert(Poly::interpolate(xs, ys) == f);
        }
        assert(!Poly::interpolate({1, 1}, {2, 2}));
    }
    cout << "PASS division_evaluation\n";
}
