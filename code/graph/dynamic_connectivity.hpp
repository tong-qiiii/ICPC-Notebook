#pragma once
#include <bits/stdc++.h>
using namespace std;
#include "rollback_dsu.hpp"

struct DynCon
{
    int n;
    vector<array<int, 3>> op;

    DynCon(int n) : n(n)
    {
    }

    // type: 1 加边，-1 删去一份边，0 查询连通性。
    void add(int type, int u, int v)
    {
        assert(type >= -1 && type <= 1);
        assert(u >= 1 && u <= n && v >= 1 && v <= n);
        if (u > v) swap(u, v);
        op.push_back({type, u, v});
    }

    vector<int> work() const
    {
        int q = op.size();
        if (!q) return {};
        vector<vector<pair<int, int>>> seg(4 * q);
        auto put = [&](auto &&self, int p, int l, int r,
                       int ql, int qr, pair<int, int> e) -> void
        {
            if (ql > qr) return;
            if (ql <= l && r <= qr)
            {
                seg[p].push_back(e);
                return;
            }
            int mid = (l + r) / 2;
            if (ql <= mid) self(self, p * 2, l, mid, ql, qr, e);
            if (qr > mid) self(self, p * 2 + 1, mid + 1, r, ql, qr, e);
        };
        map<pair<int, int>, pair<int, int>> active;
        for (int i = 0; i < q; i++)
        {
            auto [type, u, v] = op[i];
            if (!type) continue;
            auto &s = active[{u, v}];
            if (type == 1)
            {
                if (!s.first++) s.second = i;
            }
            else
            {
                assert(s.first > 0);
                if (!--s.first) put(put, 1, 0, q - 1, s.second, i - 1, {u, v});
            }
        }
        for (auto [e, s] : active)
        {
            if (s.first) put(put, 1, 0, q - 1, s.second, q - 1, e);
        }
        RollbackDSU d(n);
        vector<int> ans;
        auto dfs = [&](auto &&self, int p, int l, int r) -> void
        {
            int snap = d.snapshot();
            for (auto [u, v] : seg[p]) d.merge(u, v);
            if (l == r)
            {
                auto [type, u, v] = op[l];
                if (!type) ans.push_back(d.same(u, v));
            }
            else
            {
                int mid = (l + r) / 2;
                self(self, p * 2, l, mid);
                self(self, p * 2 + 1, mid + 1, r);
            }
            d.rollback(snap);
        };
        dfs(dfs, 1, 0, q - 1);
        return ans;
    }
};
