#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct SeqBlock
{
    int n = 0;
    int len = 0;
    int num = 0;

    vector<ll> a;
    vector<ll> sum;
    vector<ll> tag;

    vector<int> bel;
    vector<int> L, R;

    SeqBlock() = default;

    SeqBlock(const vector<ll> &a)
    {
        build(a);
    }

    void build(const vector<ll> &src)
    {
        a = src;
        n = (int)a.size() - 1;

        len = max(1, (int)sqrt(n));
        num = (n + len - 1) / len;

        sum.assign(num + 1, 0);
        tag.assign(num + 1, 0);

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
                sum[b] += a[i];
            }
        }
    }

    void pushdown(int b)
    {
        if (!tag[b])
        {
            return;
        }

        for (int i = L[b]; i <= R[b]; ++i)
        {
            a[i] += tag[b];
        }

        tag[b] = 0;
    }

    void rebuild(int b)
    {
        sum[b] = 0;

        for (int i = L[b]; i <= R[b]; ++i)
        {
            sum[b] += a[i];
        }
    }

    void change(int pos, ll v)
    {
        int b = bel[pos];

        pushdown(b);

        a[pos] = v;

        rebuild(b);
    }

    void modify(int l, int r, ll v)
    {
        int bl = bel[l];
        int br = bel[r];

        if (bl == br)
        {
            pushdown(bl);

            for (int i = l; i <= r; ++i)
            {
                a[i] += v;
            }

            rebuild(bl);
            return;
        }

        pushdown(bl);

        for (int i = l; i <= R[bl]; ++i)
        {
            a[i] += v;
        }

        rebuild(bl);

        for (int b = bl + 1; b < br; ++b)
        {
            tag[b] += v;
            sum[b] += 1LL * (R[b] - L[b] + 1) * v;
        }

        pushdown(br);

        for (int i = L[br]; i <= r; ++i)
        {
            a[i] += v;
        }

        rebuild(br);
    }

    ll query(int l, int r) const
    {
        int bl = bel[l];
        int br = bel[r];

        ll res = 0;

        if (bl == br)
        {
            for (int i = l; i <= r; ++i)
            {
                res += a[i] + tag[bl];
            }

            return res;
        }

        for (int i = l; i <= R[bl]; ++i)
        {
            res += a[i] + tag[bl];
        }

        for (int b = bl + 1; b < br; ++b)
        {
            res += sum[b];
        }

        for (int i = L[br]; i <= r; ++i)
        {
            res += a[i] + tag[br];
        }

        return res;
    }
};
