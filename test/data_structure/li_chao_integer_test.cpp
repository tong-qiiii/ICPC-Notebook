#include "code/data_structure/li_chao_integer.hpp"
int main()
{
    mt19937 gen(20261016);
    LiChao t(-5, 5);
    t.add_line(2, 1);
    t.add_seg(-1, 5, -2, 3);
    assert(t.query(2) == 5);
    {
        LiChao t;
        for (int run = 0; run < 10; run++)
        {
            t.init(-30, 30);
            vector<array<ll, 4>> lines;
            for (int j = 0; j < 180; j++)
            {
                ll k = int(gen() % 41) - 20, b = int(gen() % 41) - 20,
                   l = int(gen() % 61) - 30, r = int(gen() % 61) - 30;
                if (l > r)
                {
                    swap(l, r);
                }
                if (gen() % 2)
                {
                    l = -30;
                    r = 30;
                    t.add_line(k, b);
                }
                else
                {
                    t.add_seg(k, b, l, r);
                }
                lines.push_back({k, b, l, r});
                for (int x = -30; x <= 30; x++)
                {
                    ll z = LLONG_MIN;
                    for (auto line : lines)
                    {
                        if (line[2] <= x && x <= line[3])
                        {
                            z = max(z, line[0] * x + line[1]);
                        }
                    }
                    if (z != LLONG_MIN)
                    {
                        assert(t.query(x) == z);
                    }
                }
            }
        }
    }
    cout << "PASS li_chao_integer\n";
}
