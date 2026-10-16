#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct PersistentFHQSeq
{
    struct Node
    {
        int lc = 0, rc = 0;
        int val = 0, siz = 0;
        uint32_t pri = 0;
        bool rev = false;
    };

    vector<Node> tr{Node{}};
    vector<int> root{0};

    mt19937 rng{
        (uint32_t)chrono::steady_clock::now().time_since_epoch().count()};

    void init(int versions = 0)
    {
        tr.assign(1, Node{});
        root.assign(versions + 1, 0);
    }

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

    int clone(int p)
    {
        if (!p)
        {
            return 0;
        }

        tr.push_back(tr[p]);
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

    // p 已经是当前版本的副本
    void push_down(int p)
    {
        if (!p || !tr[p].rev)
        {
            return;
        }

        tr[p].lc = clone(tr[p].lc);
        tr[p].rc = clone(tr[p].rc);

        apply_rev(tr[p].lc);
        apply_rev(tr[p].rc);

        tr[p].rev = false;
        // 其他 lazy 同样：修改儿子前先 clone
    }

    pair<int, int> split(int p, int k)
    {
        if (!p)
        {
            return {0, 0};
        }

        int cur = clone(p);
        push_down(cur);

        int ls = size(tr[cur].lc);

        if (k <= ls)
        {
            auto [x, y] = split(tr[cur].lc, k);
            tr[cur].lc = y;
            push_up(cur);
            return {x, cur};
        }
        else
        {
            auto [x, y] = split(tr[cur].rc, k - ls - 1);
            tr[cur].rc = x;
            push_up(cur);
            return {cur, y};
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
            int cur = clone(x);
            push_down(cur);

            tr[cur].rc = merge(tr[cur].rc, y);
            push_up(cur);
            return cur;
        }
        else
        {
            int cur = clone(y);
            push_down(cur);

            tr[cur].lc = merge(x, tr[cur].lc);
            push_up(cur);
            return cur;
        }
    }

    // 仅用于初始建树，新节点之间可直接修改
    int merge_build(int x, int y)
    {
        if (!x || !y)
        {
            return x | y;
        }

        if (tr[x].pri < tr[y].pri)
        {
            tr[x].rc = merge_build(tr[x].rc, y);
            push_up(x);
            return x;
        }
        else
        {
            tr[y].lc = merge_build(x, tr[y].lc);
            push_up(y);
            return y;
        }
    }

    void build(int ver, const vector<int> &a)
    {
        int p = 0;

        for (int x : a)
        {
            p = merge_build(p, new_node(x));
        }

        set_root(ver, p);
    }

    void insert(int from, int to, int pos, int val)
    {
        auto [x, y] = split(root[from], pos - 1);

        int new_root = merge(merge(x, new_node(val)), y);
        set_root(to, new_root);
    }

    void erase(int from, int to, int l, int r)
    {
        auto [x, yz] = split(root[from], l - 1);
        auto [y, z] = split(yz, r - l + 1);

        int new_root = merge(x, z);
        set_root(to, new_root);
    }

    void reverse(int from, int to, int l, int r)
    {
        auto [x, yz] = split(root[from], l - 1);
        auto [y, z] = split(yz, r - l + 1);

        apply_rev(y);

        int new_root = merge(x, merge(y, z));
        set_root(to, new_root);
    }

    int kth(int ver, int k) const
    {
        int p = root[ver];
        bool flip = false;

        while (p)
        {
            int left = flip ? tr[p].rc : tr[p].lc;
            int right = flip ? tr[p].lc : tr[p].rc;
            int ls = size(left);

            bool next_flip = flip ^ tr[p].rev;

            if (k <= ls)
            {
                p = left;
                flip = next_flip;
            }
            else if (k == ls + 1)
            {
                return tr[p].val;
            }
            else
            {
                k -= ls + 1;
                p = right;
                flip = next_flip;
            }
        }

        return 0;
    }

    vector<int> to_vector(int ver) const
    {
        vector<int> a;
        a.reserve(size(root[ver]));

        inorder(root[ver], false, a);
        return a;
    }

  private:
    void set_root(int ver, int p)
    {
        if (ver >= (int)root.size())
        {
            root.resize(ver + 1);
        }

        root[ver] = p;
    }

    void inorder(int p, bool flip, vector<int> &a) const
    {
        if (!p)
        {
            return;
        }

        int left = flip ? tr[p].rc : tr[p].lc;
        int right = flip ? tr[p].lc : tr[p].rc;
        bool next_flip = flip ^ tr[p].rev;

        inorder(left, next_flip, a);
        a.push_back(tr[p].val);
        inorder(right, next_flip, a);
    }
};
