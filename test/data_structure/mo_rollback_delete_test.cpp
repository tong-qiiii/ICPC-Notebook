#include "code/data_structure/mo_rollback_delete.hpp"
int main()
{
    mt19937 gen(20261016);
    ll now = 0;
    auto ans = mo_rollback_delete(
        3, vector<Query>{{1, 3, 0}}, now, [&]() { now = 6; }, [&](int p) { now -= p; },
        [&](int p) { now += p; });
    assert(ans[0] == 6);
    {
        for (int n = 1; n <= 80; n++)
        {
            vector<Query> q;
            vector<ll> ref;
            for (int i = 0; i < 100; i++)
            {
                int l = gen() % n + 1, r = gen() % n + 1;
                if (l > r)
                {
                    swap(l, r);
                }
                q.push_back({l, r, i});
                ref.push_back(1LL * (l + r) * (r - l + 1) / 2);
            }
            ll now = 0;
            assert(mo_rollback_delete(
                       n, q, now, [&]() { now = 1LL * n * (n + 1) / 2; },
                       [&](int p) { now -= p; }, [&](int p) { now += p; }) == ref);
        }
    }
    cout << "PASS mo_rollback_delete\n";
}
