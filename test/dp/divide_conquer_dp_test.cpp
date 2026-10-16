#include "code/dp/divide_conquer_dp.hpp"
int main()
{
    mt19937 gen(20261016);
    auto d = divideConquerDP(4, 2,
                             [](int l, int r) -> ll
                             { return 1LL * (r - l + 1) * (r - l + 1); });
    assert(d[4] == 8);
    {
        for (int n = 0; n < 60; n++)
        {
            vector<ll> pre(n + 1);
            for (int i = 1; i <= n; i++)
            {
                pre[i] = pre[i - 1] + gen() % 10;
            }
            auto cost = [&](int l, int r)
            {
                ll v = pre[r] - pre[l - 1];
                return v * v;
            };
            vector<ll> ref(n + 1, LLONG_MAX / 4);
            ref[0] = 0;
            for (int K = 0; K <= n + 1; K++)
            {
                assert(divideConquerDP(n, K, cost) == ref);
                vector<ll> nxt(n + 1, LLONG_MAX / 4);
                for (int i = 1; i <= n; i++)
                {
                    for (int j = 0; j < i; j++)
                    {
                        if (ref[j] < LLONG_MAX / 8)
                        {
                            nxt[i] = min(nxt[i], ref[j] + cost(j + 1, i));
                        }
                    }
                }
                ref = nxt;
            }
        }
    }
    cout << "PASS divide_conquer_dp\n";
}
