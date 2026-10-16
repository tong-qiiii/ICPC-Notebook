#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline vector<int> prefix_function(const string &s)
{
    int n = (int)s.size();
    vector<int> pi(n);

    for (int i = 1; i < n; ++i)
    {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j])
        {
            j = pi[j - 1];
        }
        if (s[i] == s[j])
        {
            ++j;
        }
        pi[i] = j;
    }

    return pi;
}

inline vector<int> kmp_match(const string &text, const string &pat)
{
    vector<int> pos;
    if (pat.empty())
    {
        return pos;
    }

    vector<int> pi = prefix_function(pat);
    int m = (int)pat.size();

    for (int i = 0, j = 0; i < (int)text.size(); ++i)
    {
        while (j > 0 && text[i] != pat[j])
        {
            j = pi[j - 1];
        }

        if (text[i] == pat[j])
        {
            ++j;
        }

        if (j == m)
        {
            pos.push_back(i - m + 1);
            j = pi[j - 1];
        }
    }

    return pos;
}
