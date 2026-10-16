#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// 默认求谷底；cmp(a,b) 表示 a 比 b 更优。
// 两侧严格单调，可有最优值平台，不能直接用于任意阶梯函数。
template<class F, class Compare = less<>>
ll ternary_search_int(ll l, ll r, F f, Compare cmp = {})
{
    assert(l <= r);
    while ((__int128)r - l > 3)
    {
        ll d = (ll)(((__int128)r - l) / 3);
        ll m1 = l + d, m2 = r - d;
        auto v1 = f(m1), v2 = f(m2);
        if (cmp(v2, v1)) l = m1 + 1;
        else r = m2 - 1;
    }
    ll ans = l;
    auto best = f(l);
    // 先判断再递增，避免 r == LLONG_MAX 时溢出。
    for (ll i = l; i < r; )
    {
        auto val = f(++i);
        if (cmp(val, best))
        {
            best = val;
            ans = i;
        }
    }
    return ans;
}
