#include "code/interval_greedy/interval_group.hpp"
int main()
{
    mt19937 gen(20261016);
    auto [cnt, bel] = interval_group(vector<pll>{{}, {1, 2}, {2, 3}});
    assert(cnt == 2 && bel[1] != bel[2]);
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
            auto [cnt, bel] = interval_group(s);
            int peak = 0;
            for (int x = 0; x < 8; x++)
            {
                int c = 0;
                for (int i = 1; i <= n; i++)
                {
                    c += s[i].first <= x && x <= s[i].second;
                }
                peak = max(peak, c);
            }
            assert(cnt == peak);
            for (int i = 1; i <= n; i++)
            {
                for (int j = i + 1; j <= n; j++)
                {
                    if (bel[i] == bel[j])
                    {
                        assert(max(s[i].first, s[j].first) >
                               min(s[i].second, s[j].second));
                    }
                }
            }
        }
    }
    cout << "PASS interval_group\n";
}
