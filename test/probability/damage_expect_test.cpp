#include "code/probability/damage_expect.hpp"
#include "code/math/modint.hpp"

int main()
{
    using mint = Mint<998244353>;
    assert(damage_expect(3, mint(10) / 100)[3].v == 229596204);
    assert(damage_expect(5, mint(1))[5].v == 3);
    assert(damage_expect(280, mint(59) / 100)[280].v == 567484387);
    for (int n = 0; n <= 10; n++)
    {
        for (double p : {0.0, .2, .5, .8, 1.0})
        {
            double ref = 0;
            auto dfs = [&](auto &&self, int hp, int cnt, double w) -> void
            {
                if (hp <= 0) { ref += cnt * w; return; }
                self(self, hp - 1, cnt + 1, w * (1 - p));
                self(self, hp - 2, cnt + 1, w * p);
            };
            dfs(dfs, n, 0, 1);
            assert(abs(damage_expect(n, p)[n] - ref) < 1e-10);
        }
    }
    cout << "PASS damage: exhaustive stopping paths and all official Critical Hit samples\n";
}
