#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct DSUOnTree
{
    int n = 0, diff = 0;
    vector<vector<int>> g;
    vector<int> color, siz, son, ans;
    DSUOnTree(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        g.assign(n + 1, {});
        color.assign(n + 1, 0);
        siz.assign(n + 1, 0);
        son.assign(n + 1, 0);
        ans.assign(n + 1, 0);
        col.assign(n + 1, 0);
        cnt.clear();
        diff = 0;
    }

    void add(int u, int v)
    {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void set_color(int u, int c)
    {
        color[u] = c;
    }

    const vector<int>& work(int root = 1)
    {
        if (!n)
        {
            return ans;
        }
        vector<int> xs(color.begin() + 1, color.end());
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        for (int u = 1; u <= n; ++u)
        {
            col[u] = lower_bound(xs.begin(), xs.end(), color[u]) - xs.begin();
        }
        cnt.assign(xs.size(), 0);
        fill(son.begin(), son.end(), 0);
        fill(ans.begin(), ans.end(), 0);
        diff = 0;
        dfs_size(root, 0);
        dfs(root, 0, false);
        return ans;
    }
private:
    vector<int> col, cnt;
    void dfs_size(int u, int p)
    {
        siz[u] = 1;
        for (int v : g[u])
        {
            if (v == p)
            {
                continue;
            }
            dfs_size(v, u);
            siz[u] += siz[v];
            if (siz[v] > siz[son[u]])
            {
                son[u] = v;
            }
        }
    }

    void add_node(int u)
    {
        if (cnt[col[u]]++ == 0)
        {
            ++diff;
        }
    }

    void del_node(int u)
    {
        if (--cnt[col[u]] == 0)
        {
            --diff;
        }
    }

    void add_subtree(int u, int p)
    {
        add_node(u);
        for (int v : g[u])
        {
            if (v != p)
            {
                add_subtree(v, u);
            }
        }
    }

    void del_subtree(int u, int p)
    {
        del_node(u);
        for (int v : g[u])
        {
            if (v != p)
            {
                del_subtree(v, u);
            }
        }
    }

    void dfs(int u, int p, bool keep)
    {
        for (int v : g[u])
        {
            if (v == p || v == son[u])
            {
                continue;
            }
            dfs(v, u, false);
        }
        if (son[u])
        {
            dfs(son[u], u, true);
        }
        for (int v : g[u])
        {
            if (v == p || v == son[u])
            {
                continue;
            }
            add_subtree(v, u);
        }
        add_node(u);
        ans[u] = diff;
        if (!keep)
        {
            del_subtree(u, p);
        }
    }
};
