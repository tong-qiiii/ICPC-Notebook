#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline mt19937_64 gen(20261014);

inline int rand_int(int l, int r)
{
    return uniform_int_distribution<int>(l, r)(gen);
}

inline void check(bool ok, const char* msg)
{
    if (!ok) throw runtime_error(msg);
}

inline void passed(const char* name)
{
    cout << "PASS " << name << '\n';
}

struct TestEdge
{
    int u, v;
    ll w;
};

inline vector<vector<int>> reachability(const vector<vector<int>>& g)
{
    int n = (int)g.size() - 1;
    vector<vector<int>> reach(n + 1, vector<int>(n + 1));
    for (int s = 1; s <= n; ++s)
    {
        queue<int> q;
        reach[s][s] = 1;
        q.push(s);
        while (!q.empty())
        {
            int u = q.front(); q.pop();
            for (int v : g[u])
                if (!reach[s][v])
                {
                    reach[s][v] = 1;
                    q.push(v);
                }
        }
    }
    return reach;
}

inline bool acyclic(const vector<vector<int>>& g)
{
    auto reach = reachability(g);
    for (int u = 1; u < (int)g.size(); ++u)
        for (int v : g[u])
            if (reach[v][u]) return false;
    return true;
}

inline vector<int> component_ids(int n, const vector<pair<int, int>>& e, int skip = -1, int removed = -1)
{
    vector<vector<int>> g(n + 1);
    for (int i = 0; i < (int)e.size(); ++i)
    {
        auto [u, v] = e[i];
        if (i == skip || u == removed || v == removed) continue;
        g[u].push_back(v); g[v].push_back(u);
    }
    vector<int> bel(n + 1);
    int cnt = 0;
    for (int s = 1; s <= n; ++s)
    {
        if (s == removed || bel[s]) continue;
        queue<int> q;
        bel[s] = ++cnt; q.push(s);
        while (!q.empty())
        {
            int u = q.front(); q.pop();
            for (int v : g[u])
                if (!bel[v])
                {
                    bel[v] = cnt;
                    q.push(v);
                }
        }
    }
    return bel;
}

inline int components(int n, const vector<pair<int, int>>& e, int skip = -1, int removed = -1)
{
    auto bel = component_ids(n, e, skip, removed);
    return *max_element(bel.begin(), bel.end());
}

inline bool negative_cycle_brute(int n, const vector<TestEdge>& e)
{
    vector<__int128> dis(n + 1);
    for (int i = 1; i <= n; ++i)
    {
        bool changed = false;
        for (auto [u, v, w] : e)
            if (dis[v] > dis[u] + w)
            {
                dis[v] = dis[u] + w;
                changed = true;
            }
        if (!changed) return false;
    }
    return n != 0;
}

inline ll cut_brute(int n, const vector<TestEdge>& e, int s = 0, int t = 0, bool undirected = true)
{
    if (n <= 1) return 0;
    ll ans = numeric_limits<ll>::max();
    for (int mask = 1; mask < (1 << n) - 1; ++mask)
    {
        if (s && (!(mask >> (s - 1) & 1) || (mask >> (t - 1) & 1))) continue;
        ll sum = 0;
        for (auto [u, v, w] : e)
        {
            int a = mask >> (u - 1) & 1, b = mask >> (v - 1) & 1;
            if (undirected ? a != b : a && !b) sum += w;
        }
        ans = min(ans, sum);
    }
    return ans;
}
