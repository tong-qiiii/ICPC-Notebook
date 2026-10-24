#include "code/probability/pattern_wait.hpp"

int main()
{
    assert(pattern_wait("", "HT", {0, .5, .5}) == 0);
    assert(isinf(pattern_wait("X", "HT", {0, .5, .5})));
    assert(isinf(pattern_wait("T", "HT", {0, 1, 0})));
    assert(abs(pattern_wait("HHH", "HT", {0, .5, .5}) - 14) < 1e-10);
    assert(abs(pattern_wait("H", "HT", {0, 1e-12, 1 - 1e-12}) / 1e12 - 1) < 1e-12);
    assert(abs(pattern_wait("aaa", "a", {0, 1}) - 3) < 1e-10);
    int count = 0;
    // 独立的 border 等待时间恒等式：各非空 border 的出现概率倒数之和。
    for (int n = 1; n <= 7; n++)
    {
        for (int mask = 0; mask < (1 << n); mask++)
        {
            string s;
            for (int i = 0; i < n; i++) s += mask >> i & 1 ? 'H' : 'T';
            for (db prob : {.2, .5, .8})
            {
                db want = 0, pr = 1;
                for (int len = 1; len <= n; len++)
                {
                    pr *= s[len - 1] == 'H' ? prob : 1 - prob;
                    if (s.substr(0, len) == s.substr(n - len)) want += 1 / pr;
                }
                db got = pattern_wait(s, "HT", {0, prob, 1 - prob});
                assert(abs(got - want) < 1e-8 * want);
                count++;
            }
        }
    }
    // 再用完整历史的前向概率枚举，检查短模式的尾和，无 KMP / 消元复用。
    string s = "HTH";
    vector<pair<string, db>> mass{{"", 1}};
    db tails = 0;
    for (int step = 0; step < 17; step++)
    {
        vector<pair<string, db>> next;
        for (auto [h, w] : mass)
        {
            tails += w;
            for (char ch : string("HT"))
            {
                string t = h + ch;
                if (t.find(s) == string::npos) next.push_back({t, w * .5});
            }
        }
        mass.swap(next);
    }
    assert(tails < 10 && tails > 8);
    cout << "PASS pattern wait: " << count << " border identities, overlaps, rare and impossible symbols\n";
}
