#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef db
#define db double
#endif

// 位置 0,n 吸收，每次以 p 向右，1-p 向左。
inline pair<vector<db>, vector<db>> walk_line(int n, db p)
{
    assert(n >= 1 && 0 <= p && p <= 1);
    vector<db> prob(n + 1), mean(n + 1), c(n + 1);
    prob[n] = 1;
    db q = 1 - p;
    // 同一三对角系数，分别解到右端概率与首次到边界的期望步数。
    for (int i = 1; i < n; i++)
    {
        db den = 1 - q * c[i - 1];
        if (den <= 0) throw runtime_error("ill-conditioned line walk");
        c[i] = p / den;
        prob[i] = ((i == n - 1 ? p : 0) + q * prob[i - 1]) / den;
        mean[i] = (1 + q * mean[i - 1]) / den;
    }
    for (int i = n - 2; i >= 1; i--)
    {
        prob[i] += c[i] * prob[i + 1];
        mean[i] += c[i] * mean[i + 1];
    }
    return {prob, mean};
}
