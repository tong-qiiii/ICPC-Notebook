#include "code/data_structure/persistent_segment_tree_count.hpp"
int main()
{
    mt19937 gen(20261016);
    PST p;
    int a = p.modify(0, 1, 5, 2, 1), b = p.modify(a, 1, 5, 4, 1);
    assert(p.kth(0, b, 1, 5, 2) == 4 && p.query(a, b, 1, 5, 1, 5) == 1);
    {
        for (int run = 0; run < 80; run++)
        {
            PST p;
            int n = 40;
            vector<int> a(n + 1), rt(n + 1);
            for (int i = 1; i <= n; i++)
            {
                a[i] = gen() % 20 + 1;
                rt[i] = p.modify(rt[i - 1], 1, 20, a[i], 1);
            }
            for (int j = 0; j < 200; j++)
            {
                int l = gen() % n + 1, r = gen() % n + 1;
                if (l > r)
                {
                    swap(l, r);
                }
                vector<int> b(a.begin() + l, a.begin() + r + 1);
                sort(b.begin(), b.end());
                assert(p.count(rt[l - 1], rt[r]) == (int)b.size());
                for (int k = 0; k < (int)b.size(); k++)
                {
                    assert(p.kth(rt[l - 1], rt[r], 1, 20, k + 1) == b[k]);
                }
            }
        }
    }
    cout << "PASS persistent_segment_tree_count\n";
}
