#include "code/poly/consecutive_shift.hpp"
int main()
{
    mt19937 gen(20261016);
    using namespace poly998;
    assert((consecutive_value_shift({1, 3}, 2, 3) == vector<int>{5, 7, 9}));
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
            int n = gen() % 35 + 1;
            vector<int> a(n), ys(n);
            for (int &x : a)
            {
                x = gen() % MOD;
            }
            Poly f(a);
            for (int i = 0; i < n; i++)
            {
                ys[i] = eval(f, i);
            }
            ll c = (t % 3 == 0 ? -15 : (t % 3 == 1 ? MOD - 10 : 10)),
               m = 60;
            auto z = consecutive_value_shift(ys, c, m);
            for (int i = 0; i < m; i++)
            {
                assert(z[i] == eval(f, norm(c + i)));
            }
        }
        assert(consecutive_value_shift({}, LLONG_MAX, 3) == vector<int>(3));
    }
    cout << "PASS consecutive_shift\n";
}
