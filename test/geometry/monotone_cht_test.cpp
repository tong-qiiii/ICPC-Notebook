#include "code/geometry/monotone_cht.hpp"
int main()
{
    mt19937 gen(20261016);
    MonotoneCHTMax h;
    h.add_line(-1, 2);
    h.add_line(1, 0);
    assert(h.query(3) == 3);
    {
        for (int t = 0; t < 100; t++)
        {
            MonotoneCHTMax h;
            vector<pair<ll, ll>> lines;
            for (int i = -30; i <= 30; i++)
            {
                ll b = int(gen() % 101) - 50;
                h.add_line(i / 2, b);
                lines.push_back({i / 2, b});
            }
            for (int x = -100; x <= 100; x++)
            {
                ll z = LLONG_MIN;
                for (auto [k, b] : lines)
                {
                    z = max(z, k * x + b);
                }
                assert(h.query(x) == z);
            }
            h.clear();
            assert(h.empty());
        }
    }
    cout << "PASS monotone_cht\n";
}
