#include "code/data_structure/sequence_block.hpp"
int main()
{
    mt19937 gen(20261016);
    SeqBlock b(vector<ll>{0, 1, 2, 3});
    b.modify(1, 3, 2);
    b.change(2, 0);
    assert(b.query(1, 3) == 8);
    {
        SeqBlock b;
        for (int n = 1; n <= 50; n++)
        {
            vector<ll> a(n + 1);
            b.build(a);
            for (int j = 0; j < 400; j++)
            {
                int l = gen() % n + 1, r = gen() % n + 1;
                if (l > r)
                {
                    swap(l, r);
                }
                ll v = int(gen() % 31) - 15;
                if (gen() % 3 == 0)
                {
                    b.change(l, v);
                    a[l] = v;
                }
                else if (gen() % 2)
                {
                    b.modify(l, r, v);
                    for (int i = l; i <= r; i++)
                    {
                        a[i] += v;
                    }
                }
                else
                {
                    assert(b.query(l, r) ==
                           accumulate(a.begin() + l, a.begin() + r + 1, 0LL));
                }
            }
        }
    }
    cout << "PASS sequence_block\n";
}
