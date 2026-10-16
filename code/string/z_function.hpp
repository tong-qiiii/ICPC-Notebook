#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

template <class Seq> vector<int> z_function(const Seq &s)
{
    int n = (int)s.size();
    vector<int> z(n);

    if (n == 0)
    {
        return z;
    }

    z[0] = n;

    for (int i = 1, l = 0, r = -1; i < n; ++i)
    {
        if (i <= r)
        {
            z[i] = min(r - i + 1, z[i - l]);
        }

        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
        {
            ++z[i];
        }

        if (i + z[i] - 1 > r)
        {
            l = i;
            r = i + z[i] - 1;
        }
    }

    return z;
}

inline vector<int> z_match(const string &text, const string &pat)
{
    vector<int> pos;
    if (pat.empty())
    {
        return pos;
    }

    int m = (int)pat.size();
    vector<int> s;
    s.reserve(m + 1 + text.size());

    for (unsigned char c : pat)
    {
        s.push_back(c);
    }

    s.push_back(256);

    for (unsigned char c : text)
    {
        s.push_back(c);
    }

    vector<int> z = z_function(s);

    for (int i = m + 1; i < (int)s.size(); ++i)
    {
        if (z[i] >= m)
        {
            pos.push_back(i - m - 1);
        }
    }

    return pos;
}
