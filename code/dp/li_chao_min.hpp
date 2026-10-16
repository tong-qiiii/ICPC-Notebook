#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include <bits/stdc++.h>
using namespace std;

using i128 = __int128_t;

struct LiChaoMin
{
    static constexpr ll INF = LLONG_MAX;

    struct Line
    {
        ll k = 0, b = 0;

        i128 eval(ll x) const
        {
            return (i128)k * x + b;
        }
    };

    struct Node
    {
        Line line;
        int ls = 0, rs = 0;
        bool has = false;
    };

    ll L, R;
    vector<Node> tr;

    LiChaoMin(ll L_, ll R_) : L(L_), R(R_)
    {
        assert(L <= R);
        tr.resize(2);
    }

    int newNode()
    {
        tr.push_back({});
        return (int)tr.size() - 1;
    }

    void add(Line nw, int p, ll l, ll r)
    {
        if (!tr[p].has)
        {
            tr[p].line = nw;
            tr[p].has = true;
            return;
        }

        if (tr[p].line.k == nw.k)
        {
            if (nw.b < tr[p].line.b)
            {
                tr[p].line = nw;
            }
            return;
        }

        ll mid = l + (ll)(((i128)r - l) / 2);
        bool lef = nw.eval(l) < tr[p].line.eval(l);
        bool midBetter = nw.eval(mid) < tr[p].line.eval(mid);

        if (midBetter)
        {
            swap(nw, tr[p].line);
        }
        if (l == r)
        {
            return;
        }

        if (lef != midBetter)
        {
            if (!tr[p].ls)
            {
                tr[p].ls = newNode();
            }
            add(nw, tr[p].ls, l, mid);
        }
        else
        {
            if (!tr[p].rs)
            {
                tr[p].rs = newNode();
            }
            add(nw, tr[p].rs, mid + 1, r);
        }
    }

    void add(ll k, ll b)
    {
        add({k, b}, 1, L, R);
    }

    i128 query128(ll x, int p, ll l, ll r) const
    {
        const i128 IINF = (i128)1 << 120;

        i128 res = tr[p].has ? tr[p].line.eval(x) : IINF;
        if (l == r)
        {
            return res;
        }

        ll mid = l + (ll)(((i128)r - l) / 2);

        if (x <= mid && tr[p].ls)
        {
            res = min(res, query128(x, tr[p].ls, l, mid));
        }
        if (x > mid && tr[p].rs)
        {
            res = min(res, query128(x, tr[p].rs, mid + 1, r));
        }

        return res;
    }

    ll query(ll x) const
    {
        i128 res = query128(x, 1, L, R);

        if (res == ((i128)1 << 120))
        {
            return INF;
        }
        assert(res >= LLONG_MIN && res <= LLONG_MAX);
        return (ll)res;
    }
};
