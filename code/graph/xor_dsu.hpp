#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct XorDSU
{
    vector<int> fa, siz;
    vector<ll> dis;

    XorDSU(int n = 0)
    {
        init(n);
    }

    void init(int n)
    {
        fa.resize(n + 1);
        siz.assign(n + 1, 1);
        dis.assign(n + 1, 0);
        iota(fa.begin(), fa.end(), 0);
    }

    int find(int x)
    {
        if (fa[x] == x)
            return x;

        int f = fa[x];
        fa[x] = find(f);
        dis[x] ^= dis[f];

        return fa[x];
    }

    // val[x] ^ val[y] = w
    bool merge(int x, int y, ll w)
    {
        int fx = find(x);
        int fy = find(y);

        if (fx == fy)
            return (dis[x] ^ dis[y]) == w;

        if (siz[fx] > siz[fy])
        {
            swap(x, y);
            swap(fx, fy);
        }

        fa[fx] = fy;
        dis[fx] = dis[x] ^ dis[y] ^ w;
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
        return dis[x] ^ dis[y];
    }
};
