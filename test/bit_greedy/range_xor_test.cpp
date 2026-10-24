#include "../../code/bit_greedy/range_xor.hpp"
#include "support.hpp"

int main()
{
    for (ll l = 0; l < 64; l++)
    {
        for (ll r = l; r < 64; r++)
        {
            ll best = 0;
            for (ll u = l; u <= r; u++)
            {
                for (ll v = u; v <= r; v++)
                {
                    best = max(best, u ^ v);
                }
            }
            assert(range_max_xor(l, r) == best);
        }
    }
    assert(range_max_xor(0, (1LL << 62) - 1) == (1LL << 62) - 1);
    assert(range_max_xor((1LL << 61) - 1, 1LL << 61) == (1LL << 62) - 1);
    cout << "PASS range_max_xor: all 2080 small intervals, 62-bit boundaries\n";
}
