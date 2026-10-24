#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// a[1..n] 非负且 < 2^61，0 <= k < 2^61；每次加 1 花费 1。
inline ll max_and_increment(vector<ll> a, ll k)
{
    int n = (int)a.size() - 1;
    assert(n > 0 && 0 <= k && k < (1LL << 61));
    ll ans = 0;
    for (int b = 61; b >= 0; b--)
    {
        ll bit = 1LL << b, cost = 0;
        bool ok = true;
        for (int i = 1; i <= n; i++)
        {
            if (a[i] & bit)
            {
                continue;
            }
            ll need = bit - (a[i] & (bit - 1));
            if (need > k - cost)
            {
                ok = false;
                break;
            }
            cost += need;
        }
        if (!ok)
        {
            continue;
        }
        ans |= bit;
        k -= cost;
        for (int i = 1; i <= n; i++)
        {
            if (!(a[i] & bit))
            {
                a[i] += bit - (a[i] & (bit - 1));
            }
        }
    }
    return ans;
}
