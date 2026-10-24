#include "../../code/bit_greedy/range_and_or.hpp"
#include "support.hpp"

int main()
{
    for (ll l = 0; l < 64; l++)
    {
        ll both = LLONG_MAX, either = 0;
        for (ll r = l; r < 64; r++)
        {
            both &= r;
            either |= r;
            assert(range_and_or(l, r) == make_pair(both, either));
        }
    }
    for (ll n = 0; n < 64; n++)
    {
        for (ll target = 0; target < 64; target++)
        {
            ll both = LLONG_MAX, want = -1;
            for (ll r = n; r <= 128; r++)
            {
                both &= r;
                if (both == target)
                {
                    want = r;
                    break;
                }
            }
            assert(min_and_end(n, target) == want);
        }
    }
    ll n = (1LL << 60) + 3;
    assert(min_and_end(n, 1LL << 60) == (1LL << 60) + 4);
    assert(min_and_end(n, 0) == (1LL << 61));
    assert(range_and_or(0, (1LL << 62) - 1) == make_pair(0LL, (1LL << 62) - 1));
    cout << "PASS range_and_or: 2080 intervals, 4096 inverse-endpoint cases, high bits\n";
}
