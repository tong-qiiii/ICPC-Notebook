#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct ModDSU
{
    int k;
    vector<int> fa, siz;
    vector<ll> dis;

    ModDSU(int n = 0, int k_ = 2)
    {
        init(n, k_);
    }

    void init(int n, int k_)
    {
        assert(k_ > 0);
        k = k_;
        fa.resize(n + 1);
        siz.assign(n + 1, 1);
        dis.assign(n + 1, 0);
        iota(fa.begin(), fa.end(), 0);
    }

    ll norm(ll w) const
    {
        w %= k;
        return w < 0 ? w + k : w;
    }

    int find(int x)
    {
        if (fa[x] == x)
            return x;

        int f = fa[x];
        fa[x] = find(f);
        dis[x] = norm(dis[x] + dis[f]);

        return fa[x];
    }

    // val[y] - val[x] = w (mod k)
    bool merge(int x, int y, ll w)
    {
        w = norm(w);
        int fx = find(x);
        int fy = find(y);

        if (fx == fy)
            return norm(dis[x] - dis[y]) == w;

        if (siz[fx] > siz[fy])
        {
            swap(x, y);
            swap(fx, fy);
            w = norm(-w);
        }

        fa[fx] = fy;
        dis[fx] = norm(dis[y] - dis[x] + w);
        siz[fy] += siz[fx];

        return true;
    }

    bool same(int x, int y)
    {
        return find(x) == find(y);
    }

    ll diff(int x, int y)
    {
        find(x);
        find(y);
        return norm(dis[x] - dis[y]);
    }
};
