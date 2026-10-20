#include "code/geometry/geometry_base.hpp"
static_assert(is_same_v<decltype(len2(PointI{})), i128>);
static_assert(is_same_v<decltype(len2(PointD{})), db>);
static_assert(is_same_v<decltype(len(PointI{})), long double>);
static_assert(is_same_v<decltype(norm(PointI{})), i128>);
static_assert(is_same_v<decltype(orient(PointI{}, PointI{}, PointI{})), int>);
int main()
{
    assert(same_point({0, 0}, {0, EPS / 2}));
    assert(!same_point(PointI{0, 0}, PointI{1, 0}));
    mt19937 gen(20261016);
    PointI a{0, 0}, b{3, 0}, c{0, 4};
    assert(cross(a, b, c) == 12 && dist2(b, c) == 25);
    {
        for (int i = 0; i < 2000; i++)
        {
            PointI a{int(gen() % 201) - 100, int(gen() % 201) - 100},
                b{int(gen() % 201) - 100, int(gen() % 201) - 100};
            assert(dot(a, b) == dot(b, a));
            assert(cross(a, b) == -cross(b, a));
            assert(dist2(a, b) == dist2(b, a));
            auto z = rotate(toD(a), PI / 2);
            assert(dist(z, toD(perp(a))) < 1e-8);
        }
        assert(cross(PointI{-4000000000LL, 0}, PointI{4000000000LL, 0},
                     PointI{0, 4000000000LL}) > LLONG_MAX);
    }
    cout << "PASS geometry_base\n";
}
