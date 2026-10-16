#include "code/data_structure/monotonic_stack.hpp"
int main()
{
    mt19937 gen(20261016);
    assert((nearest_smaller(vector<int>{0, 2, 2, 1, 3}) == vector<int>{0, 0, 0, 0, 3}));
    {
        for (int n = 0; n < 80; n++)
        {
            vector<int> a(n + 1);
            for (int i = 1; i <= n; i++)
            {
                a[i] = gen() % 10;
            }
            auto r = nearest_smaller(a);
            for (int i = 1; i <= n; i++)
            {
                int j = i - 1;
                while (j && a[j] >= a[i])
                {
                    j--;
                }
                assert(r[i] == j);
            }
        }
    }
    cout << "PASS monotonic_stack\n";
}
