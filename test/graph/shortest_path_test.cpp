#include "check.hpp"
#include "code/graph/dijkstra.hpp"
#include "code/graph/negative_cycle.hpp"
#include "code/graph/floyd.hpp"
#include "code/graph/transitive_closure.hpp"
#include "code/graph/difference_constraints.hpp"
#include "code/graph/kruskal.hpp"
#include "code/graph/prim.hpp"
#include "code/graph/zero_one_bfs.hpp"

vector<ll> shortest_brute(int n, const vector<TestEdge>& e, int s)
{
    const ll INF = 1LL << 62;
    vector<ll> dis(n + 1, INF);
    dis[s] = 0;
    for (int i = 1; i < n; ++i)
        for (auto [u, v, w] : e)
            if (dis[u] != INF) dis[v] = min(dis[v], dis[u] + w);
    return dis;
}

pair<bool, ll> mst_brute(int n, const vector<TestEdge>& e)
{
    if (n <= 1) return {true, 0};
    ll ans = LLONG_MAX;
    for (int mask = 0; mask < (1 << e.size()); ++mask)
    {
        if (__builtin_popcount(unsigned(mask)) != n - 1) continue;
        vector<pair<int, int>> selected;
        ll sum = 0;
        for (int i = 0; i < (int)e.size(); ++i)
            if (mask >> i & 1)
            {
                selected.push_back({e[i].u, e[i].v}); sum += e[i].w;
            }
        if (components(n, selected) == 1) ans = min(ans, sum);
    }
    return ans == LLONG_MAX ? pair<bool, ll>{false, 0} : pair<bool, ll>{true, ans};
}

void test_shortest()
{
    const ll INF = 1LL << 62;
    vector<vector<pair<int, ll>>> sample(5);
    sample[1] = {{2, 9}, {3, 1}};
    sample[3] = {{2, 2}};
    auto dis = dijkstra(sample, 1);
    check(dis[2] == 3 && dis[4] == INF, "Dijkstra sample/stale state/unreachable");
    sample[1].push_back({4, LLONG_MAX});
    check(dijkstra(sample, 1)[4] == INF, "Dijkstra overflowing candidate ignored");
    for (int tc = 0; tc < 180; ++tc)
    {
        int n = rand_int(1, 7);
        vector<vector<pair<int, ll>>> g(n + 1);
        vector<TestEdge> e;
        for (int i = rand_int(0, 20); i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, n), w = rand_int(0, 20);
            e.push_back({u, v, w}); g[u].push_back({v, w});
        }
        for (int s = 1; s <= n; ++s)
        {
            auto actual = dijkstra(g, s);
            check(actual == shortest_brute(n, e, s), "Dijkstra Bellman-Ford reference");
            check(dijkstra(g, s) == actual, "Dijkstra repeated source");
        }
    }
    passed("dijkstra");

    vector<vector<pair<int, ll>>> g(1);
    check(!neg_cycle(g), "empty negative cycle detector");
    g.resize(5); g[3].push_back({4, -1}); g[4].push_back({3, 0});
    check(neg_cycle(g) && neg_cycle(g), "disconnected negative cycle/reuse");
    g.assign(3, {}); g[1].push_back({2, LLONG_MIN});
    check(!neg_cycle(g), "large negative acyclic edge");
    g[2].push_back({1, 0}); check(neg_cycle(g), "large negative cycle without signed overflow");
    for (int tc = 0; tc < 180; ++tc)
    {
        int n = rand_int(1, 7);
        g.assign(n + 1, {});
        vector<TestEdge> e;
        for (int i = rand_int(0, 20); i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, n), w = rand_int(-10, 10);
            e.push_back({u, v, w}); g[u].push_back({v, w});
        }
        check(neg_cycle(g) == negative_cycle_brute(n, e), "negative cycle Bellman-Ford reference");
    }
    passed("negative_cycle");

    vector<vector<ll>> empty;
    floyd(empty); check(empty.empty(), "empty Floyd");
    for (int tc = 0; tc < 150; ++tc)
    {
        int n = rand_int(1, 7);
        vector<vector<ll>> d(n + 1, vector<ll>(n + 1, INF));
        vector<TestEdge> e;
        for (int u = 1; u <= n; ++u) d[u][u] = 0;
        for (int i = rand_int(0, 20); i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, n), w = rand_int(-10, 10);
            if (u >= v) continue;
            e.push_back({u, v, w}); d[u][v] = min(d[u][v], (ll)w);
        }
        floyd(d);
        for (int s = 1; s <= n; ++s)
        {
            auto expected = shortest_brute(n, e, s);
            for (int v = 1; v <= n; ++v) check(d[s][v] == expected[v], "Floyd all-source reference");
        }
        auto old = d; floyd(d); check(d == old, "Floyd repeated no-cycle result");
    }
    vector<vector<ll>> cycle(12, vector<ll>(12, INF));
    for (int u = 1; u <= 11; ++u)
    {
        cycle[u][u] = 0; cycle[u][u % 11 + 1] = -(1LL << 61);
    }
    floyd(cycle);
    for (int u = 1; u <= 11; ++u) check(cycle[u][u] < 0, "Floyd negative-cycle saturation");
    passed("floyd");

    vector<bitset<16>> reach(2);
    closure(reach, 1); check(!reach[1][1], "closure does not force reflexivity");
    for (int tc = 0; tc < 100; ++tc)
    {
        int n = rand_int(0, 8);
        reach.assign(n + 1, {});
        vector<vector<int>> adjacency(n + 1);
        for (int u = 1; u <= n; ++u) reach[u][u] = 1;
        for (int i = n ? rand_int(0, 20) : 0; i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, n);
            adjacency[u].push_back(v); reach[u][v] = 1;
        }
        closure(reach, n);
        auto expected = reachability(adjacency);
        for (int u = 1; u <= n; ++u)
            for (int v = 1; v <= n; ++v) check(reach[u][v] == bool(expected[u][v]), "bitset BFS reference");
        auto old = reach; closure(reach, n); check(old == reach, "closure repeat");
    }
    passed("transitive_closure");

    DiffCons dc;
    check(dc.solve().first, "zero-variable difference constraints");
    dc.init(2); dc.leq(1, 2, -3); dc.leq(2, 1, 1);
    check(!dc.solve().first, "inconsistent difference constraints");
    for (int tc = 0; tc < 150; ++tc)
    {
        int n = rand_int(1, 6); dc.init(n);
        vector<TestEdge> e;
        for (int i = rand_int(0, 18); i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, n), w = rand_int(-10, 10);
            dc.leq(u, v, w); e.push_back({u, v, w});
        }
        auto [ok, val] = dc.solve();
        check(ok == !negative_cycle_brute(n, e) && dc.solve().first == ok, "difference constraints consistency/reuse");
        if (ok)
            for (auto [u, v, w] : e) check(val[v] - val[u] <= w, "difference constraints witness");
    }
    passed("difference_constraints");
}

