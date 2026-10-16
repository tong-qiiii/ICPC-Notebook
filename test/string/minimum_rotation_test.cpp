#include "code/string/minimum_rotation.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(minRepresentation("baca") == 3 && minRepresentation("") == 0);
    {
        for (int run = 0; run < 500; run++)
        {
            int n = gen() % 40;
            string s;
            for (int i = 0; i < n; i++)
            {
                s += char('a' + gen() % 4);
            }
            int p = minRepresentation(s);
            if (!n)
            {
                assert(p == 0);
                continue;
            }
            string got = s.substr(p) + s.substr(0, p), ref = s;
            for (int i = 1; i < n; i++)
            {
                ref = min(ref, s.substr(i) + s.substr(0, i));
            }
            assert(got == ref);
        }
    }
    cout << "PASS minimum_rotation\n";
}
