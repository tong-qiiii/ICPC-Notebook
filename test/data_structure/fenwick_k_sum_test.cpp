#include "code/data_structure/fenwick_k_sum.hpp"
int main()
{
    mt19937 gen(20261016);
    FenwickKSum s(vector<ll>{-1, 2, 5});
    s.insert(-1);
    s.insert(5);
    assert(s.sum_k(1) == -1 && s.kth(2) == 5);
    {
        FenwickKSum s;
        vector<ll> values;
        for (int i = -20; i <= 20; i++)
        {
            values.push_back(i);
        }
        for (int run = 0; run < 10; run++)
        {
            s.init(values);
            multiset<ll> r;
            for (int j = 0; j < 400; j++)
            {
                ll v = int(gen() % 41) - 20;
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
                ll total = 0;
                int k = 0;
                assert(s.sum_k(0) == 0);
                for (ll x : r)
                {
                    total += x;
                    assert(s.kth(++k) == x);
                    assert(s.sum_k(k) == total);
                }
                assert(s.size() == (ll)r.size() && s.sum() == total);
                ll cs = 0, ct = 0;
                for (ll x : r)
                {
                    if (x <= v)
                    {
                        cs += x;
                        ct++;
                    }
                }
                assert(s.count_le(v) == ct && s.sum_le(v) == cs);
            }
        }
    }
    cout << "PASS fenwick_k_sum\n";
}
