#include "code/math/division_blocks.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(floor_sum(5) == 10);
    {
        for (int n = 0; n < 2000; n++)
        {
            ll s = 0;
            for (int i = 1; i <= n; i++)
            {
                s += n / i;
            }
            assert(floor_sum(n) == s);
        }
    }
    cout << "PASS division_blocks\n";
}
