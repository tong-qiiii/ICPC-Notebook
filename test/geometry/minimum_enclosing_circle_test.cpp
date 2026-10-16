#include "code/geometry/minimum_enclosing_circle.hpp"
int main()
{
    mt19937 gen(20261016);
    auto c =
        minimum_enclosing_circle(vector<PointD>{{0, 0}, {4, 0}, {1, 1}});
    assert(same_point(c.o, {2, 0}) && fabs(c.r - 2) < 1e-8);
    {
        for (int t = 0; t < 500; t++)
        {
            vector<PointD> p;
            for (int i = gen() % 8 + 1; i; i--)
            {
                p.push_back({double(gen() % 21), double(gen() % 21)});
            }
            auto result = minimum_enclosing_circle(p);
            double best = 1e100;
            auto test = [&](Circle c)
            {
                bool ok = true;
                for (auto x : p)
                {
                    ok &= in_circle(x, c);
                }
                if (ok)
                {
                    best = min(best, c.r);
                }
            };
            for (auto a : p)
            {
                test({a, 0});
            }
            for (auto a : p)
            {
                for (auto b : p)
                {
                    test(circle_from_diameter(a, b));
                }
            }
            for (auto a : p)
            {
                for (auto b : p)
                {
                    for (auto c : p)
                    {
                        auto r = circumcircle(a, b, c);
                        if (r)
                        {
                            test(*r);
                        }
                    }
                }
            }
            assert(fabs(result.r - best) < 1e-7);
            for (auto x : p)
            {
                assert(in_circle(x, result));
            }
        }
    }
    cout << "PASS minimum_enclosing_circle\n";
}
