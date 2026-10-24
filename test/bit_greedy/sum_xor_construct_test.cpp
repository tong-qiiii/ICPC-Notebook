#include "../../code/bit_greedy/sum_xor_construct.hpp"
#include "support.hpp"

int main()
{
    const int INF = 1000;
    int dp[31][64];
    for (auto &row : dp)
    {
        fill(begin(row), end(row), INF);
    }
    dp[0][0] = 0;
    for (int s = 0; s <= 30; s++)
    {
        for (int u = 0; u < 64; u++)
        {
            if (dp[s][u] == INF)
            {
                continue;
            }
            for (int v = 1; v + s <= 30; v++)
            {
                dp[s + v][u ^ v] = min(dp[s + v][u ^ v], dp[s][u] + 1);
            }
        }
    }
    for (int s = 0; s <= 30; s++)
    {
        for (int u = 0; u < 64; u++)
        {
            auto ans = sum_xor_construct(u, s);
            if (dp[s][u] == INF)
            {
                assert(ans == vector<ll>({-1}));
                continue;
            }
            assert((int)ans.size() == dp[s][u]);
            ll sum = 0, val = 0;
            for (ll v : ans)
            {
                assert(v > 0);
                sum += v;
                val ^= v;
            }
            assert(sum == s && val == u);
        }
    }
    assert(sum_xor_construct(LLONG_MAX, LLONG_MAX) == vector<ll>({LLONG_MAX}));
    auto ans = sum_xor_construct(1, LLONG_MAX);
    __int128 total = 0;
    ll val = 0;
    for (ll v : ans)
    {
        total += v;
        val ^= v;
    }
    assert(total == LLONG_MAX && val == 1);
    cout << "PASS sum_xor_construct: 1984 sum/XOR DP states, minimal count, signed limit\n";
}
