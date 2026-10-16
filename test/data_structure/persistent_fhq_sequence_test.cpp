#include "code/data_structure/persistent_fhq_sequence.hpp"
int main()
{
    mt19937 gen(20261016);
    PersistentFHQSeq s;
    s.build(0, vector<int>{1, 2, 3});
    s.reverse(0, 1, 1, 3);
    assert(s.kth(0, 1) == 1 && s.kth(1, 1) == 3);
    {
        PersistentFHQSeq s;
        for (int run = 0; run < 10; run++)
        {
            s.init();
            s.rng.seed(run);
            vector<vector<int>> all(1);
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
                }
            }
        }
    }
    cout << "PASS persistent_fhq_sequence\n";
}
