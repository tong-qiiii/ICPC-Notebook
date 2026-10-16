#include "code/string/aho_corasick.hpp"
int main()
{
    mt19937 gen(20261016);
    AC a;
    int x = a.insert("he"), y = a.insert("she");
    a.build();
    auto c = a.count("sheshe");
    assert(c[x] == 2 && c[y] == 2);
    {
        AC ac;
        for (int run = 0; run < 300; run++)
        {
            ac.init();
            vector<string> p;
            for (int i = 0; i < 20; i++)
            {
                string s;
                for (int k = gen() % 6 + 1; k; k--)
                {
                    s += char('a' + gen() % 4);
                }
                assert(ac.insert(s) == i);
                p.push_back(s);
            }
            ac.build();
            ac.build();
            string text;
            for (int j = 0; j < 100; j++)
            {
                text += char(gen() % 9 == 0 ? '#' : 'a' + gen() % 4);
            }
            auto got = ac.count(text);
            assert(got == ac.count(text));
            for (int i = 0; i < 20; i++)
            {
                int ref = 0;
                for (int j = 0; j + (int)p[i].size() <= (int)text.size();
                     j++)
                {
                    ref += text.compare(j, p[i].size(), p[i]) == 0;
                }
                assert(got[i] == ref);
            }
        }
    }
    cout << "PASS aho_corasick\n";
}
