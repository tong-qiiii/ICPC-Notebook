#include "code/string/trie.hpp"
int main()
{
    mt19937 gen(20261016);
    Trie t;
    t.insert("app");
    t.insert("apple");
    t.insert("");
    assert(t.count_word("app") == 1 && t.count_prefix("app") == 2 &&
           t.count_prefix("") == 3);
    {
        Trie t;
        for (int run = 0; run < 20; run++)
        {
            t.init();
            vector<string> all;
            for (int j = 0; j < 300; j++)
            {
                string s;
                for (int k = gen() % 10; k; k--)
                {
                    s += char('a' + gen() % 4);
                }
                if (gen() % 2)
                {
                    t.insert(s);
                    all.push_back(s);
                }
                int wc = 0, pc = 0;
                for (auto a : all)
                {
                    wc += a == s;
                    pc += a.compare(0, s.size(), s) == 0;
                }
                assert(t.count_word(s) == wc && t.count_prefix(s) == pc);
            }
        }
    }
    cout << "PASS trie\n";
}
