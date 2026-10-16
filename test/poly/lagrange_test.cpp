#include "code/poly/lagrange.hpp"
int main()
{
    mt19937 gen(20261016);
    Lagrange l({0, 1, 2}, {1, 6, 17});
    assert(l.query(3) == 34);
    {
        for (int t = 0; t < 150; t++)
        {
            int n = gen() % 30;
            vector<int> a(n), x(n), y(n);
            for (int &v : a)
            {
                v = gen() % MOD;
            }
            auto eval = [&](ll q)
            {
                ll z = 0;
                q = (q % MOD + MOD) % MOD;
                for (int i = n - 1; i >= 0; i--)
                {
                    z = (z * q + a[i]) % MOD;
                }
                return z;
            };
            for (int i = 0; i < n; i++)
            {
                x[i] = i - 10;
                y[i] = eval(x[i]);
            }
            Lagrange l(x, y);
            for (int i = -20; i < 50; i++)
            {
                assert(l.query(i) == eval(i));
            }
        }
    }
    cout << "PASS lagrange\n";
}
