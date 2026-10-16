#include "code/geometry/closest_pair.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(closest_pair(vector<PointI>{{0, 0}, {3, 4}, {0, 1}})->d2 == 1);
    assert(!closest_pair({}));
    {
        for (int t = 0; t < 1000; t++)
        {
            vector<PointI> p;
            for (int i = gen() % 40 + 2; i; i--)
            {
                p.push_back({int(gen() % 101) - 50, int(gen() % 101) - 50});
            }
            i128 ref = dist2(p[0], p[1]);
            for (int i = 0; i < (int)p.size(); i++)
            {
                for (int j = 0; j < i; j++)
                {
                    ref = min(ref, dist2(p[i], p[j]));
                }
            }
            assert(closest_pair(p)->d2 == ref);
        }
    }
    cout << "PASS closest_pair\n";
}
