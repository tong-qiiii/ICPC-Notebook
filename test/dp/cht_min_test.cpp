#include "code/dp/cht_min.hpp"
int main()
{
    mt19937 gen(20261016);
    CHTMin c;
    c.add(2, 1);
    c.add(-1, 3);
    assert(c.query(4) == -1);
    {
        for (int t = 0; t < 300; t++)
        {
            CHTMin c;
            vector<pair<ll, ll>> ls;
            for (int k = 30; k >= -30; k--)
            {
                ll b = int(gen() % 101) - 50;
                c.add(k / 2, b);
                ls.push_back({k / 2, b});
            }
            for (int x = -100; x <= 100; x++)
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
    cout << "PASS cht_min\n";
}
