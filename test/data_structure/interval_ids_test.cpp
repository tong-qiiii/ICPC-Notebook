#include "code/data_structure/interval_ids.hpp"
int main()
{
    mt19937 gen(20261016);
    IntSet s(vector<IntSet::Seg>{{}, {1, 3}, {4, 6}, {2, 5}});
    assert(s.insert(1) && s.insert(2) && !s.insert(3));
    assert((s.ids(3, 4) == vector<int>{1, 2}));
    {
        for (int run = 0; run < 30; run++)
        {
            vector<IntSet::Seg> a(101);
            for (int i = 1; i <= 100; i++)
            {
                int l = gen() % 61 - 30, r = gen() % 61 - 30;
                if (l > r)
                {
                    swap(l, r);
                }
                a[i] = {l, r};
            }
            IntSet s(a);
            set<int> active;
            for (int j = 0; j < 500; j++)
            {
                int id = gen() % 100 + 1;
                bool ok = true;
                for (int k : active)
                {
                    if (a[k].l <= a[id].r && a[id].l <= a[k].r)
                    {
                        ok = false;
                    }
                }
                if (gen() % 2)
                {
                    assert(s.insert(id) == ok);
                    if (ok)
                    {
                        active.insert(id);
                    }
                }
                else
                {
                    assert(s.erase(id) == bool(active.count(id)));
                    active.erase(id);
                }
                int l = gen() % 61 - 30, r = gen() % 61 - 30;
                if (l > r)
                {
                    swap(l, r);
                }
                vector<int> ref;
                for (int k : active)
                {
                    if (a[k].l <= r && l <= a[k].r)
                    {
                        ref.push_back(k);
                    }
                }
                auto got = s.ids(l, r);
                sort(got.begin(), got.end());
                sort(ref.begin(), ref.end());
                assert(got == ref);
                if (gen() % 5 == 0)
                {
                    auto z = s.erase_cross(l, r);
                    sort(z.begin(), z.end());
                    assert(z == ref);
                    for (int k : z)
                    {
                        active.erase(k);
                    }
                }
            }
        }
    }
    cout << "PASS interval_ids\n";
}
