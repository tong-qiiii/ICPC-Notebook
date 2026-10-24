#include "../../code/bit_greedy/min_xor_pair.hpp"
#include "support.hpp"

int main()
{
    for (int t = 0; t < 1800; t++)
    {
        int n = rnd(2, 15);
        vector<ll> a(n + 1);
        for (int i = 1; i <= n; i++)
        {
            a[i] = rnd(0, 63);
        }
        ll best = LLONG_MAX;
        for (int i = 1; i <= n; i++)
        {
            for (int j = i + 1; j <= n; j++)
            {
                best = min(best, a[i] ^ a[j]);
            }
        }
        auto [i, j] = min_xor_pair(a);
        assert(i >= 1 && i <= n && j >= 1 && j <= n && i != j);
        assert((a[i] ^ a[j]) == best);
    }
    vector<ll> a = {0, 0, LLONG_MAX};
    auto [i, j] = min_xor_pair(a);
    assert(i != j && (a[i] ^ a[j]) == LLONG_MAX);
    cout << "PASS min_xor_pair: 1800 all-pair cases, duplicates, signed maximum\n";
}
