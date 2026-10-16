#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct Manacher
{
    vector<int> d1, d2;

    Manacher() = default;

    Manacher(const string &s)
    {
        build(s);
    }

    void build(const string &s)
    {
        int n = (int)s.size();
        d1.assign(n, 0);
        d2.assign(n, 0);

        for (int i = 0, l = 0, r = -1; i < n; ++i)
        {
            int k = (i > r) ? 1 : min(d1[l + r - i], r - i + 1);

            while (i - k >= 0 && i + k < n && s[i - k] == s[i + k])
            {
                ++k;
            }

            d1[i] = k;

            if (i + k - 1 > r)
            {
                l = i - k + 1;
                r = i + k - 1;
            }
        }

        for (int i = 0, l = 0, r = -1; i < n; ++i)
        {
            int k = (i > r) ? 0 : min(d2[l + r - i + 1], r - i + 1);

            while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k])
            {
                ++k;
            }

            d2[i] = k;

            if (i + k - 1 > r)
            {
                l = i - k;
                r = i + k - 1;
            }
        }
    }

    int longest() const
    {
        int ans = 0;

        for (int x : d1)
        {
            ans = max(ans, 2 * x - 1);
        }

        for (int x : d2)
        {
            ans = max(ans, 2 * x);
        }

        return ans;
    }

    bool is_pal(int l, int r) const
    {
        int len = r - l + 1;
        int mid = (l + r) >> 1;

        if (len & 1)
        {
            return d1[mid] >= len / 2 + 1;
        }

        return d2[mid + 1] >= len / 2;
    }
};
