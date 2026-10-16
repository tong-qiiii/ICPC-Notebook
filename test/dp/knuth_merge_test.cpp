#include "code/dp/knuth_merge.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(knuth_merge(vector<ll>{0, 1, 2, 3}) == 9);
    {
        auto slow = [&](auto &&self, vector<ll> v) -> ll
        {
            if (v.size() < 2)
            {
                return 0;
            }
            ll best = LLONG_MAX;
            for (int i = 0; i + 1 < (int)v.size(); i++)
            {
                auto b = v;
                ll cost = b[i] + b[i + 1];
                b[i] = cost;
                b.erase(b.begin() + i + 1);
                best = min(best, cost + self(self, b));
            }
            return best;
        };
        for (int t = 0; t < 200; t++)
        {
            vector<ll> a(1), b;
            for (int i = gen() % 7; i; i--)
            {
                ll v = gen() % 15;
                a.push_back(v);
                b.push_back(v);
            }
            assert(knuth_merge(a) == slow(slow, b));
        }
    }
    cout << "PASS knuth_merge\n";
}
