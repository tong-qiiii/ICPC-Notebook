#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

template <class F> double simpson(const F &f, double l, double r)
{
    double mid = (l + r) / 2;
    return (f(l) + 4 * f(mid) + f(r)) * (r - l) / 6;
}

template <class F>
double adaptive_simpson_dfs(const F &f, double l, double r, double whole,
                            double eps, int dep)
{
    double mid = (l + r) / 2;
    double left = simpson(f, l, mid);
    double right = simpson(f, mid, r);
    double delta = left + right - whole;

    if (dep == 0 || mid == l || mid == r || fabs(delta) <= 15 * eps)
    {
        return left + right + delta / 15;
    }

    return adaptive_simpson_dfs(f, l, mid, left, eps / 2, dep - 1) +
           adaptive_simpson_dfs(f, mid, r, right, eps / 2, dep - 1);
}

template <class F>
double adaptive_simpson(const F &f, double l, double r, double eps = 1e-8,
                        int max_dep = 30)
{
    if (l == r)
    {
        return 0;
    }

    if (l > r)
    {
        return -adaptive_simpson(f, r, l, eps, max_dep);
    }

    return adaptive_simpson_dfs(f, l, r, simpson(f, l, r), eps, max_dep);
}
