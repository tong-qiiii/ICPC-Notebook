#include "code/interval_greedy/min_points.hpp"
int main()
{
    mt19937 gen(20261016);
    auto p = min_points(vector<pll>{{}, {1, 2}, {2, 3}});
    assert(p.size() == 2 && p[1] == 2);
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
            int best = 9;
            for (int mask = 0; mask < 256; mask++)
            {
                bool ok = true;
                for (int j = 1; j <= n; j++)
                {
                    bool hit = false;
                    for (int x = s[j].first; x <= s[j].second; x++)
                    {
                        if (mask >> x & 1)
                        {
                            hit = true;
                        }
                    }
                    ok &= hit;
                }
                if (ok)
                {
                    best = min(best, __builtin_popcount((unsigned)mask));
                }
            }
            auto p = min_points(s);
            assert((int)p.size() - 1 == best);
            for (int j = 1; j <= n; j++)
            {
                bool hit = false;
                for (int k = 1; k < (int)p.size(); k++)
                {
                    if (s[j].first <= p[k] && p[k] <= s[j].second)
                    {
                        hit = true;
                    }
                }
                assert(hit);
            }
        }
    }
    cout << "PASS min_points\n";
}
