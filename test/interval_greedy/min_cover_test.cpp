#include "code/interval_greedy/min_cover.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(min_cover(vector<pll>{{}, {1, 3}, {3, 5}}, 1, 5) == 2);
    assert(min_cover(vector<pll>{{}, {1, 2}, {3, 5}}, 1, 5) == -1);
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
            ll L = gen() % 8, R = gen() % 8;
            if (L > R)
            {
                swap(L, R);
            }
            int best = 99;
            for (int mask = 0; mask < (1 << n); mask++)
            {
                vector<pll> b;
                for (int i = 0; i < n; i++)
                {
                    if (mask >> i & 1)
                    {
                        b.push_back(s[i + 1]);
                    }
                }
                sort(b.begin(), b.end());
                ll cur = L;
                bool hit = false;
                for (auto [l, r] : b)
                {
                    if (l <= L && L <= r)
                    {
                        hit = true;
                    }
                    if (l <= cur)
                    {
                        cur = max(cur, r);
                    }
                }
                if (hit && cur >= R)
                {
                    best = min(best, __builtin_popcount((unsigned)mask));
                }
            }
            assert(min_cover(s, L, R) == (best == 99 ? -1 : best));
        }
    }
    cout << "PASS min_cover\n";
}
