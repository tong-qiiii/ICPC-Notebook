#include "code/counting/euler_transform.hpp"
#include "code/math/modint.hpp"

int main()
{
    mt19937 gen(1203);
    using mint = Mint<998244353>;
    for (int t = 0; t < 200; t++)
    {
        int n = gen() % 40;
        vector<mint> a(n + 1), ref(n + 1);
        ref[0] = 1;
        for (int i = 1; i <= n; i++)
        {
            int types = gen() % 4; a[i] = types;
            // 独立逐种完全背包，验证 Euler transform 的组合意义。
            for (int k = 0; k < types; k++)
            {
                for (int j = i; j <= n; j++) ref[j] += ref[j - i];
            }
        }
        auto b = euler_transform(a);
        assert(b == ref && euler_inverse(b) == a);
    }
    vector<ll> a(11, 1); a[0] = 0;
    auto b = euler_transform(a);
    assert((b == vector<ll>{1, 1, 2, 3, 5, 7, 11, 15, 22, 30, 42}));
    assert(euler_inverse(b) == a);
    assert(euler_transform(vector<ll>{0}) == vector<ll>{1});
    cout << "PASS Euler transform: 200 colored partition comparisons, inverse and exact division\n";
}
