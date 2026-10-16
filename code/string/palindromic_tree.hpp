#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct PAM
{
    static constexpr int A = 26;

    struct Node
    {
        array<int, A> nxt{};
        int fail = 0;
        int len = 0;
        int suf_cnt = 0;

        // cnt: 作为最长回文后缀的次数；occ: 总出现次数
        long long cnt = 0, occ = 0;

        Node(int len = 0) : len(len)
        {
        }
    };

    vector<Node> tr;
    vector<int> s;
    int last = 0;

    PAM(int n = 0)
    {
        tr.reserve(n + 2);
        s.reserve(n + 1);
        init();
    }

    void init()
    {
        tr.clear();
        tr.emplace_back(0);
        tr.emplace_back(-1);

        tr[0].fail = 1;
        tr[1].fail = 1;

        s.clear();
        s.push_back(-1);

        last = 0;
    }

  private:
    int get_fail(int u)
    {
        int n = (int)s.size() - 1;

        while (s[n - tr[u].len - 1] != s[n])
        {
            u = tr[u].fail;
        }

        return u;
    }

  public:
    int insert(char ch)
    {
        int c = ch - 'a';
        s.push_back(c);

        int cur = get_fail(last);

        if (!tr[cur].nxt[c])
        {
            int now = (int)tr.size();
            tr.emplace_back(tr[cur].len + 2);

            if (tr[now].len == 1)
            {
                tr[now].fail = 0;
            }
            else
            {
                int f = get_fail(tr[cur].fail);
                tr[now].fail = tr[f].nxt[c];
            }

            tr[now].suf_cnt = tr[tr[now].fail].suf_cnt + 1;
            tr[cur].nxt[c] = now;
        }

        last = tr[cur].nxt[c];
        ++tr[last].cnt;

        return last;
    }

    void build(const string &str)
    {
        init();
        tr.reserve(str.size() + 2);
        s.reserve(str.size() + 1);

        for (char c : str)
        {
            insert(c);
        }
    }

    void calc_occ()
    {
        for (auto &x : tr)
        {
            x.occ = x.cnt;
        }

        // fail[u] 一定指向更早创建的节点
        for (int u = (int)tr.size() - 1; u >= 2; --u)
        {
            tr[tr[u].fail].occ += tr[u].occ;
        }
    }

    int distinct() const
    {
        return (int)tr.size() - 2;
    }

    int longest_suffix() const
    {
        return tr[last].len;
    }

    int suffix_count() const
    {
        return tr[last].suf_cnt;
    }
};
