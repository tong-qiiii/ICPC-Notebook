#include "code/data_structure/mo_second_offline.hpp"
int main()
{
    mt19937 gen(20261016);
    int n = 3;
    vector<ll> pre{0, 0, 1, 3}, suf{0, 3, 1, 0, 0};
    int p = 0, s = 4;
    auto ans = mo_second_offline(
        n, vector<Query>{{1, 3, 0}}, pre, suf, [&]() { p = 0; }, [&](int x) { p = x; },
        [&](int i) -> ll { return min(p, i - 1); }, [&]() { s = n + 1; },
        [&](int x) { s = x; }, [&](int i) -> ll { return n - max(s, i + 1) + 1; });
    assert(ans[0] == 3);
    {
        for (int n = 1; n <= 40; n++)
        {
            vector<vector<ll>> w(n + 1, vector<ll>(n + 1));
            for (int i = 1; i <= n; i++)
            {
                for (int j = i + 1; j <= n; j++)
                {
                    w[i][j] = gen() % 9;
                }
            }
            vector<ll> pre(n + 1), suf(n + 2);
            for (int i = 1; i <= n; i++)
            {
                pre[i] = pre[i - 1];
                for (int j = 1; j < i; j++)
                {
                    pre[i] += w[j][i];
                }
            }
            for (int i = n; i; i--)
            {
                suf[i] = suf[i + 1];
                for (int j = i + 1; j <= n; j++)
                {
                    suf[i] += w[i][j];
                }
            }
            vector<Query> q;
            vector<ll> ref;
            for (int k = 0; k < 150; k++)
            {
                int l = gen() % n + 1, r = gen() % n + 1;
                if (l > r)
                {
                    swap(l, r);
                }
                q.push_back({l, r, k});
                ll z = 0;
                for (int i = l; i <= r; i++)
                {
                    for (int j = i + 1; j <= r; j++)
                    {
                        z += w[i][j];
                    }
                }
                ref.push_back(z);
            }
            int p = 0, s = n + 1;
            auto ans = mo_second_offline(
                n, q, pre, suf, [&]() { p = 0; }, [&](int x) { p = x; },
                [&](int i)
                {
                    ll z = 0;
                    for (int j = 1; j <= p; j++)
                    {
                        z += w[j][i];
                    }
                    return z;
                },
                [&]() { s = n + 1; }, [&](int x) { s = x; },
                [&](int i)
                {
                    ll z = 0;
                    for (int j = s; j <= n; j++)
                    {
                        z += w[i][j];
                    }
                    return z;
                });
            assert(ans == ref);
        }
    }
    cout << "PASS mo_second_offline\n";
}
