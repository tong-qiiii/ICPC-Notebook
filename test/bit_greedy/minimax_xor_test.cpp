#include "../../code/bit_greedy/minimax_xor.hpp"
#include "support.hpp"

ll evaluate(const vector<ll> &a, ll mask)
{
    ll val = 0;
    for (int i = 1; i < (int)a.size(); i++)
    {
        val = max(val, a[i] ^ mask);
    }
    return val;
}

int main()
{
    for (int t = 0; t < 1400; t++)
    {
        vector<ll> a(rnd(0, 9) + 1);
        for (int i = 1; i < (int)a.size(); i++)
        {
            a[i] = rnd(0, 31);
        }
        ll best = LLONG_MAX;
        for (ll mask = 0; mask < 32; mask++)
        {
            best = min(best, evaluate(a, mask));
        }
        auto [val, mask] = minimax_xor(a);
        assert(val == best && evaluate(a, mask) == best);
    }
    auto [val, mask] = minimax_xor({0, 0, 1LL << 61});
    assert(val == (1LL << 61) && max(mask, mask ^ (1LL << 61)) == val);
    assert(minimax_xor({0, (1LL << 62) - 1}) == make_pair(0LL, (1LL << 62) - 1));
    cout << "PASS minimax_xor: 1400 exhaustive-mask cases, witness, empty and high bits\n";
}
