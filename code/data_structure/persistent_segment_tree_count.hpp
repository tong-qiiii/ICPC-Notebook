#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct PST
{
    struct Node
    {
        int lc = 0;
        int rc = 0;
        int cnt = 0;
    };

    vector<Node> tr;

    PST()
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

    int modify(int pre, int l, int r, int pos, int v)
    {
        int cur = clone(pre);
        tr[cur].cnt += v;

        if (l == r)
        {
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

        return cur;
    }

    int query(int u, int v, int l, int r, int L, int R) const
    {
        if (L <= l && r <= R)
        {
            return tr[v].cnt - tr[u].cnt;
        }

        int mid = (l + r) >> 1;
        int ans = 0;

        if (L <= mid)
        {
            ans += query(tr[u].lc, tr[v].lc, l, mid, L, R);
        }

        if (R > mid)
        {
            ans += query(tr[u].rc, tr[v].rc, mid + 1, r, L, R);
        }

        return ans;
    }

    int kth(int u, int v, int l, int r, int k) const
    {
        if (l == r)
        {
            return l;
        }

        int mid = (l + r) >> 1;
        int cnt = tr[tr[v].lc].cnt - tr[tr[u].lc].cnt;

        if (k <= cnt)
        {
            return kth(tr[u].lc, tr[v].lc, l, mid, k);
        }

        return kth(tr[u].rc, tr[v].rc, mid + 1, r, k - cnt);
    }

    int count(int u, int v) const
    {
        return tr[v].cnt - tr[u].cnt;
    }
};
