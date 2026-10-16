#include "code/geometry/pick.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(interior_lattice_points(
               vector<PointI>{{0, 0}, {3, 0}, {3, 3}, {0, 3}}) == 4);
    {
        for (int w = 1; w < 50; w++)
        {
            for (int h = 1; h < 50; h++)
            {
                vector<PointI> p{{0, 0}, {w, 0}, {w, h}, {0, h}};
                assert(boundary_lattice_points(p) == 2 * (w + h));
                assert(interior_lattice_points(p) ==
                       1LL * (w - 1) * (h - 1));
            }
        }
    }
    cout << "PASS pick\n";
}
