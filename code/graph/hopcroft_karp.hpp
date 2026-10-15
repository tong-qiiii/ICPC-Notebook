#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct HopcroftKarp
{
    int nL, nR;
    vector<vector<int>> g;
    vector<int> matchL, matchR, dis, cur;

    HopcroftKarp(int nL_, int nR_) : nL(nL_), nR(nR_)
    {
        g.assign(nL + 1, {});
        matchL.assign(nL + 1, 0);
        matchR.assign(nR + 1, 0);
        dis.resize(nL + 1);
        cur.resize(nL + 1);
    }

    void add(int u, int v)
    {
        g[u].push_back(v);
    }

    bool bfs()
    {
        queue<int> q;
        bool found = false;

        fill(dis.begin(), dis.end(), -1);

        for (int u = 1; u <= nL; ++u)
        {
            if (!matchL[u])
            {
                dis[u] = 0;
                q.push(u);
            }
        }

        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            if (dis[0] != -1 && dis[u] >= dis[0]) continue;

            for (int v : g[u])
            {
                int w = matchR[v];

                if (!w)
                {
                    found = true;
                    if (dis[0] == -1) dis[0] = dis[u] + 1;
                }
                else if (dis[w] == -1)
                {
                    dis[w] = dis[u] + 1;
                    q.push(w);
                }
            }
        }

        return found;
    }

    bool dfs(int u)
    {
        for (int& i = cur[u]; i < (int)g[u].size(); ++i)
        {
            int v = g[u][i];
            int w = matchR[v];

            if ((!w && dis[u] + 1 == dis[0]) || (w && dis[w] == dis[u] + 1 && dfs(w)))
            {
                matchL[u] = v;
                matchR[v] = u;
                return true;
            }
        }

        dis[u] = -1;
        return false;
    }

    int work()
    {
        fill(matchL.begin(), matchL.end(), 0);
        fill(matchR.begin(), matchR.end(), 0);

        int ans = 0;

        while (bfs())
        {
            fill(cur.begin(), cur.end(), 0);
            for (int u = 1; u <= nL; ++u)
                if (!matchL[u] && dfs(u)) ++ans;
        }

        return ans;
    }
};
