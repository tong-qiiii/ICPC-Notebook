#include "code/data_structure/fenwick_range.hpp"

int main()
{
    mt19937 rg(740);
    BITRange b;
    assert(b.sum(0) == 0);
    for (int z = 0; z < 500; z++)
    {
        int n = 1 + rg() % 100;
        b.init(n);
        vector<ll> a(n + 1);
        for (int t = 0; t < 300; t++)
        {
            int l = 1 + rg() % n, r = 1 + rg() % n;
            if (l > r) swap(l, r);
            ll v = int(rg() % 2001) - 1000;
            if (t % 3)
            {
                b.add(l, r, v);
                for (int i = l; i <= r; i++) a[i] += v;
            }
            else assert(b.query(l, r) == accumulate(a.begin() + l, a.begin() + r + 1, 0LL));
            assert(b.query(r, r) == a[r]);
        }
        assert(b.sum(n) == accumulate(a.begin(), a.end(), 0LL));
    }
    cout << "PASS fenwick_range: brute random operations, endpoints, reinit\n";
}
