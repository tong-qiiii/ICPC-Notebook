#include "code/geometry/bounding_rectangle.hpp"
int main()
{
    mt19937 gen(20261016);
    auto h = convex_hull(vector<PointI>{{0, 0}, {3, 0}, {3, 2}, {0, 2}});
    assert(fabs(min_area_bounding_rectangle(h) - 6) < 1e-8);
    {
        for (int t = 0; t < 1000; t++)
        {
            vector<PointI> p;
            for (int i = gen() % 30 + 1; i; i--)
            {
                p.push_back({int(gen() % 41) - 20, int(gen() % 41) - 20});
            }
            auto h = convex_hull(p);
            long double ref = 1e100;
            if (h.size() < 3)
            {
                ref = 0;
            }
            else
            {
                for (int i = 0; i < (int)h.size(); i++)
                {
                    PointI e = h[(i + 1) % h.size()] - h[i];
                    i128 mn = dot(e, h[0]), mx = mn, cr = cross(e, h[0]),
                         lo = cr, hi = cr;
                    for (auto x : h)
                    {
                        mn = min(mn, dot(e, x));
                        mx = max(mx, dot(e, x));
                        lo = min(lo, cross(e, x));
                        hi = max(hi, cross(e, x));
                    }
                    ref = min(ref, (long double)(mx - mn) *
                                       (long double)(hi - lo) /
                                       (long double)len2(e));
                }
            }
            assert(fabsl(min_area_bounding_rectangle(h) - ref) < 1e-7);
        }
    }
    cout << "PASS bounding_rectangle\n";
}
