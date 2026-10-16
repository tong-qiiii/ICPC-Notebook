#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct SegMerge
{

    struct Node
    {
        int lc = 0;
        int rc = 0;
        ll mx = 0;
        int pos = 0;
    };

    int V = 0;
    std::vector<Node> tr;
    std::vector<int> root;

    SegMerge() = default;

    SegMerge(int n, int V)
    {
        init(n, V);
    }

    void init(int n, int V_)
    {
        V = V_;
        tr.assign(1, Node{});
        root.assign(n + 1, 0);
    }

    void add(int id, int pos, ll delta)
    {
        root[id] = add_impl(root[id], 1, V, pos, delta);
    }

    int merge(int x, int y)
    {
        return merge_impl(x, y, 1, V);
    }

  private:
    int new_node()
    {
        tr.push_back(Node{});
        return (int)tr.size() - 1;
    }

    void pull(int p)
    {
        int l = tr[p].lc;
        int r = tr[p].rc;

        if (!l)
        {
            tr[p].mx = tr[r].mx;
            tr[p].pos = tr[r].pos;
        }
        else if (!r)
        {
            tr[p].mx = tr[l].mx;
            tr[p].pos = tr[l].pos;
        }
        else if (tr[l].mx >= tr[r].mx)
        {
            tr[p].mx = tr[l].mx;
            tr[p].pos = tr[l].pos;
        }
        else
        {
            tr[p].mx = tr[r].mx;
            tr[p].pos = tr[r].pos;
        }
    }

    int add_impl(int p, int l, int r, int pos, ll delta)
    {
        if (!p)
        {
            p = new_node();
        }

        if (l == r)
        {
            tr[p].mx += delta;
            tr[p].pos = tr[p].mx ? l : 0;
            return p;
        }

        int mid = l + (r - l) / 2;

        if (pos <= mid)
        {
            tr[p].lc = add_impl(tr[p].lc, l, mid, pos, delta);
        }
        else
        {
            tr[p].rc = add_impl(tr[p].rc, mid + 1, r, pos, delta);
        }

        pull(p);
        return p;
    }

    int merge_impl(int x, int y, int l, int r)
    {
        if (!x || !y)
        {
            return x | y;
        }

        if (l == r)
        {
            tr[x].mx += tr[y].mx;
            tr[x].pos = tr[x].mx ? l : 0;
            return x;
        }

        int mid = l + (r - l) / 2;

        tr[x].lc = merge_impl(tr[x].lc, tr[y].lc, l, mid);
        tr[x].rc = merge_impl(tr[x].rc, tr[y].rc, mid + 1, r);

        pull(x);
        return x;
    }
};
