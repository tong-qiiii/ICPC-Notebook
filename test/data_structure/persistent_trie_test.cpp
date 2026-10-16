#include "code/data_structure/persistent_trie.hpp"
int main()
{
    mt19937 gen(20261016);
    PTrie01 p(0, 5);
    p.build(vector<unsigned long long>{0, 1, 4, 7});
    assert(p.range_max_xor(1, 3, 2) == 6 && p.range_min_xor(1, 3, 2) == 3);
    {
        for (int n = 1; n <= 70; n++)
        {
            PTrie01 p(0, 7);
            vector<unsigned long long> a(n + 1);
            for (int i = 1; i <= n; i++)
            {
                a[i] = gen() % 256;
            }
            p.build(a);
            for (int j = 0; j < 100; j++)
            {
                int l = gen() % n + 1, r = gen() % n + 1;
                if (l > r)
                {
                    swap(l, r);
                }
                unsigned long long v = gen() % 256, mn = ULLONG_MAX, mx = 0;
                for (int i = l; i <= r; i++)
                {
                    mn = min(mn, v ^ a[i]);
                    mx = max(mx, v ^ a[i]);
                }
                assert(p.range_max_xor(l, r, v) == mx &&
                       p.range_min_xor(l, r, v) == mn);
            }
            p.insert(n, n + 1, 3);
            auto old = p.root[n];
            p.modify(n + 1, n + 1, 3, -1);
            assert(p.root[n] == old);
            assert(p.max_xor(0, n + 1, 5) == p.max_xor(0, n, 5));
        }
    }
    cout << "PASS persistent_trie\n";
}
