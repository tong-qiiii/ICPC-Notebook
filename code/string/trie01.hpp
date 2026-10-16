#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct Trie01
{
    struct Node
    {
        int son[2] = {};
        int cnt = 0;
    };

    int LOG;
    vector<Node> tr;

    Trie01(int log = 30)
    {
        init(log);
    }

    void init(int log = 30)
    {
        assert(0 <= log && log <= 30);
        LOG = log;
        tr.assign(1, Node());
    }

    int new_node()
    {
        tr.push_back(Node());
        return (int)tr.size() - 1;
    }

    int size() const
    {
        return tr[0].cnt;
    }

    void insert(int x)
    {
        int u = 0;
        ++tr[u].cnt;

        for (int i = LOG; i >= 0; --i)
        {
            int b = (x >> i) & 1;

            if (!tr[u].son[b])
            {
                tr[u].son[b] = new_node();
            }

            u = tr[u].son[b];
            ++tr[u].cnt;
        }
    }

    void erase(int x)
    {
        assert(size() > 0);

        int u = 0;
        --tr[u].cnt;

        for (int i = LOG; i >= 0; --i)
        {
            int b = (x >> i) & 1;
            assert(tr[u].son[b] && tr[tr[u].son[b]].cnt > 0);

            u = tr[u].son[b];
            --tr[u].cnt;
        }
    }

    int max_xor(int x) const
    {
        assert(size() > 0);

        int u = 0, ans = 0;

        for (int i = LOG; i >= 0; --i)
        {
            int b = (x >> i) & 1;
            int v = b ^ 1;

            if (tr[u].son[v] && tr[tr[u].son[v]].cnt > 0)
            {
                ans |= 1 << i;
                u = tr[u].son[v];
            }
            else
            {
                u = tr[u].son[b];
            }
        }

        return ans;
    }

    int min_xor(int x) const
    {
        assert(size() > 0);

        int u = 0, ans = 0;

        for (int i = LOG; i >= 0; --i)
        {
            int b = (x >> i) & 1;

            if (tr[u].son[b] && tr[tr[u].son[b]].cnt > 0)
            {
                u = tr[u].son[b];
            }
            else
            {
                ans |= 1 << i;
                u = tr[u].son[b ^ 1];
            }
        }

        return ans;
    }
};
