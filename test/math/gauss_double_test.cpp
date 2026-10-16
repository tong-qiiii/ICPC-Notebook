#include "code/math/gauss_double.hpp"
int main()
{
    mt19937 gen(20261016);
    GaussD g;
    vector<double> a;
    assert(g.solve({{1, 1, 3}, {1, -1, 1}}, a) == 2);
    assert(fabs(a[0] - 2) < 1e-8 && fabs(a[1] - 1) < 1e-8);
    {
        GaussD g;
        vector<double> a;
        assert(g.solve({{0, 1}}, a) == -1);
        assert(g.solve({{1, 1, 2}}, a) == 1);
        assert(g.solve({}, a) == 0);
        for (int t = 0; t < 300; t++)
        {
            int n = 5;
            vector<vector<double>> m(n, vector<double>(n + 1));
            vector<double> x(n);
            for (auto &v : x)
            {
                v = int(gen() % 21) - 10;
            }
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    m[i][j] = (i == j ? 100 : 0) + int(gen() % 11) - 5;
                    m[i][n] += m[i][j] * x[j];
                }
            }
            assert(g.solve(m, a) == n);
            for (int i = 0; i < n; i++)
            {
                assert(fabs(a[i] - x[i]) < 1e-7);
            }
        }
    }
    cout << "PASS gauss_double\n";
}
