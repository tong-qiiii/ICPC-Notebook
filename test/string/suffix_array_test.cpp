#include "code/string/suffix_array.hpp"
int main()
{
    mt19937 gen(20261016);
    assert((buildSA("banana") == vector<int>{5, 3, 1, 0, 4, 2}));
    {
        for (int t = 0; t < 1000; t++)
        {
            string s;
            for (int j = gen() % 90; j; j--)
            {
                s += char(gen() % 256);
            }
            vector<int> ref(s.size());
            iota(ref.begin(), ref.end(), 0);
            sort(ref.begin(), ref.end(),
                 [&](int a, int b) { return s.substr(a) < s.substr(b); });
            assert(buildSA(s) == ref);
        }
    }
    cout << "PASS suffix_array\n";
}
