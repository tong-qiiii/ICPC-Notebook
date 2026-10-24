#include "../../code/bit_greedy/max_and_increment.hpp"
#include "support.hpp"

ll brute(const vector<ll> &a, int k)
{
    ll best = 0;
    auto dfs = [&](auto &&self, int i, int left, ll val) -> void
    {
        if (i == (int)a.size())
        {
            best = max(best, val);
            return;
        }
        for (int d = 0; d <= left; d++)
        {
            self(self, i + 1, left - d, val & (a[i] + d));
        }
    };
    dfs(dfs, 1, k, LLONG_MAX);
    return best;
}

int main()
{
    for (int t = 0; t < 1200; t++)
    {
        vector<ll> a(rnd(1, 4) + 1);
        for (int i = 1; i < (int)a.size(); i++)
        {
            a[i] = rnd(0, 15);
        }
        auto saved = a;
        int k = rnd(0, 8);
        assert(max_and_increment(a, k) == brute(a, k));
        assert(a == saved);
        assert(max_and_increment(a, 0) == brute(a, 0));
    }
    ll hi = (1LL << 61) - 1;
    assert(max_and_increment({0, hi}, hi) == 2 * hi);
    assert(max_and_increment({0, 0, 0}, 1) == 0);
    cout << "PASS max_and_increment: 1200 allocation brute cases, bounds, repeated calls\n";
}
