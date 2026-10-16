#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef db
#define db double
#endif

// check 从 false 变 true，右端点必须可行。
template<class Check>
db binary_search_real(db l, db r, Check check, int iter = 100)
{
    assert(isfinite(l) && isfinite(r) && l <= r);
    assert(isfinite(r - l) && iter > 0);
    if (check(l)) return l;
    assert(check(r));
    for (int i = 0; i < iter; i++)
    {
        db mid = l + (r - l) / 2;
        if (mid <= l || mid >= r) break;
        if (check(mid)) r = mid;
        else l = mid;
    }
    return r;
}
