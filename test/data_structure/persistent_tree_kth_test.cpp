#include "code/data_structure/persistent_tree_kth.hpp"
int main()
{
    mt19937 gen(20261016);
    PSTTreeKth p;
    int r1 = p.modify(0, 1, 5, 2), r2 = p.modify(r1, 1, 5, 4),
        r3 = p.modify(r1, 1, 5, 1);
    assert(p.kth(r2, r3, r1, 0, 1, 5, 2) == 2);
    {
        for (int n = 1; n <= 70; n++)
        {
            PSTTreeKth p;
            vector<int> par(n + 1), dep(n + 1), val(n + 1), rt(n + 1);
            for (int i = 1; i <= n; i++)
            {
                par[i] = i == 1 ? 0 : gen() % (i - 1) + 1;
                dep[i] = dep[par[i]] + 1;
                val[i] = gen() % 30 + 1;
                rt[i] = p.modify(rt[par[i]], 1, 30, val[i]);
            }
            for (int j = 0; j < 100; j++)
            {
                int u = gen() % n + 1, v = gen() % n + 1, x = u, y = v;
                vector<int> b;
                while (x != y)
                {
                    if (dep[x] < dep[y])
                    {
                        swap(x, y);
                    }
                    b.push_back(val[x]);
                    x = par[x];
                }
                b.push_back(val[x]);
                sort(b.begin(), b.end());
                for (int k = 0; k < (int)b.size(); k++)
                {
                    assert(p.kth(rt[u], rt[v], rt[x], rt[par[x]], 1, 30, k + 1) ==
                           b[k]);
                }
            }
        }
    }
    cout << "PASS persistent_tree_kth\n";
}
