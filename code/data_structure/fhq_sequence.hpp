#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct FHQSeq
{
    struct Node
    {
        int lc = 0, rc = 0;
        int val = 0, siz = 0;
        uint32_t pri = 0;
        bool rev = false;
    };

    vector<Node> tr{Node{}};
    int root = 0;

    mt19937 rng{
        (uint32_t)chrono::steady_clock::now().time_since_epoch().count()};

    int size(int p) const
    {
        return p ? tr[p].siz : 0;
    }

    void push_up(int p)
    {
        tr[p].siz = size(tr[p].lc) + size(tr[p].rc) + 1;
    }

    int new_node(int val)
    {
        Node node;
        node.val = val;
        node.siz = 1;
        node.pri = rng();

        tr.push_back(node);
        return (int)tr.size() - 1;
    }

    void apply_rev(int p)
    {
        if (!p)
        {
            return;
        }

        swap(tr[p].lc, tr[p].rc);
        tr[p].rev ^= 1;
    }

    void push_down(int p)
    {
        if (!p)
        {
            return;
        }

        if (tr[p].rev)
        {
            apply_rev(tr[p].lc);
            apply_rev(tr[p].rc);
            tr[p].rev = false;
        }
    }

    pair<int, int> split(int p, int k)
    {
        if (!p)
        {
            return {0, 0};
        }

        push_down(p);

        int ls = size(tr[p].lc);

        if (k <= ls)
        {
            auto [x, y] = split(tr[p].lc, k);
            tr[p].lc = y;
            push_up(p);
            return {x, p};
        }
        else
        {
            auto [x, y] = split(tr[p].rc, k - ls - 1);
            tr[p].rc = x;
            push_up(p);
            return {p, y};
        }
    }

    int merge(int x, int y)
    {
        if (!x || !y)
        {
            return x | y;
        }

        if (tr[x].pri < tr[y].pri)
        {
            push_down(x);
            tr[x].rc = merge(tr[x].rc, y);
            push_up(x);
            return x;
        }
        else
        {
            push_down(y);
            tr[y].lc = merge(x, tr[y].lc);
            push_up(y);
            return y;
        }
    }

    void clear()
    {
        tr.assign(1, Node{});
        root = 0;
    }

    void build(const vector<int> &a)
    {
        clear();

        for (int x : a)
        {
            root = merge(root, new_node(x));
        }
    }

    void insert(int pos, int val)
    {
        auto [x, y] = split(root, pos - 1);
        root = merge(merge(x, new_node(val)), y);
    }

    void erase(int l, int r)
    {
        auto [x, yz] = split(root, l - 1);
        auto [y, z] = split(yz, r - l + 1);

        root = merge(x, z);
    }

    void reverse(int l, int r)
    {
        auto [x, yz] = split(root, l - 1);
        auto [y, z] = split(yz, r - l + 1);

        apply_rev(y);
        root = merge(x, merge(y, z));
    }

    void inorder(int p, vector<int> &a)
    {
        if (!p)
        {
            return;
        }

        push_down(p);
        inorder(tr[p].lc, a);
        a.push_back(tr[p].val);
        inorder(tr[p].rc, a);
    }

    vector<int> to_vector()
    {
        vector<int> a;
        a.reserve(size(root));
        inorder(root, a);
        return a;
    }
};
