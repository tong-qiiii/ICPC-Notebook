#include "code/poly/berlekamp_massey.hpp"
int main()
{
    mt19937 gen(20261016);
    using namespace poly998;
    assert(
        (berlekamp_massey({0, 1, 1, 2, 3, 5, 8, 13}) == vector<int>{1, 1}));
    {
        using namespace poly998;
        for (int t = 0; t < 100; t++)
        {
            int d = gen() % 8 + 1;
            vector<int> a(100), coef(d);
            for (int &x : coef)
            {
                x = gen() % 10;
            }
            for (int i = 0; i < d; i++)
            {
                a[i] = gen() % 100;
            }
            for (int i = d; i < 100; i++)
            {
                for (int j = 0; j < d; j++)
                {
                    a[i] = (a[i] + 1LL * coef[j] * a[i - j - 1]) % MOD;
                }
            }
            auto rec =
                berlekamp_massey(vector<int>(a.begin(), a.begin() + 40));
            for (int i = 40; i < 100; i++)
            {
                ll z = 0;
                for (int j = 0; j < (int)rec.size(); j++)
                {
                    z = (z + 1LL * rec[j] * a[i - j - 1]) % MOD;
                }
                assert(z == a[i]);
            }
        }
    }
    cout << "PASS berlekamp_massey\n";
}
