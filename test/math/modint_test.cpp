#include "code/math/modint.hpp"
int main()
{
    mt19937 gen(20261016);
    using Z = Mint<101>;
    assert((Z(-1) + 2).v == 1 && (Z(8) / 2).v == 4);
    {
        using Z = Mint<101>;
        for (int a = -300; a <= 300; a++)
        {
            for (int b = -100; b <= 100; b++)
            {
                int x = (a % 101 + 101) % 101, y = (b % 101 + 101) % 101;
                assert((Z(a) + Z(b)).v == (x + y) % 101);
                assert((Z(a) * Z(b)).v == x * y % 101);
                if (y)
                {
                    assert((Z(a) / Z(b) * Z(b)) == Z(a));
                }
            }
        }
    }
    cout << "PASS modint\n";
}
