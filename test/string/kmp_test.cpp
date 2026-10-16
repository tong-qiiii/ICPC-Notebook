#include "code/string/kmp.hpp"
int main()
{
    mt19937 gen(20261016);
    assert((kmp_match("ababa", "aba") == vector<int>{0, 2}));
    assert(kmp_match("abc", "").empty());
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
            assert(kmp_match(s, p) == ref);
        }
        for (int run = 0; run < 500; run++)
        {
            int n = gen() % 40;
            string s;
            for (int i = 0; i < n; i++)
            {
                s += char('a' + gen() % 4);
            }
            auto pi = prefix_function(s);
            for (int i = 0; i < n; i++)
            {
                int ref = 0;
                for (int k = 1; k <= i; k++)
                {
                    if (s.substr(0, k) == s.substr(i - k + 1, k))
                    {
                        ref = k;
                    }
                }
                assert(pi[i] == ref);
            }
        }
    }
    cout << "PASS kmp\n";
}
