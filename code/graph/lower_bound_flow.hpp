#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif
#include "dinic.hpp"

struct LowerBoundFlow
{
    struct Info
    {
        int u, idx;
        ll low, cap;
    };

    int n, S, T;
    Dinic dinic;
    vector<ll> bal;
    vector<Info> edges;
    bool done = false, ok = false;

    LowerBoundFlow(int n_) : n(n_), S(n + 1), T(n + 2), dinic(n + 2), bal(n + 1)
    {
    }

    int add(int u, int v, ll low, ll high)
    {
        assert(!done && 0 <= low && low <= high);
        int idx = dinic.add(u, v, high - low);

        bal[u] -= low;
        bal[v] += low;
        edges.push_back({u, idx, low, high - low});

        return (int)edges.size() - 1;
    }

    bool work()
    {
        if (done) return ok;
        done = true;
        ll need = 0;

        for (int u = 1; u <= n; ++u)
        {
            if (bal[u] > 0)
            {
                dinic.add(S, u, bal[u]);
                need += bal[u];
            }
            else if (bal[u] < 0)
            {
                dinic.add(u, T, -bal[u]);
            }
        }

        return ok = dinic.maxflow(S, T) == need;
    }

    ll edgeFlow(int id) const

    {
        assert(done && ok);
        auto [u, idx, low, cap] = edges[id];
        return low + cap - dinic.g[u][idx].cap;
    }
};
