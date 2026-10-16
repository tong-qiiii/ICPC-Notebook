#include "code/data_structure/persistent_mex.hpp"
int main()
{
    mt19937 gen(20261016);
    PSTMex p;
    p.build(vector<int>{0, 0, 1, 0});
    assert(p.query(1, 3) == 2);
    {
        PSTMex p;
        for (int n = 1; n <= 50; n++)
        {
            vector<int> a(n + 1);
            for (int i = 1; i <= n; i++)
            {
                a[i] = int(gen() % 20) - 3;
            }
            p.build(a);
            for (int l = 1; l <= n; l++)
            {
                for (int r = l; r <= n; r++)
                {
                    set<int> s(a.begin() + l, a.begin() + r + 1);
                    int z = 0;
                    while (s.count(z))
                    {
                        z++;
                    }
                    assert(p.query(l, r) == z);
                }
            }
        }
    }
    cout << "PASS persistent_mex\n";
}
