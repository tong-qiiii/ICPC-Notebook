#include "code/geometry/geometry_3d.hpp"
int main()
{
    mt19937 gen(20261016);
    auto p = rotate_around_axis(Point3{1, 0, 0}, Point3{0, 0, 1}, PI / 2);
    assert(dist(p, {0, 1, 0}) < 1e-8);
    {
        for (int t = 0; t < 1000; t++)
        {
            Point3 v{double(gen() % 21), double(gen() % 21),
                     double(gen() % 21)},
                axis{1, 2, 3};
            double angle = double(gen() % 100) / 10;
            auto p = rotate_around_axis(v, axis, angle);
            assert(fabs(len(p) - len(v)) < 1e-7);
            assert(fabs(dot(p, axis) - dot(v, axis)) < 1e-7);
        }
        Plane pl{{0, 0, 2}, {0, 0, 1}};
        auto p = line_plane_intersection(Line3{{1, 1, 0}, {0, 0, 1}}, pl);
        assert(p && fabs(p->z - 2) < 1e-8);
    }
    cout << "PASS geometry_3d\n";
}
