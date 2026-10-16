#include "code/dp/domino_tilings.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(dominoTilings({"..", ".."}) == 2 && dominoTilings({}) == 1);
    {
        auto brute = [&](auto &&self, vector<string> a) -> ll
        {
            int n = a.size(), m = n ? a[0].size() : 0;
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    if (a[i][j] != '#')
                    {
                        ll z = 0;
                        a[i][j] = '#';
                        if (i + 1 < n && a[i + 1][j] != '#')
                        {
                            auto b = a;
                            b[i + 1][j] = '#';
                            z += self(self, b);
                        }
                        if (j + 1 < m && a[i][j + 1] != '#')
                        {
                            a[i][j + 1] = '#';
                            z += self(self, a);
                        }
                        return z;
                    }
                }
            }
            return 1;
        };
        for (int t = 0; t < 500; t++)
        {
            int n = gen() % 4 + 1, m = gen() % 4 + 1;
            vector<string> a(n, string(m, '.'));
            for (auto &r : a)
            {
                for (auto &c : r)
                {
                    if (gen() % 4 == 0)
                    {
                        c = '#';
                    }
                }
            }
            assert(dominoTilings(a) == brute(brute, a));
        }
    }
    cout << "PASS domino_tilings\n";
}
