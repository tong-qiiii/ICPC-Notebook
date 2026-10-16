#include "code/data_structure/persistent_fhq_sum.hpp"
int main()
{
    mt19937 gen(20261016);
    PersistentFHQSum s;
    s.build(0, vector<ll>{1, 2, 3});
    s.reverse(0, 1, 1, 3);
    assert(s.kth(0, 1) == 1 && s.kth(1, 1) == 3);
    {
        PersistentFHQSum s;
        for (int run = 0; run < 10; run++)
        {
            s.init();
            s.rng.seed(run);
            vector<vector<ll>> all(1);
            for (int j = 1; j <= 200; j++)
            {
                int v = gen() % all.size(), to = gen() % 2 ? j : v;
                auto b = all[v];
                if (b.empty() || gen() % 3 == 0)
                {
                    int p = gen() % (b.size() + 1);
                    s.insert(v, to, p + 1, j);
                    b.insert(b.begin() + p, j);
                }
                else
                {
                    int l = gen() % b.size(), r = gen() % b.size();
                    if (l > r)
                    {
                        swap(l, r);
                    }
                    if (gen() % 2)
                    {
                        s.reverse(v, to, l + 1, r + 1);
                        reverse(b.begin() + l, b.begin() + r + 1);
                    }
                    else
                    {
                        s.erase(v, to, l + 1, r + 1);
                        b.erase(b.begin() + l, b.begin() + r + 1);
                    }
                }
                if (to == j)
                {
                    all.resize(j + 1);
                    all[j] = b;
                }
                else
                {
                    all[to] = b;
                }
                for (int k = 0; k < 6; k++)
                {
                    int ver = gen() % all.size();
                    assert(s.to_vector(ver) == all[ver]);
                    for (int i = 0; i < (int)all[ver].size(); i++)
                    {
                        assert(s.kth(ver, i + 1) == all[ver][i]);
                    }
                    if (!all[ver].empty())
                    {
                        int l = gen() % all[ver].size(), r = gen() % all[ver].size();
                        if (l > r)
                        {
                            swap(l, r);
                        }
                        auto before = s.tr.size();
                        assert(s.range_sum(ver, l + 1, r + 1) ==
                               accumulate(all[ver].begin() + l,
                                          all[ver].begin() + r + 1, 0LL));
                        assert(before == s.tr.size());
                    }
                }
            }
        }
    }
    cout << "PASS persistent_fhq_sum\n";
}
