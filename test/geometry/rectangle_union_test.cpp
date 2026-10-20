#include "code/geometry/rectangle_union.hpp"

int main()
{
    assert(rectangle_union({}) == 0);
    assert(rectangle_union({{0, 0, 2, 2}, {1, 1, 3, 3}}) == 7);
    ll v = 1000000000000000000LL;
    assert(rectangle_union({{-v, -v, v, v}}) == (__int128_t)4 * v * v);
    mt19937 rg(731);
    for (int z = 0; z < 1500; z++)
    {
        vector<array<ll, 4>> a;
        bool cell[12][12] = {};
        int n = rg() % 15;
        for (int i = 0; i < n; i++)
        {
            int x1 = rg() % 11, x2 = rg() % 11;
            int y1 = rg() % 11, y2 = rg() % 11;
            a.push_back({x1 - 5, y1 - 5, x2 - 5, y2 - 5});
            for (int x = min(x1, x2); x < max(x1, x2); x++)
                for (int y = min(y1, y2); y < max(y1, y2); y++) cell[x][y] = true;
        }
        int ans = 0;
        for (auto &row : cell) for (bool x : row) ans += x;
        assert(rectangle_union(a) == ans);
        assert(rectangle_union(a) == ans);
    }
    cout << "PASS rectangle_union: grid brute, degenerate, duplicate, large coordinates\n";
}
