#include "code/geometry/circle_tangents.hpp"
int main()
{
    mt19937 gen(20261016);
    auto r = common_tangents(Circle{{0, 0}, 1}, Circle{{2, 0}, 1});
    assert(r.tangents.size() == 3);
    {
        for (int t = 0; t < 2000; t++)
        {
            Circle a{{double(gen() % 21), double(gen() % 21)},
                     double(gen() % 10 + 1)},
                b{{double(gen() % 21), double(gen() % 21)},
                  double(gen() % 10 + 1)};
            auto r = common_tangents(a, b);
            for (auto line : r.tangents)
            {
                assert(len2(line.line.v) > 1e-15);
                assert(fabs(distance_to_line(a.o, line.line) - a.r) <
                           1e-7 &&
                       fabs(distance_to_line(b.o, line.line) - b.r) < 1e-7);
            }
            PointD p{30, 30};
            for (auto q : tangent_points_from_point(p, a))
            {
                assert(fabs(dist(q, a.o) - a.r) < 1e-7);
                assert(fabs(dot(p - q, a.o - q)) < 1e-6);
            }
        }
    }
    cout << "PASS circle_tangents\n";
}
