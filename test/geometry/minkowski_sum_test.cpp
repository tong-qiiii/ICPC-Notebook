#include "code/geometry/minkowski_sum.hpp"
int main()
{
    mt19937 gen(20261016);
    auto p =
        minkowski_sum(convex_hull(vector<PointI>{{0, 0}, {1, 0}, {0, 1}}),
                      vector<PointI>{{1, 2}});
    assert(p.size() == 3);
    {
        for (int t = 0; t < 1000; t++)
        {
            vector<PointI> a, b, all;
            for (int i = gen() % 12; i; i--)
            {
                a.push_back({int(gen() % 21) - 10, int(gen() % 21) - 10});
            }
            for (int i = gen() % 12; i; i--)
            {
                b.push_back({int(gen() % 21) - 10, int(gen() % 21) - 10});
            }
            for (auto x : a)
            {
                for (auto y : b)
                {
                    all.push_back(x + y);
                }
            }
            auto ref = convex_hull(all),
                 got = convex_hull(
                     minkowski_sum(convex_hull(a), convex_hull(b)));
            assert(ref == got);
        }
    }
    cout << "PASS minkowski_sum\n";
}
