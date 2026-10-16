#include "code/string/suffix_automaton.hpp"
int main()
{
    mt19937 gen(20261016);
    SAM s;
    s.build("ababa");
    s.calc_occ();
    assert(s.distinct() == 9 && s.count("aba") == 2 &&
           s.lcs_substr("cabab") == 4);
    {
        SAM sam;
        for (int run = 0; run < 500; run++)
        {
            int n = gen() % 40;
            string s;
            for (int i = 0; i < n; i++)
            {
                s += char('a' + gen() % 4);
            }
            sam.init();
            map<string, int> ref;
            for (int i = 0; i < n; i++)
            {
                sam.extend(s[i]);
                sam.calc_occ();
            }
            for (int l = 0; l < n; l++)
            {
                for (int len = 1; l + len <= n; len++)
                {
                    ref[s.substr(l, len)]++;
                }
            }
            sam.calc_occ();
            sam.calc_occ();
            assert(sam.distinct() == (ll)ref.size());
            for (auto [t, c] : ref)
            {
                assert(sam.contains(t) && sam.count(t) == c);
            }
            assert(sam.count("") == n + 1);
            string other;
            for (int i = 0; i < 35; i++)
            {
                other += char('a' + gen() % 5);
            }
            int best = 0;
            for (int l = 0; l < (int)other.size(); l++)
            {
                for (int len = 1; l + len <= (int)other.size(); len++)
                {
                    if (ref.count(other.substr(l, len)))
                    {
                        best = max(best, len);
                    }
                }
            }
            assert(sam.lcs_substr(other) == best);
        }
    }
    cout << "PASS suffix_automaton\n";
}
