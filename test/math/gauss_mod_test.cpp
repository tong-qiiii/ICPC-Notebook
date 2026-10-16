#include "code/math/gauss_mod.hpp"
int main()
{
    mt19937 gen(20261016);
    using Z = Mint<7>;
    GaussM<7> g;
    vector<Z> a;
    assert(g.solve({{1, 1, 3}, {1, -1, 1}}, a) == 2 && a[0].v == 2 &&
           a[1].v == 1);
    {
        using Z = Mint<7>;
        GaussM<7> g;
        vector<Z> a;
        for (int t = 0; t < 200; t++)
        {
            vector<vector<Z>> m(3, vector<Z>(4));
            for (auto &r : m)
            {
                for (auto &x : r)
                {
                    x = gen() % 7;
                }
            }
            int cnt = 0;
            for (int x = 0; x < 7; x++)
            {
                for (int y = 0; y < 7; y++)
                {
                    for (int z = 0; z < 7; z++)
                    {
                        bool ok = true;
                        for (auto &r : m)
                        {
                            ok &= r[0] * x + r[1] * y + r[2] * z == r[3];
                        }
                        cnt += ok;
                    }
                }
            }
            int rank = g.solve(m, a);
            assert((rank == -1) == (cnt == 0));
            if (rank != -1)
            {
                for (auto &r : m)
                {
                    assert(r[0] * a[0] + r[1] * a[1] + r[2] * a[2] == r[3]);
                }
                assert((rank == 3) == (cnt == 1));
            }
        }
    }
    cout << "PASS gauss_mod\n";
}
