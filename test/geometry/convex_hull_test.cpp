#include "code/geometry/convex_hull.hpp"
int main()
{
    mt19937 gen(20261016);
    auto h =
        convex_hull(vector<PointI>{{0, 0}, {2, 0}, {0, 2}, {1, 0}, {0, 0}});
    assert(h.size() == 3);
    {
        assert(convex_hull({}).empty());
        assert(convex_hull({{0, 0}, {1, 1}, {2, 2}}).size() == 2);
        assert(convex_hull({{0, 0}, {1, 1}, {2, 2}}, true).size() == 3);
        for (int t = 0; t < 1000; t++)
        {
            vector<PointI> p;
            for (int i = gen() % 40; i; i--)
            {
                p.push_back({int(gen() % 31) - 15, int(gen() % 31) - 15});
            }
            auto h = convex_hull(p);
            if (h.size() >= 3)
            {
                for (int i = 0; i < (int)h.size(); i++)
                {
                    assert(orient(h[i], h[(i + 1) % h.size()],
                                  h[(i + 2) % h.size()]) > 0);
                }
                for (auto x : p)
                {
                    for (int i = 0; i < (int)h.size(); i++)
                    {
                        assert(orient(h[i], h[(i + 1) % h.size()], x) >= 0);
                    }
                }
            }
        }
    }
    cout << "PASS convex_hull\n";
}
