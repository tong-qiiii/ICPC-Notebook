#include "code/data_structure/fhq_set.hpp"
int main()
{
    mt19937 gen(20261016);
    FHQSet s;
    s.insert(3);
    s.insert(1);
    s.insert(3);
    s.erase(3);
    assert(s.kth(2) == 3 && s.rank_of(3) == 2);
    {
        FHQSet s;
        for (int run = 0; run < 10; run++)
        {
            s.clear();
            s.rng.seed(run);
            multiset<int> ref;
            for (int j = 0; j < 500; j++)
            {
                int v = int(gen() % 101) - 50;
                if (gen() % 2)
                {
                    s.insert(v);
                    ref.insert(v);
                }
                else
                {
                    s.erase(v);
                    auto it = ref.find(v);
                    if (it != ref.end())
                    {
                        ref.erase(it);
                    }
                }
                vector<int> a(ref.begin(), ref.end());
                for (int i = 0; i < (int)a.size(); i++)
                {
                    assert(s.kth(i + 1) == a[i]);
                }
                assert(s.rank_of(v) ==
                       lower_bound(a.begin(), a.end(), v) - a.begin() + 1);
                auto l = lower_bound(a.begin(), a.end(), v),
                     r = upper_bound(a.begin(), a.end(), v);
                assert(s.pre(v) == (l == a.begin() ? INT_MIN : *prev(l)));
                assert(s.nxt(v) == (r == a.end() ? INT_MAX : *r));
            }
        }
    }
    cout << "PASS fhq_set\n";
}
