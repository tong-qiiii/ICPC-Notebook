#include "code/poly/fast_interpolation.hpp"
int main()
{
    mt19937 gen(20261016);
    assert((fastInterpolation({0, 1, 2}, {1, 6, 17}) == Poly{1, 2, 3}));
    {
        for (int t = 0; t < 200; t++)
        {
            int n = gen() % 80;
            Poly a(n);
            vector<int> x(n), y(n);
            for (int &v : a)
            {
                v = gen() % MOD;
            }
            for (int i = 0; i < n; i++)
            {
                x[i] = i - 30;
                ll z = 0;
                for (int j = n - 1; j >= 0; j--)
                {
                    z = ((z * ((x[i] % MOD + MOD) % MOD) + a[j]) % MOD);
                }
                y[i] = z;
            }
            assert(fastInterpolation(x, y) == a);
        }
    }
    cout << "PASS fast_interpolation\n";
}
