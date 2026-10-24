#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif
#ifndef db
#define db double
#endif

// 花 x 除以 a，或花 y 掷公平六面骰再除以点数；均向下取整。
inline db toward_zero(ll n, int a, db x, db y)
{
    assert(n >= 0 && n <= 1000000000000000000LL && 2 <= a && a <= 6);
    assert(isfinite(x) && isfinite(y) && x > 0 && y > 0);
    unordered_map<ll, db> f{{0, 0}};
    auto dfs = [&](auto &&self, ll v) -> db
    {
        if (f.count(v)) return f[v];
        db sum = 6 * y;
        for (int d = 2; d <= 6; d++) sum += self(self, v / d);
        return f[v] = min(x + self(self, v / a), sum / 5);
    };
    return dfs(dfs, n);
}
