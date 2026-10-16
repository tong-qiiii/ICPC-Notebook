#include "code/poly/fwt_and.hpp"
int main()
{
    mt19937 gen(20261016);
    using namespace poly998;
    auto r = and_convolution({1, 2}, {3, 4});
    assert(r.size() == 2);
    {
        using namespace poly998;
        for (int t = 0; t < 300; t++)
        {
            int n = gen() % 40 + 1, m = gen() % 40 + 1, N = 1;
            while (N < max(n, m))
            {
                N *= 2;
            }
            vector<int> a(n), b(m), ref(N);
            for (int &x : a)
            {
                x = gen() % MOD;
            }
            for (int &x : b)
            {
                x = gen() % MOD;
            }
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    ref[i & j] = (ref[i & j] + 1LL * a[i] * b[j]) % MOD;
                }
            }
            assert(and_convolution(a, b) == ref);
        }
    }
    cout << "PASS fwt_and\n";
}
