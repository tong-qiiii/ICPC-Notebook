#include "code/data_structure/persistent_fhq_set.hpp"
int main()
{
    mt19937 gen(20261016);
    PersistentFHQSet s;
    s.insert(0, 1, 3);
    s.insert(1, 2, 1);
    s.erase(2, 2, 3);
    assert(s.kth(1, 1) == 3 && s.kth(2, 1) == 1);
    {
        PersistentFHQSet s;
        for (int run = 0; run < 10; run++)
        {
            s.init();
            s.rng.seed(run);
            vector<multiset<int>> all(1);
            for (int j = 1; j < 250; j++)
            {
                int v = gen() % all.size(), to = gen() % 2 ? j : v,
                    z = int(gen() % 31) - 15;
                auto b = all[v];
                if (gen() % 2)
                {
                    s.insert(v, to, z);
                    b.insert(z);
                }
                else
                {
                    s.erase(v, to, z);
                    auto it = b.find(z);
                    if (it != b.end())
                    {
                        b.erase(it);
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
                for (int k = 0; k < 5; k++)
                {
                    int ver = gen() % all.size(), id = 0;
                    for (int x : all[ver])
                    {
                        assert(s.kth(ver, ++id) == x);
                    }
                    assert(s.rank_of(ver, z) ==
                           distance(all[ver].begin(), all[ver].lower_bound(z)) + 1);
                }
            }
        }
    }
    cout << "PASS persistent_fhq_set\n";
}
