#include "code/geometry/convex_diameter.hpp"
int main()
{
    mt19937 gen(20261016);
    auto h = convex_hull(vector<PointI>{{0, 0}, {3, 0}, {0, 4}});
    assert(convex_diameter(h)->d2 == 25);
    {
        assert(!convex_diameter({}));
        for (int t = 0; t < 1000; t++)
        {
            vector<PointI> p;
            for (int i = gen() % 40 + 1; i; i--)
            {
                p.push_back({int(gen() % 41) - 20, int(gen() % 41) - 20});
            }
            i128 z = 0;
            for (auto a : p)
            {
                for (auto b : p)
                {
                    z = max(z, dist2(a, b));
                }
            }
            assert(convex_diameter(convex_hull(p))->d2 == z);
        }
    }
    cout << "PASS convex_diameter\n";
}
