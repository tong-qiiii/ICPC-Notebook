#include "code/dp/wqs.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(wqs(4, 2, 100,
               [](int l, int r) -> ll
               { return 1LL * (r - l + 1) * (r - l + 1); }) == 8);
    {
        for (int n = 1; n < 40; n++)
        {
            vector<ll> p(n + 1);
            for (int i = 1; i <= n; i++)
            {
                p[i] = p[i - 1] + gen() % 5;
            }
            auto cost = [&](int l, int r)
            {
                ll s = p[r] - p[l - 1];
                return s * s;
            };
            vector<ll> pre(n + 1, LLONG_MAX / 4);
            pre[0] = 0;
            for (int K = 1; K <= n; K++)
            {
                vector<ll> cur(n + 1, LLONG_MAX / 4);
                for (int i = 1; i <= n; i++)
                {
                    for (int j = 0; j < i; j++)
                    {
                        if (pre[j] < LLONG_MAX / 8)
                        {
                            cur[i] = min(cur[i], pre[j] + cost(j + 1, i));
                        }
                    }
                }
                pre = cur;
                assert(wqs(n, K, 1000000, cost) == pre[n]);
            }
        }
    }
    cout << "PASS wqs\n";
}
