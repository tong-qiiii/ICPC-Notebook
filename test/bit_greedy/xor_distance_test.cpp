#include "../../code/bit_greedy/xor_distance.hpp"
#include "support.hpp"

void check_case(ll a, ll b, ll r)
{
    ll best = LLONG_MAX;
    for (ll mask = 0; mask <= r; mask++)
    {
        best = min(best, abs((a ^ mask) - (b ^ mask)));
    }
    auto [gap, mask] = xor_distance(a, b, r);
    assert(mask >= 0 && mask <= r);
    assert(gap == best && gap == abs((a ^ mask) - (b ^ mask)));
}

int main()
{
    for (ll a = 0; a < 32; a++)
    {
        for (ll b = 0; b < 32; b++)
        {
            for (ll r = 0; r < 32; r++)
            {
                check_case(a, b, r);
            }
        }
    }
    for (int t = 0; t < 400; t++)
    {
        check_case((1LL << 61) + rnd(0, 127), rnd(0, 127), rnd(0, 127));
    }
    assert(xor_distance(0, (1LL << 62) - 1, (1LL << 62) - 1).first == 1);
    cout << "PASS xor_distance: 32768 exhaustive small triples, 400 high-bit cases\n";
}
