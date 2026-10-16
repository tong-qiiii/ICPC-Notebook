#include "code/data_structure/fhq_sequence.hpp"
int main()
{
    mt19937 gen(20261016);
    FHQSeq s;
    s.build(vector<int>{1, 2, 3});
    s.reverse(1, 3);
    assert((s.to_vector() == vector<int>{3, 2, 1}));
    {
        FHQSeq s;
        for (int run = 0; run < 20; run++)
        {
            s.rng.seed(run);
            vector<int> a;
            s.build(a);
            for (int j = 0; j < 300; j++)
            {
                if (a.empty() || gen() % 3 == 0)
                {
                    int p = gen() % (a.size() + 1);
                    s.insert(p + 1, j);
                    a.insert(a.begin() + p, j);
                }
                else
                {
                    int l = gen() % a.size(), r = gen() % a.size();
                    if (l > r)
                    {
                        swap(l, r);
                    }
                    if (gen() % 2)
                    {
                        s.reverse(l + 1, r + 1);
                        reverse(a.begin() + l, a.begin() + r + 1);
                    }
                    else
                    {
                        s.erase(l + 1, r + 1);
                        a.erase(a.begin() + l, a.begin() + r + 1);
                    }
                }
                assert(s.to_vector() == a);
            }
        }
    }
    cout << "PASS fhq_sequence\n";
}
