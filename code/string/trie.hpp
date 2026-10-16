#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct Trie
{
    struct Node
    {
        array<int, 26> nxt{};
        int pass = 0;
        int end = 0;
    };

    vector<Node> tr;

    Trie()
    {
        init();
    }

    void init()
    {
        tr.assign(1, Node());
    }

    int new_node()
    {
        tr.push_back(Node());
        return (int)tr.size() - 1;
    }

    void insert(const string &s)
    {
        int u = 0;
        ++tr[u].pass;

        for (char ch : s)
        {
            int c = ch - 'a';
            if (!tr[u].nxt[c])
            {
                tr[u].nxt[c] = new_node();
            }

            u = tr[u].nxt[c];
            ++tr[u].pass;
        }

        ++tr[u].end;
    }

    int count_word(const string &s) const
    {
        int u = 0;

        for (char ch : s)
        {
            int c = ch - 'a';
            if (!tr[u].nxt[c])
            {
                return 0;
            }
            u = tr[u].nxt[c];
        }

        return tr[u].end;
    }

    int count_prefix(const string &s) const
    {
        int u = 0;

        for (char ch : s)
        {
            int c = ch - 'a';
            if (!tr[u].nxt[c])
            {
                return 0;
            }
            u = tr[u].nxt[c];
        }

        return tr[u].pass;
    }
};
