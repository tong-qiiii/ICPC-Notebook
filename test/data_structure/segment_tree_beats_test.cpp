#include "code/data_structure/segment_tree_beats.hpp"

void check(Seg &seg, const vector<ll> &a, int l, int r)
{
    Info res = seg.query(l, r);
    vector<ll> b(a.begin() + l, a.begin() + r + 1);
    sort(b.begin(), b.end());
    assert(res.len == r - l + 1);
    assert(res.sum == accumulate(b.begin(), b.end(), 0LL));
    assert(res.mn == b.front() && res.mx == b.back());
    assert(res.nmn == count(b.begin(), b.end(), b.front()));
    assert(res.nmx == count(b.begin(), b.end(), b.back()));
    b.erase(unique(b.begin(), b.end()), b.end());
    assert(res.mn2 == (b.size() == 1 ? Info::INF : b[1]));
    assert(res.mx2 == (b.size() == 1 ? -Info::INF : b[b.size() - 2]));
}

int main()
{
    mt19937_64 gen(20261019);
    Seg seg;
    assert(seg.query(1, 0).len == 0);
    int ops = 0;
    for (int t = 0; t < 180; t++)
    {
        int n = t < 10 ? t + 1 : 1 + gen() % 150;
        vector<ll> a(n + 1);
        for (int i = 1; i <= n; i++) a[i] = (int)(gen() % 101) - 50;
        if (t % 4 == 0) fill(a.begin() + 1, a.end(), 7);
        if (t % 4 == 1) iota(a.begin() + 1, a.end(), -n);
        seg.build(a);
        for (int q = 0; q < 600; q++)
        {
            int l = 1 + gen() % n, r = 1 + gen() % n;
            if (l > r) swap(l, r);
            if (q % 7 == 0) l = 1, r = n;
            int op = gen() % 3;
            ll v = (int)(gen() % 101) - 50;
            if (q % 5 == 0 && op != 0)
            {
                vector<ll> b(a.begin() + l, a.begin() + r + 1);
                sort(b.begin(), b.end());
                v = b[b.size() / 2];
            }
            if (op == 0) seg.add(l, r, v);
            if (op == 1) seg.chmin(l, r, v);
            if (op == 2) seg.chmax(l, r, v);
            for (int i = l; i <= r; i++)
            {
                if (op == 0) a[i] += v;
                if (op == 1) a[i] = min(a[i], v);
                if (op == 2) a[i] = max(a[i], v);
            }
            check(seg, a, l, r);
            check(seg, a, 1, n);
            int p = 1 + gen() % n;
            check(seg, a, p, p);
            ops++;
        }
    }
    // 相等的严格次值必须向下递归；连续取上下界可变成整段相同。
    vector<ll> a{0, 1, 3, 3, 7, 7};
    seg.build(a);
    seg.chmin(1, 5, 3);
    seg.chmax(1, 5, 3);
    seg.add(2, 4, -5);
    a = {0, 3, -2, -2, -2, 3};
    for (int l = 1; l <= 5; l++)
    {
        for (int r = l; r <= 5; r++) check(seg, a, l, r);
    }
    a = {0, Info::INF - 100, -Info::INF + 100};
    seg.build(a);
    seg.chmin(1, 2, -Info::INF + 101);
    a[1] = -Info::INF + 101;
    seg.add(1, 2, 5);
    a[1] += 5; a[2] += 5;
    check(seg, a, 1, 2);
    seg.build({});
    seg.add(1, 0, 8);
    assert(seg.query(1, 0).len == 0);
    seg.build({0, 4});
    assert(seg.query(1, 1).sum == 4);
    cout << "PASS Beats: " << ops << " mixed brute-force updates, extrema metadata, rebuild and limits\n";
}
