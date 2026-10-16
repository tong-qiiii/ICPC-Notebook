#include "code/geometry/circle.hpp"
int main()
{
    mt19937 gen(20261016);
    Circle a{{0, 0}, 1}, b{{2, 0}, 1};
    assert(circle_relation(a, b) == CircleRelation::EXTERNALLY_TANGENT);
    assert(circle_circle_intersection(a, b).points.size() == 1);
    assert(fabs(circle_intersection_area(a, a) - PI) < 1e-8);
    {
        for (int t = 0; t < 2000; t++)
        {
            Circle a{{double(gen() % 21), double(gen() % 21)},
                     double(gen() % 10 + 1)},
                b{{double(gen() % 21), double(gen() % 21)},
                  double(gen() % 10 + 1)};
            auto r = circle_circle_intersection(a, b);
            for (auto p : r.points)
            {
                assert(fabs(dist(p, a.o) - a.r) < 1e-7 &&
                       fabs(dist(p, b.o) - b.r) < 1e-7);
            }
            double s = circle_intersection_area(a, b);
            assert(s >= -1e-8 &&
                   s <= PI * min(a.r * a.r, b.r * b.r) + 1e-8);
            assert(fabs(s - circle_intersection_area(b, a)) < 1e-7);
        }
        assert(
            circle_circle_intersection(Circle{{0, 0}, 0}, Circle{{0, 0}, 0})
                .type == CircleIntersectionType::ONE);
    }
    cout << "PASS circle\n";
}
