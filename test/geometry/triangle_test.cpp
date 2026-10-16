#include "code/geometry/triangle.hpp"
int main()
{
    mt19937 gen(20261016);
    auto o = circumcenter({0, 0}, {4, 0}, {0, 2});
    assert(o && same_point(*o, {2, 1}));
    assert(!circumcenter({0, 0}, {1, 0}, {2, 0}));
    {
        for (int t = 0; t < 1000; t++)
        {
            PointD a{double(gen() % 20), double(gen() % 20)},
                b{double(gen() % 20), double(gen() % 20)},
                c{double(gen() % 20), double(gen() % 20)};
            auto o = circumcenter(a, b, c);
            if (o)
            {
                assert(fabs(dist(*o, a) - dist(*o, b)) < 1e-7);
                assert(fabs(dist(*o, a) - dist(*o, c)) < 1e-7);
            }
            auto in = incenter(a, b, c);
            if (in)
            {
                assert(triangle_area(a, b, c) > 0);
            }
        }
    }
    cout << "PASS triangle\n";
}
