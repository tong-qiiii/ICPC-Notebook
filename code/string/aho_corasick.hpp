#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct AC
{
    struct Node
    {
        array<int, 26> nxt{};
        int fail = 0;
    };

    vector<Node> tr;
    vector<int> pos, order;
    bool built = false;

    AC()
    {
        init();
    }

    void init()
    {
        tr.assign(1, Node());
        pos.clear();
        order.clear();
        built = false;
    }

    int new_node()
    {
        tr.push_back(Node());
        return (int)tr.size() - 1;
    }

    int insert(const string &s)
    {
        assert(!built && !s.empty());

        int u = 0;
        for (char ch : s)
        {
            int c = ch - 'a';

            if (!tr[u].nxt[c])
            {
                tr[u].nxt[c] = new_node();
            }

            u = tr[u].nxt[c];
        }

        pos.push_back(u);
        return (int)pos.size() - 1;
    }

    void build()
    {
        if (built)
        {
            return;
        }
        built = true;

        queue<int> q;
        order.clear();

        for (int c = 0; c < 26; ++c)
        {
            int v = tr[0].nxt[c];
            if (v)
            {
                q.push(v);
            }
        }

        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            order.push_back(u);

            for (int c = 0; c < 26; ++c)
            {
                int v = tr[u].nxt[c];

                if (v)
                {
                    tr[v].fail = tr[tr[u].fail].nxt[c];
                    q.push(v);
                }
                else
                {
                    tr[u].nxt[c] = tr[tr[u].fail].nxt[c];
                }
            }
        }
    }

    vector<long long> count(const string &text) const
    {
        assert(built);

        vector<long long> cnt(tr.size());
        int u = 0;

        for (char ch : text)
        {
            if (ch < 'a' || ch > 'z')
            {
                u = 0;
                continue;
            }

            u = tr[u].nxt[ch - 'a'];
            ++cnt[u];
        }

        for (int i = (int)order.size() - 1; i >= 0; --i)
        {
            int v = order[i];
            cnt[tr[v].fail] += cnt[v];
        }

        vector<long long> ans(pos.size());
        for (int i = 0; i < (int)pos.size(); ++i)
        {
            ans[i] = cnt[pos[i]];
        }

        return ans;
    }
};
