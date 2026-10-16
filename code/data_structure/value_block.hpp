#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct ValueBlock
{
    int n = 0;
    int len = 0;
    int num = 0;
    int tot = 0;

    vector<int> freq;
    vector<int> sum;
    vector<int> bel;
    vector<int> L, R;

    ValueBlock() = default;

    ValueBlock(int n)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        len = max(1, (int)sqrt(n));
        num = (n + len - 1) / len;
        tot = 0;

        freq.assign(n + 1, 0);
        sum.assign(num + 1, 0);
        bel.assign(n + 1, 0);
        L.assign(num + 1, 0);
        R.assign(num + 1, 0);

        for (int b = 1; b <= num; ++b)
        {
            L[b] = (b - 1) * len + 1;
            R[b] = min(n, b * len);

            for (int i = L[b]; i <= R[b]; ++i)
            {
                bel[i] = b;
            }
        }
    }

    void add(int x, int v)
    {
        freq[x] += v;
        sum[bel[x]] += v;
        tot += v;
    }

    int count_le(int x) const
    {
        if (x <= 0)
        {
            return 0;
        }

        if (x >= n)
        {
            return tot;
        }

        int b = bel[x];
        int res = 0;

        for (int i = 1; i < b; ++i)
        {
            res += sum[i];
        }

        for (int i = L[b]; i <= x; ++i)
        {
            res += freq[i];
        }

        return res;
    }

    int count_lt(int x) const
    {
        return x <= 1 ? 0 : count_le(x - 1);
    }

    int kth(int k) const
    {
        if (k < 1 || k > tot)
        {
            return -1;
        }

        for (int b = 1; b <= num; ++b)
        {
            if (k > sum[b])
            {
                k -= sum[b];
                continue;
            }

            for (int x = L[b]; x <= R[b]; ++x)
            {
                if (k > freq[x])
                {
                    k -= freq[x];
                }
                else
                {
                    return x;
                }
            }
        }

        return -1;
    }

    int prev(int x) const
    {
        if (!n || x <= 1)
        {
            return -1;
        }

        int p = min(n, x - 1);
        int b = bel[p];

        for (int i = p; i >= L[b]; --i)
        {
            if (freq[i])
            {
                return i;
            }
        }

        for (--b; b >= 1; --b)
        {
            if (!sum[b])
            {
                continue;
            }

            for (int i = R[b]; i >= L[b]; --i)
            {
                if (freq[i])
                {
                    return i;
                }
            }
        }

        return -1;
    }

    int next(int x) const
    {
        if (x >= n)
        {
            return -1;
        }

        int p = max(1, x + 1);
        int b = bel[p];

        for (int i = p; i <= R[b]; ++i)
        {
            if (freq[i])
            {
                return i;
            }
        }

        for (++b; b <= num; ++b)
        {
            if (!sum[b])
            {
                continue;
            }

            for (int i = L[b]; i <= R[b]; ++i)
            {
                if (freq[i])
                {
                    return i;
                }
            }
        }

        return -1;
    }
};
