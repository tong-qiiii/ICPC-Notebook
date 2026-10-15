#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct LCATarjan
{
    int n = 0;
    vector<vector<int>> g;
    vector<vector<pair<int, int>>> qry;
    vector<int> ans;
    LCATarjan(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        g.assign(n + 1, {});
        qry.assign(n + 1, {});
        ans.clear();
        dsu.resize(n + 1);
        siz.assign(n + 1, 1);
        anc.assign(n + 1, 0);
        vis.assign(n + 1, 0);
    }

    void add(int u, int v)
    {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    int ask(int u, int v)
    {
        int id = ans.size();
        ans.push_back(0);
        qry[u].push_back({v, id});
        qry[v].push_back({u, id});
        return id;
    }

    const vector<int>& work(int root = 1)
    {
        iota(dsu.begin(), dsu.end(), 0);
        fill(siz.begin(), siz.end(), 1);
        fill(anc.begin(), anc.end(), 0);
        fill(vis.begin(), vis.end(), 0);
        fill(ans.begin(), ans.end(), 0);
        if (!n)
        {
            return ans;
        }
        tarjan(root, 0);
        return ans;
    }
private:
    vector<int> dsu, siz, anc;
    vector<char> vis;
    int find(int x)
    {
        return dsu[x] == x ? x : dsu[x] = find(dsu[x]);
    }

    void merge(int x, int y)
    {
        x = find(x);
        y = find(y);
        if (x == y)
        {
            return;
        }
        if (siz[x] < siz[y])
        {
            swap(x, y);
        }
        dsu[y] = x;
        siz[x] += siz[y];
    }

    void tarjan(int u, int p)
    {
        anc[u] = u;
        for (int v : g[u])
        {
            if (v == p)
            {
                continue;
            }
            tarjan(v, u);
            merge(u, v);
            anc[find(u)] = u;
        }
        vis[u] = 1;
        for (auto [v, id] : qry[u])
        {
            if (vis[v])
            {
                ans[id] = anc[find(v)];
            }
        }
    }
};
