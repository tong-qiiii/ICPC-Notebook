#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct PSTMex
{
    struct Node
    {
        int lc = 0;
        int rc = 0;
        int mn = 0;
    };

    int n = 0;
    vector<Node> tr;
    vector<int> root;

    PSTMex()
    {
        init();
    }

    void init()
    {
        n = 0;
        tr.assign(1, {});
        root.clear();
    }

    int clone(int p)
    {
        tr.push_back(tr[p]);
        return (int)tr.size() - 1;
    }

    int modify(int pre, int l, int r, int pos, int v)
    {
        int cur = clone(pre);

        if (l == r)
        {
            tr[cur].mn = v;
            return cur;
        }

        int mid = (l + r) >> 1;

        if (pos <= mid)
        {
            tr[cur].lc = modify(tr[pre].lc, l, mid, pos, v);
        }
        else
        {
            tr[cur].rc = modify(tr[pre].rc, mid + 1, r, pos, v);
        }

        tr[cur].mn = min(tr[tr[cur].lc].mn, tr[tr[cur].rc].mn);

        return cur;
    }

    int mex(int p, int l, int r, int L) const
    {
        if (l == r)
        {
            return l;
        }

        int mid = (l + r) >> 1;

        if (tr[tr[p].lc].mn < L)
        {
            return mex(tr[p].lc, l, mid, L);
        }

        return mex(tr[p].rc, mid + 1, r, L);
    }

    void build(const vector<int> &a)
    {
        init();

        n = (int)a.size() - 1;
        root.assign(n + 1, 0);

        for (int i = 1; i <= n; i++)
        {
            root[i] = root[i - 1];

            if (0 <= a[i] && a[i] <= n)
            {
                root[i] = modify(root[i - 1], 0, n, a[i], i);
            }
        }
    }

    int query(int l, int r) const
    {
        return mex(root[r], 0, n, l);
    }
};
