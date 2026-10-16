#include "code/data_structure/interval_union.hpp"
int main()
{
    mt19937 gen(20261016);
    IntSet s;
    s.add(1, 3);
    s.add(4, 6);
    s.erase(2, 5);
    assert(s.contains(1) && s.contains(6) && !s.contains(3));
    {
        IntSet s;
        for (int run = 0; run < 10; run++)
        {
            s.clear();
            set<int> r;
            for (int j = 0; j < 500; j++)
            {
                int l = int(gen() % 61) - 30, h = int(gen() % 61) - 30;
                if (l > h)
                {
                    swap(l, h);
                }
                if (gen() % 2)
                {
                    s.add(l, h);
                    for (int x = l; x <= h; x++)
                    {
                        r.insert(x);
                    }
                }
                else
                {
                    s.erase(l, h);
                    for (int x = l; x <= h; x++)
                    {
                        r.erase(x);
                    }
                }
                for (int x = -32; x <= 32; x++)
                {
                    assert(s.contains(x) == bool(r.count(x)));
                }
            }
        }
        s.clear();
        s.add(LLONG_MIN, LLONG_MIN);
        s.add(LLONG_MAX, LLONG_MAX);
        s.erase(LLONG_MIN, LLONG_MAX);
        assert(s.seg.empty());
    }
    cout << "PASS interval_union\n";
}
