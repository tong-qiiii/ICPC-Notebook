#include "code/math/combination.hpp"
int main()
{
    mt19937 gen(20261016);
    Comb<101> c(30);
    assert(c.C(5, 2).v == 10 && c.A(5, 2).v == 20);
    {
        Comb<101> c;
        vector<vector<int>> p(101, vector<int>(101));
        p[0][0] = 1;
        for (int n = 0; n < 101; n++)
        {
            c.init(n);
            for (int k = 0; k <= n; k++)
            {
                if (n)
                {
                    p[n][k] =
                        ((k ? p[n - 1][k - 1] : 0) + p[n - 1][k]) % 101;
                }
                assert(c.C(n, k).v == p[n][k]);
            }
            assert(c.C(n, -1).v == 0 && c.C(n, n + 1).v == 0);
        }
    }
    cout << "PASS combination\n";
}
