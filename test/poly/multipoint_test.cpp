#include "code/poly/multipoint.hpp"
int main()
{
    mt19937 gen(20261016);
    Multipoint t(vector<int>{0, 1, 2});
    assert((t.eval(Poly{1, 2, 3}) == vector<int>{1, 6, 17}));
    {
        for (int run = 0; run < 200; run++)
        {
            int n = gen() % 80, m = gen() % 90;
            Poly f(n);
            vector<int> xs(m);
            for (int &x : f)
            {
                x = gen() % MOD;
            }
            for (int &x : xs)
            {
                x = gen() % MOD;
            }
            Multipoint t(xs);
            auto got = t.eval(f);
            assert(got == t.eval(f));
            for (int i = 0; i < m; i++)
            {
                ll z = 0;
                for (int j = n - 1; j >= 0; j--)
                {
                    z = (z * xs[i] + f[j]) % MOD;
                }
                assert(got[i] == z);
            }
        }
    }
    cout << "PASS multipoint\n";
}
