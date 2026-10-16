#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include <bits/stdc++.h>
using namespace std;

inline int minRepresentation(const string &s)
{
    int n = s.size();
    if (!n)
    {
        return 0;
    }

    int i = 0, j = 1, k = 0;

    while (i < n && j < n && k < n)
    {
        char a = s[(i + k) % n];
        char b = s[(j + k) % n];

        if (a == b)
        {
            ++k;
            continue;
        }

        if (a > b)
        {
            i += k + 1;
            if (i == j)
            {
                ++i;
            }
        }
        else
        {
            j += k + 1;
            if (i == j)
            {
                ++j;
            }
        }

        k = 0;
    }

    return min(i, j);
}
