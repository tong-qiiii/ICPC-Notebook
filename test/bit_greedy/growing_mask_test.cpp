#include "../../code/bit_greedy/growing_mask.hpp"
#include "support.hpp"

vector<ll> brute(const vector<ll> &a)
{
    vector<ll> cur(a.size()), ans;
    auto dfs = [&](auto &&self, int i, ll prev) -> bool
    {
        if (i == (int)a.size())
        {
            ans = cur;
            return true;
        }
        for (ll mask = 0; mask < 8; mask++)
        {
            ll val = a[i] ^ mask;
            if ((prev & val) != prev)
            {
                continue;
            }
            cur[i] = mask;
            if (self(self, i + 1, val))
            {
                return true;
            }
        }
        return false;
    };
    assert(dfs(dfs, 1, 0));
    return ans;
}

int main()
{
    for (int t = 0; t < 1300; t++)
    {
        vector<ll> a(rnd(0, 5) + 1);
        for (int i = 1; i < (int)a.size(); i++)
        {
            a[i] = rnd(0, 7);
        }
        assert(growing_mask(a) == brute(a));
    }
    assert(growing_mask({0, 1LL << 61, 0}) == vector<ll>({0, 0, 1LL << 61}));
    cout << "PASS growing_mask: 1300 lexicographic enumeration cases, empty and high bits\n";
}
