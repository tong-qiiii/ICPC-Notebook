#include "code/string/duval.hpp"
int main()
{
    mt19937 gen(20261016);
    auto d = duval("ababb");
    assert((d == vector<pair<int, int>>{{0, 5}}));
    {
        for (int run = 0; run < 500; run++)
        {
            int n = gen() % 40;
            string s;
            for (int i = 0; i < n; i++)
            {
                s += char('a' + gen() % 4);
            }
            auto d = duval(s);
            string joined, last;
            int pos = 0;
            for (auto [l, r] : d)
            {
                assert(l == pos && l < r && r <= n);
                pos = r;
                string t = s.substr(l, r - l);
                joined += t;
                if (!last.empty())
                {
                    assert(last >= t);
                }
                last = t;
                for (int k = 1; k < (int)t.size(); k++)
                {
                    assert(t < t.substr(k) + t.substr(0, k));
                }
            }
            assert(joined == s && pos == n);
        }
    }
    cout << "PASS duval\n";
}
