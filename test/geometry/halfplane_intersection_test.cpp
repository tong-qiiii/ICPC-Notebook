#include "code/geometry/halfplane_intersection.hpp"
int main()
{
    mt19937 gen(20261016);
    vector<HalfPlane> h{{{0, 0}, {2, 0}},
                        {{2, 0}, {2, 3}},
                        {{2, 3}, {0, 3}},
                        {{0, 3}, {0, 0}}};
    auto p = halfplane_intersection(h);
    assert(fabs(polygon_area(p) - 6) < 1e-8);
    {
        for (int t = 0; t < 1000; t++)
        {
            double x1 = gen() % 20, y1 = gen() % 20,
                   x2 = x1 + gen() % 20 + 1, y2 = y1 + gen() % 20 + 1;
            vector<HalfPlane> h{{{x1, y1}, {x2, y1}},
                                {{x2, y1}, {x2, y2}},
                                {{x2, y2}, {x1, y2}},
                                {{x1, y2}, {x1, y1}}};
            h.push_back(h[0]);
            shuffle(h.begin(), h.end(), gen);
            auto p = halfplane_intersection(h);
            assert(fabs(polygon_area(p) - (x2 - x1) * (y2 - y1)) < 1e-7);
        }
    }
    // Independent polygon clipping also exercises both ends of the HPI deque.
    for (int run = 0; run < 1000; ++run)
    {
        vector<PointD> clipped{{-20, -20}, {20, -20}, {20, 20}, {-20, 20}};
        vector<HalfPlane> h{{{-20, -20}, {20, -20}},
                            {{20, -20}, {20, 20}},
                            {{20, 20}, {-20, 20}},
                            {{-20, 20}, {-20, -20}}};
        for (int i = 0; i < 15; ++i)
        {
            PointD p{double(int(gen() % 31) - 15), double(int(gen() % 31) - 15)};
            PointD v{double(int(gen() % 21) - 10), double(int(gen() % 21) - 10)};
            if (len2(v) == 0)
            {
                v = {1, 0};
            }
            h.emplace_back(LineD{p, v});
            vector<PointD> next;
            for (int j = 0; j < (int)clipped.size(); ++j)
            {
                PointD a = clipped[j], b = clipped[(j + 1) % clipped.size()];
                double da = cross(v, a - p), db_ = cross(v, b - p);
                if (da >= -EPS)
                {
                    next.push_back(a);
                }
                if ((da > EPS && db_ < -EPS) || (da < -EPS && db_ > EPS))
                {
                    next.push_back(a + (b - a) * (da / (da - db_)));
                }
            }
            clipped = move(next);
        }
        shuffle(h.begin(), h.end(), gen);
        auto got = halfplane_intersection(h);
        assert(fabs(polygon_area(got) - polygon_area(clipped)) < 1e-6);
        for (auto p : got)
        {
            for (auto hp : h)
            {
                assert(!hp.outside(p));
            }
        }
    }
    cout << "PASS halfplane_intersection\n";
}
