#include "code/data_structure/line_hull.hpp"
int main()
{
    mt19937 gen(20261016);
    LineHull h;
    h.add_line(-1, 2);
    h.add_line(1, 0);
    assert(h.query(3) == 3);
    {
        for (int t = 0; t < 50; t++)
        {
            LineHull h;
            vector<pair<ll, ll>> lines;
            for (int i = -20; i <= 20; i++)
            {
                ll b = int(gen() % 101) - 50;
                h.add_line(i / 2, b);
                lines.push_back({i / 2, b});
            }
            for (int x = -80; x <= 80; x++)
            {
                ll z = LLONG_MIN;
                for (auto [k, b] : lines)
                {
                    z = max(z, k * x + b);
                }
                assert(h.query(x) == z);
                assert(h.query_mono(x) == z);
            }
            h.clear();
            assert(h.query(0) == LLONG_MIN);
        }
    }
    cout << "PASS line_hull\n";
}
