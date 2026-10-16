#include "code/data_structure/mo_rollback_add.hpp"
int main()
{
    mt19937 gen(20261016);
    ll now = 0;
    auto add = [&](int p) { now += p; };
    auto undo = [&](int p) { now -= p; };
    auto ans = mo_rollback_add(3, vector<Query>{{1, 3, 0}}, now, add, add, undo,
                               [&]() { now = 0; });
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
            auto add = [&](int p) { now += p; };
            auto undo = [&](int p) { now -= p; };
            assert(mo_rollback_add(n, q, now, add, add, undo, [&]() { now = 0; }) ==
                   ref);
        }
    }
    cout << "PASS mo_rollback_add\n";
}
