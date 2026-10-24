#pragma once
#include "../string/kmp.hpp"
#include "markov_absorb.hpp"

// p[1..sigma]；s 与 alphabet 用自然的 0-index，每次独立采样一个字符。
inline db pattern_wait(const string &s, const string &alphabet, const vector<db> &p)
{
    int m = (int)s.size(), k = (int)alphabet.size();
    assert(k > 0 && (int)p.size() == k + 1);
    vector<int> id(256, -1);
    db sum = 0;
    for (int c = 0; c < k; c++)
    {
        unsigned char ch = alphabet[c];
        assert(id[ch] == -1 && isfinite(p[c + 1]) && p[c + 1] >= 0);
        id[ch] = c;
        sum += p[c + 1];
    }
    assert(abs(sum - 1) < 1e-10);
    if (m == 0) return 0;
    for (unsigned char ch : s)
    {
        if (id[ch] == -1 || p[id[ch] + 1] == 0) return numeric_limits<db>::infinity();
    }
    auto pi = prefix_function(s);
    vector<vector<int>> go(m, vector<int>(k));
    vector<vector<db>> trans(m + 2, vector<db>(m + 2));
    for (int i = 0; i < m; i++)
    {
        for (int c = 0; c < k; c++)
        {
            if (s[i] == alphabet[c]) go[i][c] = i + 1;
            else if (i) go[i][c] = go[pi[i - 1]][c];
            trans[i + 1][go[i][c] + 1] += p[c + 1];
        }
    }
    vector<int> end(m + 2);
    end[m + 1] = 1;
    return markov_absorb(trans, end).mean[1];
}
