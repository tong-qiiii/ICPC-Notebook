#include "../../code/bit_greedy/xor_inversions.hpp"
#include "support.hpp"

ll inversions(const vector<ll> &a, ll mask)
{
    ll ans = 0;
    for (int i = 1; i < (int)a.size(); i++)
    {
        for (int j = i + 1; j < (int)a.size(); j++)
        {
            ans += (a[i] ^ mask) > (a[j] ^ mask);
        }
    }
    return ans;
}

int main()
{
    for (int t = 0; t < 1600; t++)
    {
        vector<ll> a(rnd(0, 10) + 1);
        for (int i = 1; i < (int)a.size(); i++)
        {
            a[i] = rnd(0, 15);
        }
        pair<ll, ll> best = {LLONG_MAX, 0};
        for (ll mask = 0; mask < 16; mask++)
        {
            best = min(best, make_pair(inversions(a, mask), mask));
        }
        assert(xor_inversions(a) == best);
    }
    assert(xor_inversions({0, 1LL << 61, 0}) == make_pair(0LL, 1LL << 61));
    assert(xor_inversions({0}) == make_pair(0LL, 0LL));
    cout << "PASS xor_inversions: 1600 exhaustive-mask cases, duplicates, empty, high bits\n";
}
