#include "code/data_structure/zkw_segment_tree_point.hpp"

int main()
{
    mt19937_64 gen(20261020);
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
            int p = 1 + gen() % n;
            ll v = (int)(gen() % 2001) - 1000;
            seg.modify(p, v);
            a[p] = v;
            int l = 1 + gen() % n, r = 1 + gen() % n;
            if (l > r) swap(l, r);
            assert(seg.query(l, r) == accumulate(a.begin() + l, a.begin() + r + 1, 0LL));
            assert(seg.query(1, n) == accumulate(a.begin() + 1, a.end(), 0LL));
            assert(seg.query(p, p) == v);
            ops++;
        }
    }
    seg.build({0, 1000000000000LL, -3000000000000LL});
    assert(seg.query(1, 2) == -2000000000000LL);
    seg.build({0});
    assert(seg.query(1, 0) == 0);
    cout << "PASS zkw point: " << ops << " brute-force updates, padding and rebuild\n";
}
