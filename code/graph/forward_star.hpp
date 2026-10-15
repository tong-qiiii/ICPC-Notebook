#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct ForwardStar
{
    struct Edge
    {
        int to, nxt;
        ll w;
    };

    int n;
    vector<int> head;
    vector<Edge> e;

    ForwardStar(int n = 0, int m = 0)
    {
        init(n, m);
    }

    void init(int n_, int m = 0)
    {
        n = n_;
        head.assign(n + 1, -1);
        e.clear();
        if (m)
            e.reserve(m);
    }

    int add(int u, int v, ll w = 1)
    {
        int id = e.size();
        e.push_back({v, head[u], w});
        head[u] = id;
        return id;
    }
};
