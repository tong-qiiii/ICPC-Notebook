#include "../../code/bit_greedy/power_terms.hpp"
#include "support.hpp"

int main()
{
    bool dp[61][61]{};
    dp[0][0] = true;
    for (int k = 1; k <= 60; k++)
    {
        for (int s = 0; s <= 60; s++)
        {
            for (int v = 1; v <= s; v <<= 1)
            {
                dp[k][s] |= dp[k - 1][s - v];
            }
        }
    }
    for (int s = 0; s <= 60; s++)
    {
        for (int k = 0; k <= 60; k++)
        {
            assert(power_terms(s, k) == dp[k][s]);
        }
    }
    assert(!power_terms(-1, 0) && !power_terms(1, -1));
    assert(power_terms(1LL << 61, 1) && power_terms(1LL << 61, 1LL << 61));
    assert(power_terms(LLONG_MAX, 63) && !power_terms(LLONG_MAX, 62));
    cout << "PASS power_terms: 3721 exact-term DP states, zero, invalid and large inputs\n";
}
