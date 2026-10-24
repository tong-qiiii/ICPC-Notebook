#include "code/math/combination.hpp"
int main()
{
    mt19937 gen(20261016);
    Comb<101> c(30);
    Comb<101> zero;
    assert(zero.C(0, 0).v == 1 && zero.A(0, 0).v == 1);
    assert(zero.C(-1, 0).v == 0 && zero.C(20, 21).v == 0);
    for (int op = 0; op < 4; op++)
    {
        bool caught = false;
        try
        {
            if (op == 0) zero.C(1, 0);
            if (op == 1) zero.A(1, 1);
            if (op == 2) zero.init(-1);
            if (op == 3) zero.init(101);
        }
        catch (const logic_error &)
        {
            caught = true;
        }
        if (!caught) return 1; // NDEBUG 下同样检查保护。
    }
    zero.init(5);
    assert(zero.C(5, 2).v == 10 && zero.A(5, 2).v == 20);
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
