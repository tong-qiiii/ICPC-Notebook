#include "code/data_structure/segment_tree_merge.hpp"
int main()
{
    mt19937 gen(20261016);
    SegMerge s(2, 5);
    s.add(1, 2, 1);
    s.add(2, 3, 2);
    s.root[1] = s.merge(s.root[1], s.root[2]);
    s.root[2] = 0;
    assert(s.tr[s.root[1]].pos == 3);
    {
        for (int t = 0; t < 100; t++)
        {
            SegMerge s(20, 30);
            vector<int> a(31);
            for (int i = 1; i <= 20; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    int v = gen() % 30 + 1;
                    s.add(i, v, 1);
                    a[v]++;
                }
            }
            for (int i = 2; i <= 20; i++)
            {
                s.root[1] = s.merge(s.root[1], s.root[i]);
                s.root[i] = 0;
            }
            int best = max_element(a.begin() + 1, a.end()) - a.begin();
            assert(s.tr[s.root[1]].pos == best && s.tr[s.root[1]].mx == a[best]);
        }
    }
    {
        SegMerge seg(1, 20);
        vector<int> freq(21);
        for (int i = 0; i < 3000; ++i)
        {
            int p = gen() % 20 + 1;
            int delta = freq[p] && gen() % 2 ? -1 : 1;
            freq[p] += delta;
            seg.add(1, p, delta);
            int best = max_element(freq.begin() + 1, freq.end()) - freq.begin();
            int count = freq[best];
            if (!count)
            {
                best = 0;
            }
            assert(seg.tr[seg.root[1]].mx == count);
            assert(seg.tr[seg.root[1]].pos == best);
        }
        for (int p = 1; p <= 20; ++p)
        {
            seg.add(1, p, -freq[p]);
        }
        assert(seg.tr[seg.root[1]].mx == 0 && seg.tr[seg.root[1]].pos == 0);
        SegMerge zero(2, 1);
        zero.add(1, 1, 0);
        zero.add(2, 1, 0);
        int root = zero.merge(zero.root[1], zero.root[2]);
        assert(zero.tr[root].mx == 0 && zero.tr[root].pos == 0);
    }
    cout << "PASS segment_tree_merge\n";
}
