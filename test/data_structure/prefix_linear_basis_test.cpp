#include "code/data_structure/prefix_linear_basis.hpp"
int main()
{
    mt19937 gen(20261016);
    LinearBasis b;
    b.insert(3, 1);
    b.insert(5, 2);
    assert(b.query(1) == 6 && b.query(2) == 5);
    {
        for (int t = 0; t < 100; t++)
        {
            int n = 10;
            vector<int> a(n + 1);
            vector<LinearBasis> p(n + 1);
            for (int i = 1; i <= n; i++)
            {
                a[i] = gen() % 128;
                p[i] = p[i - 1];
                p[i].insert(a[i], i);
            }
            for (int l = 1; l <= n; l++)
            {
                for (int r = l; r <= n; r++)
                {
                    int z = 0;
                    for (int mask = 0; mask < (1 << (r - l + 1)); mask++)
                    {
                        int v = 0;
                        for (int i = l; i <= r; i++)
                        {
                            if (mask >> (i - l) & 1)
                            {
                                v ^= a[i];
                            }
                        }
                        z = max(z, v);
                    }
                    assert(p[r].query(l) == z);
                }
            }
        }
    }
    cout << "PASS prefix_linear_basis\n";
}
