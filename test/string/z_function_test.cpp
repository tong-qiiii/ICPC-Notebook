#include "code/string/z_function.hpp"
int main()
{
    mt19937 gen(20261016);
    assert((z_function(string("aaaa")) == vector<int>{4, 3, 2, 1}));
    {
        for (int run = 0; run < 1000; run++)
        {
            string s, p;
            for (int i = gen() % 80; i; i--)
            {
                s += char(gen() % 256);
            }
            for (int i = gen() % 10; i; i--)
            {
                p += char(gen() % 256);
            }
            if (run % 3 == 0 && !s.empty())
            {
                int l = gen() % s.size(), len = gen() % (s.size() - l) + 1;
                p = s.substr(l, len);
            }
            vector<int> ref;
            if (!p.empty())
            {
                for (int i = 0; i + (int)p.size() <= (int)s.size(); i++)
                {
                    if (s.compare(i, p.size(), p) == 0)
                    {
                        ref.push_back(i);
                    }
                }
            }
            assert(z_match(s, p) == ref);
        }
        for (int run = 0; run < 500; run++)
        {
            int n = gen() % 40;
            string s;
            for (int i = 0; i < n; i++)
            {
                s += char('a' + gen() % 4);
            }
            auto z = z_function(s);
            for (int i = 0; i < n; i++)
            {
                int ref = 0;
                while (i + ref < n && s[ref] == s[i + ref])
                {
                    ref++;
                }
                assert(z[i] == ref);
            }
        }
    }
    cout << "PASS z_function\n";
}
