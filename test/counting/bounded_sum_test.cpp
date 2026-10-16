#include "code/counting/bounded_sum.hpp"
#include "code/math/modint.hpp"

int main()
{
    mt19937 gen(1204);
    for (int t = 0; t < 300; t++)
    {
        int k = gen() % 7;
        vector<int> u(k + 1);
        for (int i = 1; i <= k; i++) u[i] = gen() % 5;
        vector<ll> ref(25);
        auto dfs = [&](auto &&self, int i, int sum) -> void
        {
            if (i > k) { ref[sum]++; return; }
            for (int v = 0; v <= u[i]; v++) self(self, i + 1, sum + v);
        };
        dfs(dfs, 1, 0);
        for (int s = 0; s <= 24; s++)
        {
            assert(bounded_sum<ll>(u, s) == ref[s]);
            assert(bounded_sum<Mint<998244353>>(u, s) == ref[s]);
        }
    }
    assert(bounded_sum<ll>({}, 0) == 1 && bounded_sum<ll>({0, 3}, -1) == 0);
    cout << "PASS bounded sum: 300 exhaustive bounds, empty boxes, zero bounds and modular\n";
}
