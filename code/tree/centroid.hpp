#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct Centroid
{
    struct Edge
    {
        int to;
        ll w;
    };

    int n = 0;
    ll k = 0;
    vector<vector<Edge>> g;
    Centroid(int n = 0, ll k = 0)
    {
        init(n, k);
    }

    void init(int n_, ll k_)
    {
        n = n_;
        k = k_;
        g.assign(n + 1, {});
        siz.assign(n + 1, 0);
        removed.assign(n + 1, 0);
        ans = 0;
    }

    void add(int u, int v, ll w)
    {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    ll work()
    {
        ans = 0;
        fill(removed.begin(), removed.end(), 0);
        if (n)
        {
            decompose(1);
        }
        return ans;
    }
private:
    ll ans = 0;
    vector<int> siz;
    vector<char> removed;
    void get_size(int u, int p)
    {
        siz[u] = 1;
        for (auto [v, w] : g[u])
        {
            if (v == p || removed[v])
            {
                continue;
            }
            get_size(v, u);
            siz[u] += siz[v];
        }
    }

    int get_centroid(int u, int p, int total)
    {
        for (auto [v, w] : g[u])
        {
            if (v == p || removed[v])
            {
                continue;
            }
            if (siz[v] > total / 2)
            {
                return get_centroid(v, u, total);
            }
        }
        return u;
    }

    void collect(int u, int p, __int128 dis, vector<__int128> &ds)
    {
        ds.push_back(dis);
        for (auto [v, w] : g[u])
        {
            if (v == p || removed[v])
            {
                continue;
            }
            collect(v, u, dis + w, ds);
        }
    }

    ll count_pairs(vector<__int128> &ds)
    {
        sort(ds.begin(), ds.end());
        ll res = 0;
        int l = 0, r = (int)ds.size() - 1;
        while (l < r)
        {
            if (ds[l] + ds[r] <= k)
            {
                res += r - l;
                ++l;
            }
            else
            {
                --r;
            }
        }
        return res;
    }

    void decompose(int entry)
    {
        get_size(entry, 0);
        int total = siz[entry];
        int c = get_centroid(entry, 0, total);
        removed[c] = 1;
        vector<__int128> all = {0};
        all.reserve(total);
        for (auto [v, w] : g[c])
        {
            if (removed[v])
            {
                continue;
            }
            vector<__int128> sub;
            collect(v, c, w, sub);
            ans -= count_pairs(sub);
            all.insert(all.end(), sub.begin(), sub.end());
        }
        ans += count_pairs(all);
        for (auto [v, w] : g[c])
        {
            if (!removed[v])
            {
                decompose(v);
            }
        }
    }
};
