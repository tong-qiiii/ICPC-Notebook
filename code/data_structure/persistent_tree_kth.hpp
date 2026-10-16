#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct PSTTreeKth
{
    struct Node
    {
        int lc = 0;
        int rc = 0;
        int cnt = 0;
    };

    vector<Node> tr;

    PSTTreeKth()
    {
        init();
    }

    void init()
    {
        tr.assign(1, {});
    }

    int clone(int p)
    {
        tr.push_back(tr[p]);
        return (int)tr.size() - 1;
    }

    int modify(int pre, int l, int r, int pos)
    {
        int cur = clone(pre);
        tr[cur].cnt++;

        if (l == r)
        {
            return cur;
        }

        int mid = (l + r) >> 1;

        if (pos <= mid)
        {
            tr[cur].lc = modify(tr[pre].lc, l, mid, pos);
        }
        else
        {
            tr[cur].rc = modify(tr[pre].rc, mid + 1, r, pos);
        }

        return cur;
    }

    int kth(int a, int b, int c, int d, int l, int r, int k) const
    {
        if (l == r)
        {
            return l;
        }

        int mid = (l + r) >> 1;
        int cnt = tr[tr[a].lc].cnt + tr[tr[b].lc].cnt - tr[tr[c].lc].cnt -
                  tr[tr[d].lc].cnt;

        if (k <= cnt)
        {
            return kth(tr[a].lc, tr[b].lc, tr[c].lc, tr[d].lc, l, mid, k);
        }

        return kth(tr[a].rc, tr[b].rc, tr[c].rc, tr[d].rc, mid + 1, r,
                   k - cnt);
    }
};
