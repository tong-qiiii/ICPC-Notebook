#include "code/math/lucas.hpp"
int main()
{
    mt19937 gen(20261016);
    Lucas<7> l;
    assert(l.C(10, 3).v == 1);
    {
        Lucas<7> l;
        vector<vector<int>> p(201, vector<int>(201));
        p[0][0] = 1;
        for (int n = 0; n <= 200; n++)
        {
            for (int k = 0; k <= n; k++)
            {
                if (n)
                {
                    p[n][k] = ((k ? p[n - 1][k - 1] : 0) + p[n - 1][k]) % 7;
                }
                assert(l.C(n, k).v == p[n][k]);
            }
        }
    }
    cout << "PASS lucas\n";
}
