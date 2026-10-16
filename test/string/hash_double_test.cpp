#include "code/string/hash_double.hpp"
int main()
{
    mt19937 gen(20261016);
    Hash2 h("ababa");
    assert(h.equal(0, 2, 2, 4));
    assert(h.merge(h.get(0, 1), h.get(2, 4), 3) == h.get(0, 4));
    {
        Hash2 h;
        for (int run = 0; run < 500; run++)
        {
            int n = gen() % 40;
            string s;
            for (int i = 0; i < n; i++)
            {
                s += char('a' + gen() % 4);
            }
            h.build(s);
            for (int k = 0; k < 100 && n; k++)
            {
                int l = gen() % n, r = gen() % n;
                if (l > r)
                {
                    swap(l, r);
                }
                auto got = h.get(l, r);
                Hash2 ref(s.substr(l, r - l + 1));
                assert(got == ref.get(0, r - l));
                if (l < r)
                {
                    int mid = (l + r) / 2;
                    assert(h.merge(h.get(l, mid), h.get(mid + 1, r),
                                   r - mid) == got);
                }
                int x = gen() % n, y = gen() % n;
                if (x > y)
                {
                    swap(x, y);
                }
                assert(h.equal(l, r, x, y) ==
                       (s.substr(l, r - l + 1) == s.substr(x, y - x + 1)));
            }
        }
    }
    cout << "PASS hash_double\n";
}
