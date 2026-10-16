#pragma once
#include <bits/stdc++.h>
using namespace std;

using WeightedTree = vector<vector<pair<int, long long>>>;

inline void add_edge(WeightedTree& g, int u, int v, long long w)
{
    g[u].push_back({v, w});
    g[v].push_back({u, w});
}

template<class Solver>
void check_diameter(const WeightedTree& g, Solver solve)
{
    auto before = g;
    auto result = solve(g);
    assert(solve(g) == result);
    assert(g == before);
    auto [len, s, t] = result;
    int n = (int)g.size() - 1;
    if (n <= 0)
    {
        assert(len == 0 && s == 0 && t == 0);
        return;
    }
    vector<vector<long long>> dis(n + 1, vector<long long>(n + 1));
    long long best = 0;
    for (int root = 1; root <= n; root++)
    {
        vector<int> vis(n + 1);
        queue<int> q;
        q.push(root);
        vis[root] = 1;
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (auto [v, w] : g[u])
            {
                if (vis[v]) continue;
                vis[v] = 1;
                dis[root][v] = dis[root][u] + w;
                best = max(best, dis[root][v]);
                q.push(v);
            }
        }
    }
    assert(1 <= s && s <= n && 1 <= t && t <= n);
    assert(len == best && dis[s][t] == best);
}

template<class Solver>
void random_diameters(Solver solve, bool negative)
{
    mt19937 rng(20261016);
    for (int round = 0; round < 600; round++)
    {
        int n = 1 + rng() % 40;
        vector<int> id(n);
        iota(id.begin(), id.end(), 1);
        shuffle(id.begin(), id.end(), rng);
        WeightedTree g(n + 1);
        for (int i = 1; i < n; i++)
        {
            long long w = rng() % 31;
            if (negative) w -= 15;
            add_edge(g, id[i], id[rng() % i], w);
        }
        check_diameter(g, solve);
    }
}
