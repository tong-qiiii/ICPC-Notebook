#include "code/data_structure/zkw_segment_tree_lazy.hpp"

int main()
{
    mt19937_64 gen(20261021);
    Seg seg;
    assert(seg.query(1, 0) == 0);
    int ops = 0;
    for (int n : {1, 2, 3, 4, 5, 7, 8, 9, 31, 32, 33, 127, 128, 129, 1025})
    {
        vector<ll> a(n + 1);
        for (int i = 1; i <= n; i++) a[i] = (int)(gen() % 1001) - 500;
        seg.build(a);
        for (int q = 0; q < 3000; q++)
        {
            int l = 1 + gen() % n, r = 1 + gen() % n;
            if (l > r) swap(l, r);
            if (q % 7 == 0) l = 1, r = n;
            if (q % 11 == 0) r = l;
            ll v = (int)(gen() % 2001) - 1000;
            seg.add(l, r, v);
            for (int i = l; i <= r; i++) a[i] += v;
            assert(seg.query(l, r) == accumulate(a.begin() + l, a.begin() + r + 1, 0LL));
            l = 1 + gen() % n; r = 1 + gen() % n;
            if (l > r) swap(l, r);
            assert(seg.query(l, r) == accumulate(a.begin() + l, a.begin() + r + 1, 0LL));
            assert(seg.query(1, n) == accumulate(a.begin() + 1, a.end(), 0LL));
            ops++;
        }
        for (int p = 1; p <= n; p++) assert(seg.query(p, p) == a[p]);
    }
    seg.build({0, 1, 2, 3, 4, 5});
    seg.add(1, 5, 1000000000000LL);
    seg.add(2, 4, -2000000000000LL);
    assert(seg.query(1, 5) == -1000000000000LL + 15);
    assert(seg.query(2, 4) == -3000000000000LL + 9);
    seg.build({});
    seg.add(1, 0, 7);
    assert(seg.query(1, 0) == 0);
    seg.build({0, 6});
    assert(seg.query(1, 1) == 6);
    cout << "PASS zkw range: " << ops << " brute-force updates, ancestor tags, sentinels and rebuild\n";
}
