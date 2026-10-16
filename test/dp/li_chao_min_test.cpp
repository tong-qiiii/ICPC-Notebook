#include "code/dp/li_chao_min.hpp"
int main()
{
    mt19937 gen(20261016);
    LiChaoMin t(-10, 10);
    t.add(2, 1);
    t.add(-1, 3);
    assert(t.query(4) == -1);
    {
        for (int t = 0; t < 20; t++)
        {
            LiChaoMin c(-40, 40);
            vector<pair<ll, ll>> ls;
            for (int j = 0; j < 150; j++)
            {
                ll k = int(gen() % 101) - 50, b = int(gen() % 101) - 50;
                c.add(k, b);
                ls.push_back({k, b});
                for (int x = -40; x <= 40; x++)
                {
                    ll ref = LLONG_MAX;
                    for (auto [k, b] : ls)
                    {
                        ref = min(ref, k * x + b);
                    }
                    assert(c.query(x) == ref);
                }
            }
        }
        LiChaoMin edge(LLONG_MIN, LLONG_MAX);
        edge.add(0, LLONG_MAX - 1);
        assert(edge.query(LLONG_MAX) == LLONG_MAX - 1);
        edge.add(0, LLONG_MIN + 1);
        assert(edge.query(LLONG_MIN) == LLONG_MIN + 1);
    }
    cout << "PASS li_chao_min\n";
}
