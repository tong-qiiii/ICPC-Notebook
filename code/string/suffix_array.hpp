#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include <bits/stdc++.h>
using namespace std;

inline vector<int> buildSA(const string &s)
{
    int n = s.size();
    if (!n)
    {
        return {};
    }

    int m = 256;

    vector<int> sa(n), x(n), y(n);
    vector<int> cnt(max(n, 256) + 1);

    for (int i = 0; i < n; ++i)
    {
        ++cnt[x[i] = (unsigned char)s[i]];
    }

    for (int i = 1; i < m; ++i)
    {
        cnt[i] += cnt[i - 1];
    }
    for (int i = n - 1; i >= 0; --i)
    {
        sa[--cnt[x[i]]] = i;
    }

    for (int k = 1; k < n; k <<= 1)
    {
        int p = 0;

        for (int i = n - k; i < n; ++i)
        {
            y[p++] = i;
        }
        for (int i = 0; i < n; ++i)
        {
            if (sa[i] >= k)
            {
                y[p++] = sa[i] - k;
            }
        }

        fill(cnt.begin(), cnt.begin() + m, 0);

        for (int i = 0; i < n; ++i)
        {
            ++cnt[x[y[i]]];
        }
        for (int i = 1; i < m; ++i)
        {
            cnt[i] += cnt[i - 1];
        }
        for (int i = n - 1; i >= 0; --i)
        {
            sa[--cnt[x[y[i]]]] = y[i];
        }

        swap(x, y);

        x[sa[0]] = 0;
        p = 1;

        for (int i = 1; i < n; ++i)
        {
            int a = sa[i - 1], b = sa[i];

            bool same = y[a] == y[b];
            same &=
                (a + k < n ? y[a + k] : -1) == (b + k < n ? y[b + k] : -1);

            x[b] = same ? p - 1 : p++;
        }

        if (p == n)
        {
            break;
        }
        m = p;
    }

    return sa;
}
