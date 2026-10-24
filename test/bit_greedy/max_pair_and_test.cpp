#include "../../code/bit_greedy/max_pair_and.hpp"
#include "support.hpp"

ll brute(const vector<ll> &a, vector<ll> b)
{
    sort(b.begin() + 1, b.end());
    ll ans = 0;
    do
    {
        ll val = LLONG_MAX;
        for (int i = 1; i < (int)a.size(); i++)
        {
            val &= a[i] ^ b[i];
        }
        ans = max(ans, val);
    } while (next_permutation(b.begin() + 1, b.end()));
    return ans;
}

int main()
{
    for (int t = 0; t < 650; t++)
    {
        int n = rnd(1, 6);
        vector<ll> a(n + 1), b(n + 1);
        for (int i = 1; i <= n; i++)
        {
            a[i] = rnd(0, 15);
            b[i] = rnd(0, 15);
        }
        assert(max_pair_and(a, b) == brute(a, b));
    }
    assert(max_pair_and({0, 0, 1}, {0, (1LL << 61), (1LL << 61) + 1})
           == (1LL << 61) + 1);
    cout << "PASS max_pair_and: 650 full-permutation cases, multiple calls, high bits\n";
}
