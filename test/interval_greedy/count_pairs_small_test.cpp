#include "code/interval_greedy/count_pairs_small.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(count_pairs_small(3, vector<pll>{{}, {1, 2}}) == 2);
    {
        for (int n = 1; n <= 15; n++)
        {
            for (int t = 0; t < 50; t++)
            {
                vector<pll> s(1);
                for (int j = 0; j < 10; j++)
                {
                    int l = gen() % n + 1, r = gen() % n + 1;
                    if (l > r)
                    {
                        swap(l, r);
                    }
                    s.push_back({l, r});
                }
                ll z = 0;
                for (int l = 1; l <= n; l++)
                {
                    for (int r = l; r <= n; r++)
                    {
                        bool ok = false;
                        for (int j = 1; j < (int)s.size(); j++)
                        {
                            if (l <= s[j].first && s[j].second <= r)
                            {
                                ok = true;
                            }
                        }
                        z += ok;
                    }
                }
                assert(count_pairs_small(n, s) == z);
            }
        }
    }
    cout << "PASS count_pairs_small\n";
}