void test_mst_zero_one()
{
    check(kruskal(0, {}) == pair<bool, ll>{true, 0}, "empty Kruskal");
    check(prim(vector<vector<pair<int, ll>>>(1)) == pair<bool, ll>{true, 0}, "empty Prim");
    for (int tc = 0; tc < 150; ++tc)
    {
        int n = rand_int(1, 6);
        vector<TestEdge> e;
        vector<MstEdge> edges;
        vector<vector<pair<int, ll>>> g(n + 1);
        for (int i = rand_int(0, 9); i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, n), w = rand_int(-20, 20);
            e.push_back({u, v, w}); edges.push_back({u, v, w});
            g[u].push_back({v, w}); g[v].push_back({u, w});
        }
        auto expected = mst_brute(n, e), a = kruskal(n, edges), b = prim(g);
        check(a.first == expected.first && b.first == expected.first, "MST connectivity reference");
        if (expected.first) check(a.second == expected.second && b.second == expected.second, "MST exhaustive spanning-tree reference");
        check(kruskal(n, edges) == a && prim(g) == b, "MST repeated calls");
    }
    passed("kruskal"); passed("prim");
    const ll INF = 1LL << 60;
    for (int tc = 0; tc < 150; ++tc)
    {
        int n = rand_int(1, 7);
        vector<vector<pair<int, int>>> g(n + 1);
        vector<TestEdge> e;
        for (int i = rand_int(0, 22); i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, n), w = rand_int(0, 1);
            g[u].push_back({v, w}); e.push_back({u, v, w});
        }
        for (int s = 1; s <= n; ++s)
        {
            auto expected = shortest_brute(n, e, s), actual = zeroOneBFS(g, s);
            for (int v = 1; v <= n; ++v)
                check(actual[v] == (expected[v] == (1LL << 62) ? INF : expected[v]), "0-1 BFS shortest path reference");
            check(zeroOneBFS(g, s) == actual, "0-1 BFS reuse");
        }
    }
    passed("zero_one_bfs");
}

int main()
{
    test_shortest(); test_mst_zero_one();
}
