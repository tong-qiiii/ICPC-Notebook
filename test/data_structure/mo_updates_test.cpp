#include "code/data_structure/mo_updates.hpp"
int main()
{
    mt19937 gen(20261016);
    vector<int> a{0, 1, 2};
    vector<Change> c{{}, {1, 1, 5}};
    vector<Query> q{{1, 2, 0, 0}, {1, 2, 1, 1}};
    ll now = 0;
    auto ans = mo_updates(
        a, c, q, now, [&](int v) { now += v; }, [&](int v) { now -= v; });
    assert((ans == vector<ll>{3, 7}));
    {
        for (int t = 0; t < 120; t++)
        {
            int n = gen() % 25 + 1;
            vector<int> a(n + 1), cur;
            for (int i = 1; i <= n; i++)
            {
                a[i] = gen() % 20;
            }
            cur = a;
            vector<Change> c(1);
            vector<Query> q;
            vector<ll> ref;
            for (int i = 0; i < 80; i++)
            {
                if (gen() % 2)
                {
                    int p = gen() % n + 1, v = gen() % 20;
                    c.push_back({p, cur[p], v});
                    cur[p] = v;
                }
                else
                {
                    int l = gen() % n + 1, r = gen() % n + 1;
                    if (l > r)
                    {
                        swap(l, r);
                    }
                    q.push_back({l, r, (int)c.size() - 1, (int)q.size()});
                    ref.push_back(
                        accumulate(cur.begin() + l, cur.begin() + r + 1, 0LL));
                }
            }
            ll now = 0;
            assert(mo_updates(
                       a, c, q, now, [&](int v) { now += v; },
                       [&](int v) { now -= v; }) == ref);
        }
    }
    cout << "PASS mo_updates\n";
}
