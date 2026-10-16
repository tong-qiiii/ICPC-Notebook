#include "code/interval_greedy/max_disjoint.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(max_disjoint(vector<pll>{{}, {1, 2}, {2, 3}, {3, 4}}) == 2);
    {
        for (int t = 0; t < 300; t++)
        {
            int n = gen() % 8;
            vector<pll> s(1);
            for (int i = 0; i < n; i++)
            {
                int l = gen() % 8, r = gen() % 8;
                if (l > r)
                {
                    swap(l, r);
                }
                s.push_back({l, r});
            }
            int best = 0;
            for (int mask = 0; mask < (1 << n); mask++)
            {
                bool ok = true;
                for (int i = 0; i < n; i++)
                {
                    for (int j = i + 1; j < n; j++)
                    {
                        if ((mask >> i & 1) && (mask >> j & 1) &&
                            max(s[i + 1].first, s[j + 1].first) <=
                                min(s[i + 1].second, s[j + 1].second))
                        {
                            ok = false;
                        }
                    }
                }
                if (ok)
                {
                    best = max(best, __builtin_popcount((unsigned)mask));
                }
            }
            assert(max_disjoint(s) == best);
        }
    }
    cout << "PASS max_disjoint\n";
}
