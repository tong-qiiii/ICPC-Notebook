#include "code/poly/ntt.hpp"

int main()
{
    using namespace poly998;
    mt19937 gen(712367);
    assert(convolution({}, {1}).empty());
    assert(convolution({-1}, {2}) == vector<int>{MOD - 2});
    for (int t = 0; t < 400; t++)
    {
        int n = gen() % 180, m = gen() % 180;
        vector<int> a(n), b(m), ref(n && m ? n + m - 1 : 0);
        for (int &x : a) x = int(gen() % 2000000001) - 1000000000;
        for (int &x : b) x = int(gen() % 2000000001) - 1000000000;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                ref[i + j] = (ref[i + j] + 1LL * poly998::norm(a[i]) *
                              poly998::norm(b[j])) % MOD;
        assert(convolution(a, b) == ref);
    }
    for (int k = 0; k <= 14; k++)
    {
        vector<int> a(1 << k);
        for (int &x : a) x = gen() % MOD;
        auto b = a;
        ntt(b, false);
        ntt(b, true);
        assert(a == b);
    }
    cout << "PASS ntt: signed convolution oracle / round trips\n";
}
