#include "code/data_structure/multiset_k_sum.hpp"
int main()
{
    mt19937 gen(20261016);
    KSum s(2);
    s.insert(3);
    s.insert(1);
    s.insert(2);
    assert(s.sum() == 3 && s.kth() == 2);
    {
        KSum s;
        for (int k = 0; k <= 15; k++)
        {
            s.init(k);
            multiset<ll> r;
            for (int j = 0; j < 300; j++)
            {
                ll v = int(gen() % 31) - 15;
                auto it = r.find(v);
                if (it != r.end() && gen() % 2)
                {
                    r.erase(it);
                    s.erase(v);
                }
                else
                {
                    r.insert(v);
                    s.insert(v);
                }
                int c = 0;
                ll z = 0, last = 0;
                for (ll x : r)
                {
                    if (c++ == k)
                    {
                        break;
                    }
                    z += x;
                    last = x;
                }
                assert(s.sum() == z && s.full() == ((int)r.size() >= k));
                if (k && s.full())
                {
                    assert(s.kth() == last);
                }
            }
        }
    }
    cout << "PASS multiset_k_sum\n";
}
