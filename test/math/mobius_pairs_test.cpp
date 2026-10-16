#include "code/math/mobius_pairs.hpp"
int main()
{
    mt19937 gen(20261016);
    Mobius m(10);
    assert(m.coprime_pairs(3, 3) == 7 && m.gcd_eq(6, 6, 2) == 7);
    {
        Mobius m(60);
        for (int a = 0; a <= 60; a++)
        {
            for (int b = 0; b <= 60; b++)
            {
                ll z = 0;
                for (int i = 1; i <= a; i++)
                {
                    for (int j = 1; j <= b; j++)
                    {
                        z += gcd(i, j) == 1;
                    }
                }
                assert(m.coprime_pairs(a, b) == z);
            }
        }
    }
    cout << "PASS mobius_pairs\n";
}
