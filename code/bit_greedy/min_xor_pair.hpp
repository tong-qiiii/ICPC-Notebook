#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// a[1..n] 非负，n>=2；返回一对原下标，允许不同位置值相同。
inline pair<int, int> min_xor_pair(const vector<ll> &a)
{
    assert(a.size() >= 3);
    vector<pair<ll, int>> v;
    for (int i = 1; i < (int)a.size(); i++)
    {
        v.push_back({a[i], i});
    }
    sort(v.begin(), v.end());
    ll best = LLONG_MAX;
    pair<int, int> ans = {v[0].second, v[1].second};
    for (int i = 1; i < (int)v.size(); i++)
    {
        ll cur = v[i - 1].first ^ v[i].first;
        if (cur < best)
        {
            best = cur;
            ans = {v[i - 1].second, v[i].second};
        }
    }
    return ans;
}
