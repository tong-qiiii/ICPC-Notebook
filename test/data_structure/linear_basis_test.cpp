#include "code/data_structure/linear_basis.hpp"
int main()
{
    mt19937 gen(20261016);
    Basis b;
    b.insert(3);
    b.insert(5);
    b.rebuild();
    assert(b.max_xor() == 6 && b.kth(0) == 0 && b.kth(3) == 6);
    {
        for (int t = 0; t < 250; t++)
        {
            Basis b;
            set<ll> s{0};
            for (int j = 0; j < 8; j++)
            {
                ll v = gen() % 128;
                b.insert(v);
                auto old = s;
                for (ll x : old)
                {
                    s.insert(x ^ v);
                }
            }
            b.rebuild();
            b.rebuild();
            assert(b.distinct() == s.size());
            int k = 0;
            for (ll x : s)
            {
                assert(b.contains(x));
                assert(b.kth(k) == x);
                assert(b.rank(x) == k++);
            }
            assert(b.kth(k) == -1);
            assert(b.max_xor() == *s.rbegin());
            b.init();
            assert(b.distinct() == 1);
        }
        Basis b;
        b.insert(1LL << 62);
        assert(b.max_xor() == 1LL << 62);
    }
    cout << "PASS linear_basis\n";
}
