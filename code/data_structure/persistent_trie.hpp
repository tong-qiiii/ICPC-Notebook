#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct PTrie01
{
    using U = unsigned long long;

    struct Node
    {
        int ch[2] = {};
        int cnt = 0;
    };

    int B = 30;
    std::vector<Node> tr;
    std::vector<int> root;

    PTrie01(int versions = 0, int B = 30)
    {
        init(versions, B);
    }

    void init(int versions = 0, int B_ = 30)
    {
        B = B_;
        tr.assign(1, Node{});
        root.assign(versions + 1, 0);
    }

    void build(const std::vector<U> &a)
    {
        int n = (int)a.size() - 1;
        init(n, B);

        for (int i = 1; i <= n; ++i)
        {
            insert(i - 1, i, a[i]);
        }
    }

    void modify(int from, int to, U x, int delta)
    {
        int pre_root = root[from];
        int new_root = clone(pre_root);

        int pre = pre_root;
        int cur = new_root;
        tr[cur].cnt += delta;

        for (int i = B; i >= 0; --i)
        {
            int bit = (x >> i) & 1ULL;

            int child = clone(tr[pre].ch[bit]);
            tr[cur].ch[bit] = child;

            pre = tr[pre].ch[bit];
            cur = child;

            tr[cur].cnt += delta;
        }

        if (to >= (int)root.size())
        {
            root.resize(to + 1);
        }

        root[to] = new_root;
    }

    void insert(int from, int to, U x)
    {
        modify(from, to, x, 1);
    }

    U max_xor(int left_ver, int right_ver, U x) const
    {
        int l = root[left_ver];
        int r = root[right_ver];
        U ans = 0;

        for (int i = B; i >= 0; --i)
        {
            int bit = (x >> i) & 1ULL;
            int want = bit ^ 1;

            if (count_diff(l, r, want) > 0)
            {
                ans |= 1ULL << i;
                l = tr[l].ch[want];
                r = tr[r].ch[want];
            }
            else
            {
                l = tr[l].ch[bit];
                r = tr[r].ch[bit];
            }
        }

        return ans;
    }

    U min_xor(int left_ver, int right_ver, U x) const
    {
        int l = root[left_ver];
        int r = root[right_ver];
        U ans = 0;

        for (int i = B; i >= 0; --i)
        {
            int bit = (x >> i) & 1ULL;

            if (count_diff(l, r, bit) > 0)
            {
                l = tr[l].ch[bit];
                r = tr[r].ch[bit];
            }
            else
            {
                ans |= 1ULL << i;
                int want = bit ^ 1;
                l = tr[l].ch[want];
                r = tr[r].ch[want];
            }
        }

        return ans;
    }

    U range_max_xor(int l, int r, U x) const
    {
        return max_xor(l - 1, r, x);
    }

    U range_min_xor(int l, int r, U x) const
    {
        return min_xor(l - 1, r, x);
    }

  private:
    int clone(int p)
    {
        tr.push_back(tr[p]);
        return (int)tr.size() - 1;
    }

    int count_diff(int l, int r, int bit) const
    {
        int lc = tr[l].ch[bit];
        int rc = tr[r].ch[bit];
        return tr[rc].cnt - tr[lc].cnt;
    }
};
