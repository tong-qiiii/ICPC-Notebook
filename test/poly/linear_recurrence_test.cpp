#include "code/poly/linear_recurrence.hpp"
int main()
{
    mt19937 gen(20261016);
    using namespace poly998;
    assert(linear_recurrence_nth({0, 1}, {1, 1}, 10) == 55);
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
            vector<int> init(a.begin(), a.begin() + d);
            for (int k = 0; k < 100; k++)
            {
                assert(linear_recurrence_nth(init, coef, k) == a[k]);
            }
        }
    }
    cout << "PASS linear_recurrence\n";
}
