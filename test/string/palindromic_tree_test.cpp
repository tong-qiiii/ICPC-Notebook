#include "code/string/palindromic_tree.hpp"
int main()
{
    mt19937 gen(20261016);
    PAM p;
    p.build("ababa");
    p.calc_occ();
    assert(p.distinct() == 5 && p.longest_suffix() == 5 &&
           p.suffix_count() == 3);
    {
        PAM p;
        for (int run = 0; run < 500; run++)
        {
            int n = gen() % 40;
            string s;
            for (int i = 0; i < n; i++)
            {
                s += char('a' + gen() % 4);
            }
            p.init();
            map<string, int> ref;
            for (int k = 0; k < n; k++)
            {
                p.insert(s[k]);
                p.calc_occ();
                int best = 0, cnt = 0;
                for (int l = 0; l <= k; l++)
                {
                    string u = s.substr(l, k - l + 1), v = u;
                    reverse(v.begin(), v.end());
                    if (u == v)
                    {
                        ref[u]++;
                        cnt++;
                        best = max(best, k - l + 1);
                    }
                }
                assert(p.distinct() == (int)ref.size() &&
                       p.longest_suffix() == best &&
                       p.suffix_count() == cnt);
            }
            p.calc_occ();
            p.calc_occ();
            multiset<pair<int, ll>> actual, want;
            for (int u = 2; u < (int)p.tr.size(); u++)
            {
                actual.insert({p.tr[u].len, p.tr[u].occ});
            }
            for (auto [s, c] : ref)
            {
                want.insert({s.size(), c});
            }
            assert(actual == want);
        }
    }
    cout << "PASS palindromic_tree\n";
}
