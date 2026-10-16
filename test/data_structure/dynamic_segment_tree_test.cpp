#include "code/data_structure/dynamic_segment_tree.hpp"
int main()
{
    mt19937 gen(20261016);
    DynSeg t(10);
    t.assign(3, 7, 0);
    assert(t.remain() == 5);
    t.assign(5, 6, 1);
    assert(t.remain() == 7);
    {
        DynSeg t;
        for (int n = 0; n <= 60; n++)
        {
            t.init(n);
            vector<int> a(n + 1, 1);
            for (int j = 0; j < 200; j++)
            {
                int l = int(gen() % (n + 5)) - 2, r = int(gen() % (n + 5)) - 2;
                if (l > r)
                {
                    swap(l, r);
                }
                int v = gen() % 2;
                t.assign(l, r, v);
                for (int i = max(l, 1); i <= min(r, n); i++)
                {
                    a[i] = v;
                }
                assert(t.remain() == accumulate(a.begin() + 1, a.end(), 0));
                assert(t.occupied() == n - t.remain());
            }
        }
    }
    cout << "PASS dynamic_segment_tree\n";
}
