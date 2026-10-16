#include "code/probability/sugoroku_expect.hpp"
#include "code/math/modint.hpp"

int main()
{
    using mint = Mint<998244353>;
    assert(sugoroku_expect<mint>({0, 1, 1})[1].v == 4);
    assert(sugoroku_expect<mint>({0, 3, 1, 2, 1})[1].v == 332748122);
    mt19937 gen(1104);
    for (int t = 0; t < 200; t++)
    {
        int n = 2 + gen() % 50;
        vector<int> a(n);
        vector<double> ref(n + 1);
        for (int i = n - 1; i >= 1; i--)
        {
            a[i] = 1 + gen() % (n - i);
            ref[i] = a[i] + 1;
            for (int j = i + 1; j <= i + a[i]; j++) ref[i] += ref[j];
            ref[i] /= a[i];
        }
        auto ans = sugoroku_expect<double>(a);
        for (int i = 1; i <= n; i++) assert(abs(ans[i] - ref[i]) < 1e-9);
    }
    assert(sugoroku_expect<double>({0})[1] == 0);
    cout << "PASS Sugoroku expectation: 200 naive comparisons and both official samples\n";
}
