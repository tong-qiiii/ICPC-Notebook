#include "check.hpp"
#include "code/graph/euler_directed.hpp"
#include "code/graph/euler_undirected.hpp"
#include "code/graph/segment_graph.hpp"
#include "code/graph/jump_graph.hpp"
#include "code/graph/dijkstra.hpp"

bool euler_brute(int n, const vector<pair<int, int>>& e, bool directed)
{
    if (!n) return false;
    auto dfs = [&](auto&& dfs, int u, int mask) -> bool
    {
        if (mask == (1 << e.size()) - 1) return true;
        for (int i = 0; i < (int)e.size(); ++i)
        {
            if (mask >> i & 1) continue;
            auto [a, b] = e[i];
            if (a == u && dfs(dfs, b, mask | (1 << i))) return true;
            if (!directed && b == u && dfs(dfs, a, mask | (1 << i))) return true;
        }
        return false;
    };
    for (int s = 1; s <= n; ++s) if (dfs(dfs, s, 0)) return true;
    return false;
}

void check_tour(const vector<int>& path, const vector<pair<int, int>>& e, bool directed)
{
    check(path.size() == e.size() + 1, "Euler path length");
    vector<int> used(e.size());
    for (int i = 1; i < (int)path.size(); ++i)
    {
        bool found = false;
        for (int id = 0; id < (int)e.size(); ++id)
        {
            auto [u, v] = e[id];
            if (!used[id] && ((u == path[i - 1] && v == path[i]) || (!directed && v == path[i - 1] && u == path[i])))
            {
                used[id] = 1; found = true; break;
            }
        }
        check(found, "Euler each edge used once");
    }
}

void test_euler()
{
    check(EulerDir().work().empty() && EulerUndir().work().empty(), "zero-node Euler");
    check(EulerDir(1).work() == vector<int>{1} && EulerUndir(1).work() == vector<int>{1}, "zero-edge Euler");
    for (int tc = 0; tc < 180; ++tc)
    {
        int n = rand_int(1, 5);
        EulerDir directed(n);
        EulerUndir undirected(n);
        vector<pair<int, int>> e;
        for (int i = rand_int(0, 7); i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, n);
            e.push_back({u, v}); directed.add(u, v); undirected.add(u, v);
        }
        auto a = directed.work(), b = undirected.work();
        check(!a.empty() == euler_brute(n, e, true), "directed Euler exhaustive reference");
        check(!b.empty() == euler_brute(n, e, false), "undirected Euler exhaustive reference");
        if (!a.empty()) check_tour(a, e, true);
        if (!b.empty()) check_tour(b, e, false);
        check(directed.work() == a && undirected.work() == b, "Euler does not consume original graph");
        directed.init(1); undirected.init(1);
        check(directed.work() == vector<int>{1} && undirected.work() == vector<int>{1}, "Euler reset");
    }
    passed("euler_directed"); passed("euler_undirected");
}

void test_segment_graph()
{
    SegGraph empty;
    check(empty.tot == 0 && empty.g.size() == 1, "empty segment graph");
    for (int tc = 0; tc < 100; ++tc)
    {
        int n = rand_int(1, 8);
        SegGraph sg(n);
        check(sg.tot == 3 * n - 2, "segment graph leaf reuse");
        vector<vector<pair<int, ll>>> g(n + 1);
        for (int i = 0; i < 15; ++i)
        {
            int L = rand_int(1, n), R = rand_int(L, n), u = rand_int(1, n), w = rand_int(0, 10);
            int kind = rand_int(0, 3);
            if (kind == 0)
            {
                sg.p2p(u, L, w); g[u].push_back({L, w});
            }
            else if (kind == 1)
            {
                sg.p2r(u, L, R, w);
                for (int v = L; v <= R; ++v) g[u].push_back({v, w});
            }
            else if (kind == 2)
            {
                sg.r2p(L, R, u, w);
                for (int v = L; v <= R; ++v) g[v].push_back({u, w});
            }
            else
            {
                int A = rand_int(1, n), B = rand_int(A, n), old = sg.tot;
                sg.r2r(L, R, A, B, w);
                check(sg.tot == old + 1, "r2r creates one intermediate node");
                for (int a = L; a <= R; ++a)
                    for (int b = A; b <= B; ++b) g[a].push_back({b, w});
            }
        }
        for (int s = 1; s <= n; ++s)
        {
            auto actual = dijkstra(sg.g, s), expected = dijkstra(g, s);
            for (int v = 1; v <= n; ++v) check(actual[v] == expected[v], "range edges equal explicit expansion");
        }
        sg.init(1); check(sg.tot == 1 && sg.g[1].empty(), "segment graph reset");
    }
    passed("segment_graph");
}

vector<int> tree_path(const vector<vector<int>>& tree, int s, int t)
{
    vector<int> pre(tree.size(), -1);
    queue<int> q;
    pre[s] = 0; q.push(s);
    while (!q.empty())
    {
        int u = q.front(); q.pop();
        for (int v : tree[u])
            if (pre[v] == -1)
            {
                pre[v] = u; q.push(v);
            }
    }
    vector<int> path;
    for (int u = t; u; u = pre[u])
    {
        path.push_back(u);
        if (u == s) break;
    }
    reverse(path.begin(), path.end());
    return path;
}

void test_jump_graph()
{
    for (int tc = 0; tc < 90; ++tc)
    {
        int n = rand_int(1, 8);
        JumpGraph jg(n);
        vector<vector<int>> tree(n + 1), g(n + 1);
        for (int u = 2; u <= n; ++u)
        {
            int v = rand_int(1, u - 1);
            jg.add_tree(u, v); tree[u].push_back(v); tree[v].push_back(u);
        }
        int root = rand_int(1, n);
        jg.build(root);
        for (int u = 1; u <= n; ++u)
        {
            auto path = tree_path(tree, u, root);
            for (int k = 0; k < (int)path.size(); ++k) check(jg.kth(u, k) == path[k], "jump ancestor reference");
            for (int v = 1; v <= n; ++v)
            {
                auto p = tree_path(tree, u, v);
                if (u != v) check(jg.next_on_path(u, v) == p[1], "next_on_path reference");
                auto q = tree_path(tree, v, root);
                int expected = 0;
                for (int a : path)
                    if (find(q.begin(), q.end(), a) != q.end())
                    {
                        expected = a; break;
                    }
                check(jg.lca(u, v) == expected, "jump LCA reference");
            }
        }
        for (int i = 0; i < 12; ++i)
        {
            int u = rand_int(1, n), v = rand_int(1, n), point = rand_int(1, n), type = rand_int(0, 2);
            auto path = tree_path(tree, u, v);
            if (type == 0)
            {
                jg.p2p(u, v); g[u].push_back(v);
            }
            else if (type == 1)
            {
                jg.p2path(point, u, v);
                for (int a : path) g[point].push_back(a);
            }
            else
            {
                jg.path2p(u, v, point);
                for (int a : path) g[a].push_back(point);
            }
        }
        auto actual = reachability(jg.g), expected = reachability(g);
        for (int u = 1; u <= n; ++u)
            for (int v = 1; v <= n; ++v) check(actual[u][v] == expected[u][v], "tree-path edges equal explicit expansion");
        auto order = jg.topo();
        check(((int)order.size() == n) == acyclic(g), "jump graph dependency cycles/self loop");
        check(jg.topo() == order, "jump graph topo repeated");
        jg.build(root); check((int)jg.topo().size() == n, "build clears old dependencies");
    }
    passed("jump_graph");
}

int main()
{
    test_euler(); test_segment_graph(); test_jump_graph();
}
