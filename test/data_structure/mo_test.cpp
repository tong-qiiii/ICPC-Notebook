#include "code/data_structure/mo.hpp"
int main()
{
    mt19937 gen(20261016);
    ll now = 0;
    vector<Query> q{{1, 2, 0}, {2, 3, 1}};
    vector<int> a{0, 1, 2, 1};
    auto add = [&](int p) { now += a[p]; };
    auto del = [&](int p) { now -= a[p]; };
    auto ans = mo(3, q, now, add, del);
    assert((ans == vector<ll>{3, 3}));
    {
        for (int t = 0; t < 100; t++)
        {
            int n = gen() % 35 + 1;
            vector<int> a(n + 1);
            for (int i = 1; i <= n; i++)
            {
                a[i] = gen() % 10;
            }
            vector<Query> q;
            vector<ll> ref;
            for (int i = 0; i < 80; i++)
            {
                int l = gen() % n + 1, r = gen() % n + 1;
                if (l > r)
                {
                    swap(l, r);
                }
                q.push_back({l, r, i});
                ref.push_back(accumulate(a.begin() + l, a.begin() + r + 1, 0LL));
            }
            ll now = 0;
            auto v = mo(
                n, q, now, [&](int p) { now += a[p]; }, [&](int p) { now -= a[p]; });
            assert(v == ref);
        }
    }
    cout << "PASS mo\n";
}
