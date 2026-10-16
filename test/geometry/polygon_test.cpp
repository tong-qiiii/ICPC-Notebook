#include "code/geometry/polygon.hpp"
int main()
{
    mt19937 gen(20261016);
    vector<PointI> p{{0, 0}, {4, 0}, {4, 4}, {0, 4}};
    assert(polygon_area2(p) == 32 &&
           point_in_polygon(p, PointI{2, 2}) == PointInPolygon::IN);
    {
        for (int w = 1; w < 30; w++)
        {
            for (int h = 1; h < 30; h++)
            {
                vector<PointI> p{{0, 0}, {w, 0}, {w, h}, {0, h}};
                auto c = polygon_centroid(p);
                assert(c && fabs(c->x - w / 2.) < 1e-9 &&
                       fabs(c->y - h / 2.) < 1e-9);
                for (int x = -1; x <= w + 1; x++)
                {
                    for (int y = -1; y <= h + 1; y++)
                    {
                        auto ref =
                            (x < 0 || x > w || y < 0 || y > h)
                                ? PointInPolygon::OUT
                                : ((x == 0 || x == w || y == 0 || y == h)
                                       ? PointInPolygon::ON
                                       : PointInPolygon::IN);
                        assert(point_in_polygon(p, PointI{x, y}) == ref);
                    }
                }
            }
        }
    }
    cout << "PASS polygon\n";
}
