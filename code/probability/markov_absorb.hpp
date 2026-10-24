#pragma once
#include <bits/stdc++.h>
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
        using ld = long double;
        vector<vector<ld>> w(m, vector<ld>(m));
        vector<ld> out(m), rhs(m), den(m), ans(m);
        for (int i = 0; i < m; i++)
        {
            int u = ord[i];
            rhs[i] = mean ? 1 : 0;
            for (int v = 1; v <= n; v++)
            {
                if (v == u) continue;
                if (id[v] != -1) w[i][id[v]] += p[u][v];
                else out[i] += p[u][v];
                if (!mean && end[v]) rhs[i] += p[u][v];
            }
        }
        auto old = w;
        auto old_out = out, old_rhs = rhs;
        // 对角为非自环出边之和；消元只加非负数，避免 1-p[u][u] 消减。
        for (int k = 0; k < m; k++)
        {
            den[k] = out[k];
            for (int j = k + 1; j < m; j++) den[k] += w[k][j];
            if (!(den[k] > 0) || !isfinite(den[k])) throw runtime_error("Markov pivot underflow");
            for (int i = k + 1; i < m; i++)
            {
                ld t = w[i][k] / den[k];
                out[i] += t * out[k];
                rhs[i] += t * rhs[k];
                for (int j = k + 1; j < m; j++)
                {
                    if (j != i) w[i][j] += t * w[k][j];
                }
                w[i][k] = 0;
            }
        }
        for (int i = m - 1; i >= 0; i--)
        {
            ld val = rhs[i];
            for (int j = i + 1; j < m; j++) val += w[i][j] * ans[j];
            ans[i] = val / den[i];
            if (!(ans[i] > 0) || !isfinite(ans[i]) || ans[i] > numeric_limits<db>::max() ||
                (ans[i] > 0 && (db)ans[i] == 0)) throw runtime_error("Markov result outside db range");
        }
        ld eps = 1024 * max(1, m) * numeric_limits<ld>::epsilon();
        for (int i = 0; i < m; i++)
        {
            ld diag = old_out[i], val = old_rhs[i];
            for (int j = 0; j < m; j++)
            {
                diag += old[i][j];
                val += old[i][j] * ans[j];
            }
            ld lhs = diag * ans[i];
            ld scale = max(abs(lhs) + abs(val), numeric_limits<ld>::min());
            if (!isfinite(lhs) || !isfinite(val) || abs(lhs - val) > eps * scale)
                throw runtime_error("Markov residual too large");
        }
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
