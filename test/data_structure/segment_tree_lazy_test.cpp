#include "code/data_structure/segment_tree_lazy.hpp"
int main()
{
    mt19937 gen(20261016);
    Seg seg(vector<ll>{0, 1, 2, 3});
    assert(seg.query(1, 3).sum == 6);
    {
        Seg seg;
        for (int n = 1; n <= 45; n++)
        {
            vector<ll> a(n + 1);
            seg.build(a);
            for (int j = 0; j < 300; j++)
            {
                int l = gen() % n + 1, r = gen() % n + 1;
                if (l > r)
                {
                    swap(l, r);
                }
                ll v = int(gen() % 31) - 15;
                if (gen() % 2)
                {
                    seg.modify(l, r, Tag(v));
                    for (int i = l; i <= r; i++)
                    {
                        a[i] += v;
                    }
                }
                else
                {
                    assert(seg.query(l, r).sum ==
                           accumulate(a.begin() + l, a.begin() + r + 1, 0LL));
                }
            }
        }
    }
    cout << "PASS segment_tree_lazy\n";
}
