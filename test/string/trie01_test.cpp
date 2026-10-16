#include "code/string/trie01.hpp"
int main()
{
    mt19937 gen(20261016);
    Trie01 t;
    t.insert(5);
    t.insert(10);
    assert(t.max_xor(7) == 13 && t.min_xor(7) == 2);
    t.erase(5);
    {
        Trie01 t;
        for (int run = 0; run < 20; run++)
        {
            t.init();
            multiset<int> s;
            for (int j = 0; j < 500; j++)
            {
                int v = gen() % 1024;
                auto it = s.find(v);
                if (it != s.end() && gen() % 2)
                {
                    s.erase(it);
                    t.erase(v);
                }
                else
                {
                    s.insert(v);
                    t.insert(v);
                }
                assert(t.size() == (int)s.size());
                if (!s.empty())
                {
                    int mn = INT_MAX, mx = 0;
                    for (int x : s)
                    {
                        mn = min(mn, v ^ x);
                        mx = max(mx, v ^ x);
                    }
                    assert(t.min_xor(v) == mn && t.max_xor(v) == mx);
                }
            }
        }
        t.init();
        t.insert(1 << 30);
        assert(t.max_xor(0) == 1 << 30);
    }
    cout << "PASS trie01\n";
}
