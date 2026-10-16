#include "code/counting/binomial_transform.hpp"
#include "code/math/combination.hpp"

int main()
{
    using mint = Mint<998244353>;
    Comb<998244353> cb(50);
    auto choose = [&](int n, int k) { return cb.C(n, k); };
    mt19937 gen(1201);
    for (int n = 0; n <= 50; n++)
    {
        vector<mint> a(n);
        for (auto &v : a) v = gen();
        for (bool upper : {false, true})
        {
            auto b = binom_transform(a, choose, false, upper);
            assert(binom_transform(b, choose, true, upper) == a);
        }
    }
    // 常序列 1 -> 2^n -> 1；交集计数 -> 恰好属于 k 个集合。
    vector<mint> one(20, 1);
    auto b = binom_transform(one, choose);
    for (int i = 0; i < 20; i++) assert(b[i] == mint(2).pow(i));
    vector<mint> exact{2, 3, 5, 7};
    auto inter = binom_transform(exact, choose, false, true);
    assert(inter[0] == 17 && inter[1] == 34 && inter[2] == 26 && inter[3] == 7);
    assert(binom_transform(inter, choose, true, true) == exact);
    cout << "PASS binomial transforms: both directions, random modular round trips and identities\n";
}
