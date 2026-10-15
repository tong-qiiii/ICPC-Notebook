#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct EulerDir
{
    int n, m;
    vector<vector<int>> g;
    vector<int> indeg, outdeg;

    EulerDir(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        m = 0;
        g.assign(n + 1, {});
        indeg.assign(n + 1, 0);
        outdeg.assign(n + 1, 0);
    }

    void add(int u, int v)
    {
        g[u].push_back(v);
        ++outdeg[u];
        ++indeg[v];
        ++m;
    }

    int get_start() const

    {
        int plus = 0, minus = 0;
        int start = -1, any = -1;

        for (int i = 1; i <= n; ++i)
        {
            if (outdeg[i])
                any = i;

            int d = outdeg[i] - indeg[i];

            if (d == 1)
            {
                ++plus;
                start = i;
            }
            else if (d == -1)
            {
                ++minus;
            }
            else if (d != 0)
            {
                return -1;
            }
        }

        if (plus == 1 && minus == 1)
            return start;

        if (plus == 0 && minus == 0)
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
        vector<int> stk = {start};
        vector<int> path;

        while (!stk.empty())
        {
            int u = stk.back();

            if (cur[u] < (int)g[u].size())
            {
                stk.push_back(g[u][cur[u]++]);
            }
            else
            {
                path.push_back(u);
                stk.pop_back();
            }
        }

        if ((int)path.size() != m + 1)
            return {};

        reverse(path.begin(), path.end());
        return path;
    }
};
