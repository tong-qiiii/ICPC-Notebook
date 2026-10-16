#pragma once
#include <bits/stdc++.h>
#include "../math/gauss_double.hpp"
using namespace std;
#ifndef db
#define db double
#endif

struct Walk
{
    vector<db> prob, mean;
};

// p[1..n][1..n] 为转移概率，end[u] 为目标；目标处立即停止。
inline Walk markov_absorb(const vector<vector<db>> &p, const vector<int> &end)
{
    int n = (int)p.size() - 1;
    assert(n >= 0 && (int)end.size() == n + 1);
    vector<vector<int>> rev(n + 1);
    for (int u = 1; u <= n; u++)
    {
        assert((int)p[u].size() == n + 1);
        if (end[u]) continue;
        db sum = 0;
        for (int v = 1; v <= n; v++)
        {
            assert(isfinite(p[u][v]) && p[u][v] >= 0);
            sum += p[u][v];
            // 图上的可达性不能用浮点 EPS 删除小概率边。
            if (p[u][v] > 0) rev[v].push_back(u);
        }
        assert(abs(sum - 1) < 1e-10);
    }
    auto spread = [&](vector<int> vis)
    {
        queue<int> q;
        for (int u = 1; u <= n; u++) if (vis[u]) q.push(u);
        while (!q.empty())
        {
            int u = q.front(); q.pop();
            for (int v : rev[u])
            {
                if (!vis[v]) vis[v] = 1, q.push(v);
            }
        }
        return vis;
    };
    vector<int> can = spread(end), bad(n + 1);
    for (int u = 1; u <= n; u++) bad[u] = !can[u];
    bad = spread(bad);
    Walk res{vector<db>(n + 1), vector<db>(n + 1)};
    for (int u = 1; u <= n; u++)
    {
        if (end[u]) res.prob[u] = 1;
        if (bad[u]) res.mean[u] = numeric_limits<db>::infinity();
    }
    auto solve = [&](bool mean)
    {
        vector<int> id(n + 1, -1), ord;
        for (int u = 1; u <= n; u++)
        {
            if (!end[u] && (mean ? !bad[u] : can[u]))
            {
                id[u] = (int)ord.size();
                ord.push_back(u);
            }
        }
        int m = (int)ord.size();
        vector<vector<db>> a(m, vector<db>(m + 1));
        for (int i = 0; i < m; i++)
        {
            int u = ord[i];
            a[i][i] = 1;
            a[i][m] = mean ? 1 : 0;
            for (int v = 1; v <= n; v++)
            {
                if (id[v] != -1) a[i][id[v]] -= p[u][v];
                else if (!mean && end[v]) a[i][m] += p[u][v];
            }
        }
        vector<db> ans;
        // 图论已判断无限期望；数值求解失败不能冒充无穷或零。
        if (GaussD().solve(a, ans) != m) throw runtime_error("ill-conditioned Markov system");
        for (int i = 0; i < m; i++)
        {
            if (mean) res.mean[ord[i]] = ans[i];
            else res.prob[ord[i]] = ans[i];
        }
    };
    solve(false);
    solve(true);
    return res;
}
