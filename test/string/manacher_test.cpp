#include "code/string/manacher.hpp"
int main()
{
    mt19937 gen(20261016);
    Manacher m("abacaba");
    assert(m.longest() == 7 && m.is_pal(1, 5));
    {
        Manacher m;
        for (int run = 0; run < 500; run++)
        {
            int n = gen() % 40;
            string s;
            for (int i = 0; i < n; i++)
            {
                s += char('a' + gen() % 4);
            }
            m.build(s);
            int best = 0;
            for (int l = 0; l < n; l++)
            {
                for (int r = l; r < n; r++)
                {
                    string t = s.substr(l, r - l + 1), u = t;
                    reverse(u.begin(), u.end());
                    bool ok = t == u;
                    assert(m.is_pal(l, r) == ok);
                    if (ok)
                    {
                        best = max(best, r - l + 1);
                    }
                }
            }
            assert(m.longest() == best);
        }
    }
    cout << "PASS manacher\n";
}
