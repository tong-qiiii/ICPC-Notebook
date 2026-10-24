#pragma once
#include "markov_absorb.hpp"

// 条件事件概率为 0 时，条件期望未定义，返回 NaN。
inline Walk markov_conditional(const vector<vector<db>> &p, const vector<int> &end)
{
    auto res = markov_absorb(p, end);
    int n = (int)p.size() - 1;
    vector<vector<db>> q(n + 1, vector<db>(n + 1));
    for (int u = 1; u <= n; u++)
    {
        if (end[u]) continue;
        if (res.prob[u] == 0)
        {
            q[u][u] = 1;
            continue;
        }
        long double old = 0, sum = 0;
        for (int v = 1; v <= n; v++)
        {
            if (v == u) continue;
            old += p[u][v];
            long double w = (long double)p[u][v] * res.prob[v] / res.prob[u];
            q[u][v] = (db)w;
            if (w > 0 && q[u][v] == 0) throw runtime_error("conditional probability underflow");
            sum += q[u][v];
        }
        q[u][u] = p[u][u];
        if (sum <= 0) throw runtime_error("invalid conditional transition");
        // 保留原自环，校正数值误差；不能用 1-p[u][u] 消去极小逃逸概率。
        for (int v = 1; v <= n; v++)
        {
            if (v != u)
            {
                db val = (db)(q[u][v] * old / sum);
                if (q[u][v] > 0 && val == 0) throw runtime_error("conditional probability underflow");
                q[u][v] = val;
            }
        }
    }
    auto ans = markov_absorb(q, end);
    for (int u = 1; u <= n; u++)
    {
        res.mean[u] = res.prob[u] > 0 ? ans.mean[u] : numeric_limits<db>::quiet_NaN();
    }
    return res;
}
