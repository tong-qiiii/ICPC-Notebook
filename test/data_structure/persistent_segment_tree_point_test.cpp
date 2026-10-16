#include "code/data_structure/persistent_segment_tree_point.hpp"
int main()
{
    mt19937 gen(20261016);
    PST p;
    int r = p.modify(0, 1, 3, 2, Info(5));
    assert(p.query(r, 1, 3, 1, 3).sum == 5 && p.query(0, 1, 3, 1, 3).sum == 0);
    {
        for (int run = 0; run < 20; run++)
        {
            int n = 30;
            PST pst;
            vector<int> rt{0};
            vector<vector<ll>> all(1, vector<ll>(n + 1));
            for (int t = 0; t < 200; t++)
            {
                int v = gen() % rt.size(), l = gen() % n + 1, r = gen() % n + 1;
                if (l > r)
                {
                    swap(l, r);
                }
                ll z = int(gen() % 31) - 15;
                auto b = all[v];
                int nr = pst.modify(rt[v], 1, n, l, Info(z));
                b[l] = z;
                rt.push_back(nr);
                all.push_back(b);
                for (int k = 0; k < 10; k++)
                {
                    int ver = gen() % rt.size(), ql = gen() % n + 1, qr = gen() % n + 1;
                    if (ql > qr)
                    {
                        swap(ql, qr);
                    }
                    auto cnt = pst.tr.size();
                    assert(pst.query(rt[ver], 1, n, ql, qr).sum ==
                           accumulate(all[ver].begin() + ql, all[ver].begin() + qr + 1,
                                      0LL));
                    assert(cnt == pst.tr.size());
                }
            }
        }
    }
    cout << "PASS persistent_segment_tree_point\n";
}
