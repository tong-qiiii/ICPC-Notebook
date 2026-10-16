#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct LiChao
{

    using i128 = __int128_t;

    static constexpr ll NEG_INF = std::numeric_limits<ll>::lowest();

    struct Line
    {
        ll k = 0;
        ll b = NEG_INF;

        i128 eval(ll x) const
        {
            return (i128)k * x + b;
        }
    };

    struct Node
    {
        int lc = 0;
        int rc = 0;
        Line line;
    };

    ll L = 0, R = -1;
    std::vector<Node> tr;

    LiChao() = default;

    LiChao(ll l, ll r)
    {
        init(l, r);
    }

    void init(ll l, ll r)
    {
        L = l;
        R = r;
        tr.assign(2, Node{});
    }

    void add_line(ll k, ll b)
    {
        insert_line(1, L, R, Line{k, b});
    }

    void add_seg(ll k, ll b, ll ql, ll qr)
    {
        ql = std::max(ql, L);
        qr = std::min(qr, R);
        if (ql > qr)
        {
            return;
        }

        insert_seg(1, L, R, ql, qr, Line{k, b});
    }

    ll query(ll x) const
    {
        return (ll)query_impl(1, L, R, x);
    }

  private:
    static ll midpoint(ll l, ll r)
    {
        return l + (ll)(((i128)r - l) / 2);
    }

    int new_node()
    {
        tr.push_back(Node{});
        return (int)tr.size() - 1;
    }

    void insert_line(int p, ll l, ll r, Line line)
    {
        ll mid = midpoint(l, r);

        bool lef = line.eval(l) > tr[p].line.eval(l);
        bool mid_better = line.eval(mid) > tr[p].line.eval(mid);

        if (mid_better)
        {
            std::swap(line, tr[p].line);
        }

        if (l == r)
        {
            return;
        }

        if (lef != mid_better)
        {
            if (!tr[p].lc)
            {
                int q = new_node();
                tr[p].lc = q;
            }
            insert_line(tr[p].lc, l, mid, line);
        }
        else
        {
            if (!tr[p].rc)
            {
                int q = new_node();
                tr[p].rc = q;
            }
            insert_line(tr[p].rc, mid + 1, r, line);
        }
    }

    void insert_seg(int p, ll l, ll r, ll ql, ll qr, Line line)
    {
        if (ql <= l && r <= qr)
        {
            insert_line(p, l, r, line);
            return;
        }

        ll mid = midpoint(l, r);

        if (ql <= mid)
        {
            if (!tr[p].lc)
            {
                int q = new_node();
                tr[p].lc = q;
            }
            insert_seg(tr[p].lc, l, mid, ql, qr, line);
        }

        if (qr > mid)
        {
            if (!tr[p].rc)
            {
                int q = new_node();
                tr[p].rc = q;
            }
            insert_seg(tr[p].rc, mid + 1, r, ql, qr, line);
        }
    }

    i128 query_impl(int p, ll l, ll r, ll x) const
    {
        if (!p)
        {
            return (i128)NEG_INF;
        }

        i128 res = tr[p].line.eval(x);

        if (l == r)
        {
            return res;
        }

        ll mid = midpoint(l, r);

        if (x <= mid)
        {
            return std::max(res, query_impl(tr[p].lc, l, mid, x));
        }

        return std::max(res, query_impl(tr[p].rc, mid + 1, r, x));
    }
};
