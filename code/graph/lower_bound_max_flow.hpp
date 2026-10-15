#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif
#include "dinic.hpp"

struct LowerBoundMaxFlow
{
    int n, S, T;
    Dinic dinic;
    vector<ll> bal;
    bool done = false;
    int s0 = 0, t0 = 0;
    ll ans = -1;

    LowerBoundMaxFlow(int n_) : n(n_), S(n + 1), T(n + 2), dinic(n + 2), bal(n + 1)
    {
    }

    void add(int u, int v, ll low, ll high)
    {
        assert(!done && 0 <= low && low <= high);
        dinic.add(u, v, high - low);
        bal[u] -= low;
        bal[v] += low;
    }

    ll work(int s, int t)
    {
        if (done)
        {
            assert(s == s0 && t == t0);
            return ans;
        }
        assert(s != t);
        done = true;
        s0 = s; t0 = t;
        const ll INF = 1LL << 62;

        int back = dinic.add(t, s, INF);
        vector<pair<int, int>> extra;

        ll need = 0;

        for (int u = 1; u <= n; ++u)
        {
            if (bal[u] > 0)
            {
                int id = dinic.add(S, u, bal[u]);
                extra.push_back({S, id});
                need += bal[u];
            }
            else if (bal[u] < 0)
            {
                int id = dinic.add(u, T, -bal[u]);
                extra.push_back({u, id});
            }
        }

        if (dinic.maxflow(S, T) != need) return -1;

        ll base = INF - dinic.g[t][back].cap;

        for (auto [u, id] : extra) dinic.disable(u, id);
        dinic.disable(t, back);

        return ans = base + dinic.maxflow(s, t);
    }
};
