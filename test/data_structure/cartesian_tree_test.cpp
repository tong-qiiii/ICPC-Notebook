#include "code/data_structure/cartesian_tree.hpp"
int main()
{
    mt19937 gen(20261016);
    vector<int> ls, rs;
    vector<int> a{0, 3, 1, 2};
    int rt = cartesian_tree(a, ls, rs);
    assert(rt == 2 && ls[2] == 1 && rs[2] == 3);
    {
        for (int n = 0; n < 100; n++)
        {
            vector<int> a(n + 1), ls, rs, ord;
            for (int i = 1; i <= n; i++)
            {
                a[i] = gen() % 8;
            }
            int rt = cartesian_tree(a, ls, rs);
            auto dfs = [&](auto &&self, int u) -> void
            {
                if (!u)
                {
                    return;
                }
                if (ls[u])
                {
                    assert(a[u] <= a[ls[u]]);
                }
                if (rs[u])
                {
                    assert(a[u] <= a[rs[u]]);
                }
                self(self, ls[u]);
                ord.push_back(u);
                self(self, rs[u]);
            };
            dfs(dfs, rt);
            for (int i = 0; i < n; i++)
            {
                assert(ord[i] == i + 1);
            }
        }
    }
    cout << "PASS cartesian_tree\n";
}
