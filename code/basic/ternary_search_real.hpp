#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef db
#define db double
#endif

// 默认求谷底；两侧严格单调，可有最优值平台。
template<class F, class Compare = less<>>
db ternary_search_real(db l, db r, F f,
    Compare cmp = {}, int iter = 100)
{
    assert(isfinite(l) && isfinite(r) && l <= r);
    assert(isfinite(r - l) && iter > 0);
    for (int i = 0; i < iter; i++)
    {
        db d = (r - l) / 3;
        db m1 = l + d, m2 = r - d;
        if (m1 <= l || m2 >= r || m1 >= m2) break;
        auto v1 = f(m1), v2 = f(m2);
        if (cmp(v2, v1)) l = m1;
        else r = m2;
    }
    return l + (r - l) / 2;
}
