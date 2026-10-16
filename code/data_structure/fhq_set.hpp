#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct FHQSet
{
    struct Node
    {
        int lc = 0, rc = 0;
        int val = 0, siz = 0;
        uint32_t pri = 0;
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

    // x: <= val, y: > val
    pair<int, int> split(int p, long long val)
    {
        if (!p)
        {
            return {0, 0};
        }

        if (tr[p].val <= val)
        {
            auto [x, y] = split(tr[p].rc, val);
            tr[p].rc = x;
            push_up(p);
            return {p, y};
        }
        else
        {
            auto [x, y] = split(tr[p].lc, val);
            tr[p].lc = y;
            push_up(p);
            return {x, p};
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
            tr[x].rc = merge(tr[x].rc, y);
            push_up(x);
            return x;
        }
        else
        {
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

    bool contains(int val) const
    {
        int p = root;

        while (p)
        {
            if (tr[p].val == val)
            {
                return true;
            }

            p = val < tr[p].val ? tr[p].lc : tr[p].rc;
        }

        return false;
    }

    void insert(int val)
    {
        auto [x, y] = split(root, val);
        root = merge(merge(x, new_node(val)), y);
    }

    void erase(int val)
    {
        if (!contains(val))
        {
            return;
        }

        auto [x, z] = split(root, val);
        auto [a, y] = split(x, (long long)val - 1);

        y = merge(tr[y].lc, tr[y].rc);
        root = merge(merge(a, y), z);
    }

    int rank_of(int val) const
    {
        int p = root;
        int res = 1;

        while (p)
        {
            if (tr[p].val >= val)
            {
                p = tr[p].lc;
            }
            else
            {
                res += size(tr[p].lc) + 1;
                p = tr[p].rc;
            }
        }

        return res;
    }

    int kth(int k) const
    {
        int p = root;

        while (p)
        {
            int ls = size(tr[p].lc);

            if (k <= ls)
            {
                p = tr[p].lc;
            }
            else if (k == ls + 1)
            {
                return tr[p].val;
            }
            else
            {
                k -= ls + 1;
                p = tr[p].rc;
            }
        }

        return 0;
    }

    int pre(int val) const
    {
        int p = root;
        int res = INT_MIN; // 【按题意修改】

        while (p)
        {
            if (tr[p].val < val)
            {
                res = tr[p].val;
                p = tr[p].rc;
            }
            else
            {
                p = tr[p].lc;
            }
        }

        return res;
    }

    int nxt(int val) const
    {
        int p = root;
        int res = INT_MAX; // 【按题意修改】

        while (p)
        {
            if (tr[p].val > val)
            {
                res = tr[p].val;
                p = tr[p].lc;
            }
            else
            {
                p = tr[p].rc;
            }
        }

        return res;
    }
};
