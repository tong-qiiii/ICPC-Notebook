#include "../../code/bit_greedy/unique_bits.hpp"
#include "support.hpp"

ll fold(const vector<ll> &a)
{
    ll val = a[1];
    for (int i = 2; i < (int)a.size(); i++)
    {
        val = (val | a[i]) - a[i];
    }
    return val;
}

int main()
{
    for (int t = 0; t < 500; t++)
    {
        int n = rnd(1, 6);
        vector<ll> a(n + 1);
        for (int i = 1; i <= n; i++)
        {
            a[i] = rnd(0, 31);
        }
        auto b = a;
        sort(b.begin() + 1, b.end());
        ll best = 0;
        do
        {
            best = max(best, fold(b));
        } while (next_permutation(b.begin() + 1, b.end()));
        int i = unique_bits(a);
        assert(i >= 1 && i <= n);
        swap(a[1], a[i]);
        assert(fold(a) == best);
    }
    assert(unique_bits({0, 1, 1LL << 61}) == 2);
    assert(unique_bits({0, 7, 7}) == 1);
    cout << "PASS unique_bits: 500 full-permutation fold cases, duplicates, high bits\n";
}
