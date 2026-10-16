#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include <bits/stdc++.h>
using namespace std;

inline vector<pair<int, int>> duval(const string &s)
{
    int n = s.size();
    vector<pair<int, int>> res;

    int i = 0;

    while (i < n)
    {
        int j = i + 1, k = i;

        while (j < n && s[k] <= s[j])
        {
            if (s[k] < s[j])
            {
                k = i;
            }
            else
            {
                ++k;
            }

            ++j;
        }

        int len = j - k;

        while (i <= k)
        {
            res.push_back({i, i + len});
            i += len;
        }
    }

    return res;
}
