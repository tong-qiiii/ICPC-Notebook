#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct EulerUndir
{
    struct Edge
    {
        int to, id;
    };

    int n, m;
    vector<vector<Edge>> g;
    vector<int> deg;

    EulerUndir(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        m = 0;
        g.assign(n + 1, {});
        deg.assign(n + 1, 0);
    }

    void add(int u, int v)
    {
        g[u].push_back({v, m});
        g[v].push_back({u, m});
        ++deg[u];
        ++deg[v];
        ++m;
    }

    int get_start() const

    {
        int odd = 0;
        int start = -1, any = -1;

        for (int i = 1; i <= n; ++i)
        {
            if (deg[i])
                any = i;

            if (deg[i] & 1)
            {
                ++odd;
                start = i;
            }
        }

        if (odd == 2)
            return start;

        if (odd == 0)
            return any == -1 ? 1 : any;

        return -1;
    }

    vector<int> work() const

    {
        if (!n) return {};
        int start = get_start();

        if (start == -1)
            return {};

        vector<int> cur(n + 1, 0);
        vector<char> used(m, false);
        vector<int> stk = {start};
        vector<int> path;

        while (!stk.empty())
        {
            int u = stk.back();

            while (cur[u] < (int)g[u].size() && used[g[u][cur[u]].id])
            {
                ++cur[u];
            }

            if (cur[u] == (int)g[u].size())
            {
                path.push_back(u);
                stk.pop_back();
            }
            else
            {
                auto [v, id] = g[u][cur[u]++];
                used[id] = true;
                stk.push_back(v);
            }
        }

        if ((int)path.size() != m + 1)
            return {};

        reverse(path.begin(), path.end());
        return path;
    }
};
