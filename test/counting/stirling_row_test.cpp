#include "code/counting/stirling_row.hpp"
#include "code/counting/stirling.hpp"
#include "code/math/modint.hpp"

int main()
{
    using Z = Mint<998244353>;
    Stirling<Z> all(60);
    for (int n = 0; n <= 60; n++)
    {
        for (int k = 0; k <= n; k++)
        {
            auto a = stirling_row<Z>(n, k);
            auto b = stirling_row<Z>(n, k, true);
            for (int j = 0; j <= k; j++)
            {
                assert(a[j] == all.s2[n][j]);
                assert(b[j] == all.s1[n][j]);
            }
        }
    }
    assert((stirling_row<ll>(4) == vector<ll>{0, 1, 7, 6, 1}));
    assert((stirling_row<ll>(4, 2, true) == vector<ll>{0, 6, 11}));
    assert(stirling_row<ll>(0)[0] == 1 && stirling_row<ll>(20, 0)[0] == 0);
    int n = 100000;
    auto a = stirling_row<Z>(n, 2);
    assert(a[2] == Z(2).pow(n - 1) - Z(1));
    auto b = stirling_row<Z>(n, 1, true);
    Z fact = 1;
    for (int i = 1; i < n; i++) fact *= i;
    assert(b[1] == fact);
    // 合数模仅用加乘，不需要逆元。
    using C = Mint<1000>;
    auto c = stirling_row<C>(4);
    assert(c[2].v == 7);
    cout << "PASS Stirling row: all n,k<=60 vs full table, large n/small k, composite modulus\n";
}
