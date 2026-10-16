#include "code/data_structure/mo_max_span.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(mo_max_span(vector<int>{0, 1, 2, 1}, vector<Query>{{1, 3, 0}}, 3)[0] == 2);
    {
        for (int n = 1; n <= 70; n++)
        {
            vector<int> a(n + 1);
            for (int i = 1; i <= n; i++)
            {
                a[i] = gen() % 8;
            }
            vector<Query> q;
            vector<ll> ref;
            for (int i = 0; i < 90; i++)
            {
                int l = gen() % n + 1, r = gen() % n + 1;
                if (l > r)
                {
                    swap(l, r);
                }
                q.push_back({l, r, i});
                int z = 0;
                for (int x = l; x <= r; x++)
                {
                    for (int y = x; y <= r; y++)
                    {
                        if (a[x] == a[y])
                        {
                            z = max(z, y - x);
                        }
                    }
                }
                ref.push_back(z);
            }
            assert(mo_max_span(a, q, 8) == ref);
        }
    }
    cout << "PASS mo_max_span\n";
}
