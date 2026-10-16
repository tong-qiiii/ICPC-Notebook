#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct SAM
{
    static constexpr int A = 26;

    struct Node
    {
        array<int, A> nxt;
        int len = 0;
        int link = -1;

        // cnt: endpos 初始贡献；occ: calc_occ() 后的出现次数
        long long cnt = 0, occ = 0;

        Node()
        {
            nxt.fill(-1);
        }
    };

    vector<Node> st;
    int last = 0;

    SAM(int n = 0)
    {
        st.reserve(2 * n + 5);
        init();
    }

    void init()
    {
        st.clear();
        st.emplace_back();
        last = 0;
    }

    void extend(char ch)
    {
        int c = ch - 'a';

        int cur = (int)st.size();
        st.emplace_back();

        st[cur].len = st[last].len + 1;
        st[cur].cnt = 1;

        int p = last;

        while (p != -1 && st[p].nxt[c] == -1)
        {
            st[p].nxt[c] = cur;
            p = st[p].link;
        }

        if (p == -1)
        {
            st[cur].link = 0;
        }
        else
        {
            int q = st[p].nxt[c];

            if (st[p].len + 1 == st[q].len)
            {
                st[cur].link = q;
            }
            else
            {
                int clone = (int)st.size();
                st.push_back(st[q]);

                st[clone].len = st[p].len + 1;
                st[clone].cnt = 0;
                st[clone].occ = 0;

                while (p != -1 && st[p].nxt[c] == q)
                {
                    st[p].nxt[c] = clone;
                    p = st[p].link;
                }

                st[q].link = st[cur].link = clone;
            }
        }

        last = cur;
    }

    void build(const string &s)
    {
        init();
        st.reserve(2 * s.size() + 5);

        for (char c : s)
        {
            extend(c);
        }
    }

    void calc_occ()
    {
        int mx = 0;

        for (auto &x : st)
        {
            x.occ = x.cnt;
            mx = max(mx, x.len);
        }

        vector<int> cnt(mx + 1);
        vector<int> ord(st.size());

        for (auto &x : st)
        {
            ++cnt[x.len];
        }

        for (int i = 1; i <= mx; ++i)
        {
            cnt[i] += cnt[i - 1];
        }

        for (int u = (int)st.size() - 1; u >= 0; --u)
        {
            ord[--cnt[st[u].len]] = u;
        }

        for (int i = (int)ord.size() - 1; i > 0; --i)
        {
            int u = ord[i];
            st[st[u].link].occ += st[u].occ;
        }
    }

    long long distinct() const
    {
        long long ans = 0;

        for (int u = 1; u < (int)st.size(); ++u)
        {
            ans += st[u].len - st[st[u].link].len;
        }

        return ans;
    }

    bool contains(const string &t) const
    {
        int u = 0;

        for (char ch : t)
        {
            int c = ch - 'a';

            if (st[u].nxt[c] == -1)
            {
                return false;
            }

            u = st[u].nxt[c];
        }

        return true;
    }

    long long count(const string &t) const
    {
        if (t.empty())
        {
            return (long long)st[last].len + 1;
        }
        int u = 0;

        for (char ch : t)
        {
            int c = ch - 'a';

            if (st[u].nxt[c] == -1)
            {
                return 0;
            }

            u = st[u].nxt[c];
        }

        return st[u].occ;
    }

    int lcs_substr(const string &t) const
    {
        int u = 0;
        int len = 0;
        int ans = 0;

        for (char ch : t)
        {
            int c = ch - 'a';

            while (u && st[u].nxt[c] == -1)
            {
                u = st[u].link;
                len = st[u].len;
            }

            if (st[u].nxt[c] != -1)
            {
                u = st[u].nxt[c];
                ++len;
            }
            else
            {
                u = 0;
                len = 0;
            }

            ans = max(ans, len);
        }

        return ans;
    }
};
