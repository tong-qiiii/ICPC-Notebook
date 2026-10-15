#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct TwoSAT
{
    int n, timer, cnt;
    vector<vector<int>> g;
    vector<int> dfn, low, bel, stk;
    vector<char> in;
    vector<int> assignment;

    TwoSAT(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        timer = cnt = 0;
        g.assign(2 * n + 1, {});
        dfn.assign(2 * n + 1, 0);
        low.assign(2 * n + 1, 0);
        bel.assign(2 * n + 1, 0);
        stk.clear();
        in.assign(2 * n + 1, false);
        assignment.assign(n + 1, 0);
    }

    int id(int x, bool v) const

    {
        return 2 * x - (v ? 1 : 0);
    }

    int opp(int u) const

    {
        return (u & 1) ? u + 1 : u - 1;
    }

    void either(int x, bool vx, int y, bool vy)
    {
        int a = id(x, vx);
        int b = id(y, vy);

        g[opp(a)].push_back(b);
        g[opp(b)].push_back(a);
    }

    void force(int x, bool v)
    {
        either(x, v, x, v);
    }

    void eq(int x, int y)
    {
        either(x, true, y, false);
        either(x, false, y, true);
    }

    void neq(int x, int y)
    {
        either(x, true, y, true);
        either(x, false, y, false);
    }

    void dfs(int u)
    {
        dfn[u] = low[u] = ++timer;
        stk.push_back(u);
        in[u] = true;

        for (int v : g[u])
        {
            if (!dfn[v])
            {
                dfs(v);
                low[u] = min(low[u], low[v]);
            }
            else if (in[v])
            {
                low[u] = min(low[u], dfn[v]);
            }
        }

        if (low[u] == dfn[u])
        {
            ++cnt;

            while (true)
            {
                int x = stk.back();
                stk.pop_back();
                in[x] = false;
                bel[x] = cnt;
                if (x == u) break;
            }
        }
    }

    bool satisfiable()
    {
        timer = cnt = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(bel.begin(), bel.end(), 0);
        fill(in.begin(), in.end(), false);
        fill(assignment.begin(), assignment.end(), 0);
        stk.clear();

        for (int i = 1; i <= 2 * n; ++i)
            if (!dfn[i])
                dfs(i);

        for (int x = 1; x <= n; ++x)
        {
            int f = id(x, false);
            int t = id(x, true);

            if (bel[f] == bel[t])
                return false;

            assignment[x] = bel[t] < bel[f];
        }

        return true;
    }
};
