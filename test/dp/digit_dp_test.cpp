#include "code/dp/digit_dp.hpp"
int main()
{
    mt19937 gen(20261016);
    auto next = [](int st, int d, bool) { return (st + d) % 3; };
    auto accept = [](int st, bool) { return st == 0; };
    assert(digit_query(0, 20, 3, 0, next, accept) == 7);
    {
        for (int m = 1; m <= 10; m++)
        {
            auto next = [&](int st, int d, bool) { return (st + d) % m; };
            auto accept = [](int st, bool) { return st == 0; };
            ll count = 0;
            for (int x = 0; x < 3000; x++)
            {
                int z = x, sum = 0;
                while (z)
                {
                    sum += z % 10;
                    z /= 10;
                }
                count += sum % m == 0;
                assert(digit_count(x, m, 0, next, accept) == count);
            }
        }
        assert(digit_count(
                   LLONG_MAX, 1, 0, [](int, int, bool) { return 0; },
                   [](int, bool)
                   { return true; }) == (__int128)LLONG_MAX + 1);
        assert(digit_query(
                   LLONG_MIN, 0, 1, 0, [](int, int, bool) { return 0; },
                   [](int, bool) { return true; }) == 1);
    }
    cout << "PASS digit_dp\n";
}
