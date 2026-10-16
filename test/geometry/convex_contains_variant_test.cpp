#include "code/geometry/convex_contains_variant.hpp"
int main()
{
    mt19937 gen(20261016);
    auto h = convex_hull(vector<PointI>{{0, 0}, {4, 0}, {4, 4}, {0, 4}});
    assert(point_in_convex_polygon(h, {2, 2}) == PointInPolygon::IN);
    {
        for (int t = 0; t < 500; t++)
        {
            vector<PointI> p;
            for (int i = gen() % 30; i; i--)
            {
                p.push_back({int(gen() % 21) - 10, int(gen() % 21) - 10});
            }
            for (bool keep : {false, true})
            {
                auto h = convex_hull(p, keep);
                for (int q = 0; q < 100; q++)
                {
                    PointI x{int(gen() % 25) - 12, int(gen() % 25) - 12};
                    assert(point_in_convex_polygon(h, x) ==
                           point_in_polygon(h, x));
                }
            }
        }
    }
    cout << "PASS convex_contains_variant\n";
}
