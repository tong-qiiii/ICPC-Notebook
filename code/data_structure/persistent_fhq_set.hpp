#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct PersistentFHQSet
{
    struct Node
    {
        int lc = 0, rc = 0;
        int val = 0, siz = 0;
        uint32_t pri = 0;
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
        tr.push_back(tr[p]);
        return (int)tr.size() - 1;
    }

    pair<int, int> split(int p, long long val)
    {
        if (!p)
        {
            return {0, 0};
        }

        int cur = clone(p);

        if (tr[cur].val <= val)
        {
            auto [x, y] = split(tr[cur].rc, val);
            tr[cur].rc = x;
            push_up(cur);
            return {cur, y};
        }
        else
        {
            auto [x, y] = split(tr[cur].lc, val);
            tr[cur].lc = y;
            push_up(cur);
            return {x, cur};
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
            tr[cur].rc = merge(tr[cur].rc, y);
            push_up(cur);
            return cur;
        }
        else
        {
            int cur = clone(y);
            tr[cur].lc = merge(x, tr[cur].lc);
            push_up(cur);
            return cur;
        }
    }

    void insert(int from, int to, int val)
    {
        int base = root[from];
        auto [x, y] = split(base, val);

        int new_root = merge(merge(x, new_node(val)), y);
        set_root(to, new_root);
    }

    void erase(int from, int to, int val)
    {
        int base = root[from];

        if (!contains_root(base, val))
        {
            set_root(to, base);
            return;
        }

        auto [x, z] = split(base, val);
        auto [a, y] = split(x, (long long)val - 1);

        y = merge(tr[y].lc, tr[y].rc);

        int new_root = merge(merge(a, y), z);
        set_root(to, new_root);
    }

    bool contains(int ver, int val) const
    {
        return contains_root(root[ver], val);
    }

    int rank_of(int ver, int val) const
    {
        int p = root[ver];
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

    int kth(int ver, int k) const
    {
        int p = root[ver];

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

    int pre(int ver, int val) const
    {
        int p = root[ver];
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

    int nxt(int ver, int val) const
    {
        int p = root[ver];
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

  private:
    void set_root(int ver, int p)
    {
        if (ver >= (int)root.size())
        {
            root.resize(ver + 1);
        }

        root[ver] = p;
    }

    bool contains_root(int p, int val) const
    {
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
};
