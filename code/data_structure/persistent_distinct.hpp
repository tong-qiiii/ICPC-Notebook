#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct PSTColor
{
    struct Node
    {
        int lc = 0;
        int rc = 0;
        int sum = 0;
    };

    int n = 0;
    vector<Node> tr;
    vector<int> root;

    PSTColor()
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
        tr[cur].sum += v;

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

    int query(int p, int l, int r, int L, int R) const
    {
        if (L <= l && r <= R)
        {
            return tr[p].sum;
        }

        int mid = (l + r) >> 1;
        int ans = 0;

        if (L <= mid)
        {
            ans += query(tr[p].lc, l, mid, L, R);
        }

        if (R > mid)
        {
            ans += query(tr[p].rc, mid + 1, r, L, R);
        }

        return ans;
    }

    void build(const vector<int> &a)
    {
        init();

        n = (int)a.size() - 1;
        root.assign(n + 1, 0);

        vector<int> b(a.begin() + 1, a.end());
        sort(b.begin(), b.end());
        b.erase(unique(b.begin(), b.end()), b.end());

        vector<int> last(b.size() + 1);

        for (int i = 1; i <= n; i++)
        {
            int c = lower_bound(b.begin(), b.end(), a[i]) - b.begin() + 1;

            root[i] = root[i - 1];

            if (last[c])
            {
                root[i] = modify(root[i], 1, n, last[c], -1);
            }

            root[i] = modify(root[i], 1, n, i, 1);
            last[c] = i;
        }
    }

    int query(int l, int r) const
    {
        return query(root[r], 1, n, l, r);
    }
};
