#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// mod 必须为质数且 <= INT_MAX；空行列式为 1。
inline ll det_mod(vector<vector<ll>> a, int mod)
{
    assert(mod >= 2);
    int n = a.size();
    for (auto &v : a)
    {
        assert((int)v.size() == n);
        for (ll &x : v) x = (x % mod + mod) % mod;
    }
    auto power = [&](ll x, int k)
    {
        ll res = 1;
        for (; k; k >>= 1, x = x * x % mod)
        {
            if (k & 1) res = res * x % mod;
        }
        return res;
    };
    ll ans = 1;
    for (int i = 0; i < n; i++)
    {
        int p = i;
        while (p < n && !a[p][i]) p++;
        if (p == n) return 0;
        if (p != i)
        {
            swap(a[p], a[i]);
            ans = (mod - ans) % mod;
        }
        ans = ans * a[i][i] % mod;
        ll inv = power(a[i][i], mod - 2);
        for (int j = i + 1; j < n; j++)
        {
            ll v = a[j][i] * inv % mod;
            for (int k = i; k < n; k++)
            {
                a[j][k] = (a[j][k] - v * a[i][k] % mod + mod) % mod;
            }
        }
    }
    return ans;
}
