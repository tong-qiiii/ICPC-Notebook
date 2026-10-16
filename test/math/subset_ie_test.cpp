#include "code/math/subset_ie.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(subset_ie(3, [](uint64_t) { return 1LL; }) == 1);
    {
        for (int n = 0; n < 15; n++)
        {
            assert(subset_ie(n, [](uint64_t) { return 1LL; }) == (n != 0));
        }
    }
    cout << "PASS subset_ie\n";
}
