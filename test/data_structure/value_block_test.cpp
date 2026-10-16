#include "code/data_structure/value_block.hpp"
int main()
{
    mt19937 gen(20261016);
    ValueBlock b(5);
    b.add(2, 2);
    b.add(5, 1);
    assert(b.kth(3) == 5 && b.prev(5) == 2 && b.next(2) == 5);
    {
        ValueBlock b;
        for (int t = 0; t < 20; t++)
        {
            b.init(30);
            multiset<int> s;
            for (int j = 0; j < 200; j++)
            {
                int v = gen() % 30 + 1;
                auto it = s.find(v);
                if (it != s.end() && gen() % 2)
                {
                    s.erase(it);
                    b.add(v, -1);
                }
                else
                {
                    s.insert(v);
                    b.add(v, 1);
                }
                vector<int> a(s.begin(), s.end());
                assert(b.kth(0) == -1 && b.kth(a.size() + 1) == -1);
                for (int i = 0; i < (int)a.size(); i++)
                {
                    assert(b.kth(i + 1) == a[i]);
                }
                for (int v = -1; v <= 32; v++)
                {
                    assert(b.count_le(v) ==
                           upper_bound(a.begin(), a.end(), v) - a.begin());
                    assert(b.count_lt(v) ==
                           lower_bound(a.begin(), a.end(), v) - a.begin());
                    auto l = lower_bound(a.begin(), a.end(), v),
                         r = upper_bound(a.begin(), a.end(), v);
                    assert(b.prev(v) == (l == a.begin() ? -1 : *prev(l)));
                    assert(b.next(v) == (r == a.end() ? -1 : *r));
                }
            }
        }
        assert(b.count_lt(INT_MIN) == 0);
    }
    cout << "PASS value_block\n";
}
