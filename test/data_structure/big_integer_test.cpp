#include "code/data_structure/big_integer.hpp"
int main()
{
    mt19937 gen(20261016);
    BigInt a = BigInt(1) << 200;
    assert((a * a / a) == a);
    assert((a + 123) % a == 123);
    {
        for (int i = 0; i < 500; i++)
        {
            ll a = int(gen() % 1000000) - 500000, b = gen() % 999999 + 1;
            BigInt x = a, y = b;
            assert((x + y).convert_to<ll>() == a + b);
            assert((x * y).convert_to<ll>() == a * b);
            assert((x / y).convert_to<ll>() == a / b);
            assert((x % y).convert_to<ll>() == a % b);
        }
    }
    cout << "PASS big_integer\n";
}
