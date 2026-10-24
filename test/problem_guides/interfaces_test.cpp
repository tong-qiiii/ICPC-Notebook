#include <bits/stdc++.h>
#include "code/tree/hld.hpp"
#include "code/tree/virtual_tree.hpp"
#include "code/tree/dsu_on_tree.hpp"
#include "code/tree/kruskal_tree.hpp"
#include "code/tree/centroid.hpp"
#include "code/dp/reroot_distance.hpp"
#include "code/graph/difference_constraints.hpp"
#include "code/graph/two_sat.hpp"
#include "code/graph/hopcroft_karp.hpp"
#include "code/graph/lower_bound_flow.hpp"
#include "code/graph/min_cost_flow_potential.hpp"
#include "code/string/kmp.hpp"
#include "code/string/manacher.hpp"
#include "code/string/aho_corasick.hpp"
#include "code/string/palindromic_tree.hpp"
#include "code/string/suffix_automaton.hpp"
#include "code/string/lcp_rmq.hpp"
#include "code/string/minimum_rotation.hpp"
using namespace std;

mt19937 rng(180034);

int rnd(int l, int r)
{
    return uniform_int_distribution<int>(l, r)(rng);
}

void tree_test()
{
    for (int rep = 0; rep < 100; rep++)
    {
        int n = rnd(1, 16);
        HLD h(n);
        DSUOnTree dsu(n);
        VTree vt(n);
        Centroid ct(n, 3);
        vector<vector<pair<int, ll>>> g(n + 1);
        vector<int> colors(n + 1);
        for (int v = 1; v <= n; v++)
        {
            colors[v] = rnd(1, 4);
            dsu.set_color(v, colors[v]);
            if (v > 1)
            {
                int p = rnd(1, v - 1);
                h.add(p, v);
                vt.add(p, v);
                dsu.add(p, v);
                ct.add(p, v, 1);
                g[p].push_back({v, 1});
                g[v].push_back({p, 1});
            }
        }
        h.build(1);
        vt.prep(1);
        auto colors_ans = dsu.work(1);
        assert(dsu.work(1) == colors_ans);
        auto ans = reroot_distance(g);
        ll pairs = 0;
        for (int u = 1; u <= n; u++)
        {
            vector<int> dis(n + 1, -1);
            queue<int> q;
            q.push(u);
            dis[u] = 0;
            while (!q.empty())
            {
                int x = q.front(); q.pop();
                for (auto [y, w] : g[x])
                {
                    if (dis[y] == -1)
                    {
                        dis[y] = dis[x] + 1;
                        q.push(y);
                    }
                }
            }
            assert(ans[u] == accumulate(dis.begin() + 1, dis.end(), 0LL));
            set<int> seen;
            auto [l, r] = h.subtree(u);
            for (int v = 1; v <= n; v++)
            {
                if (l <= h.dfn[v] && h.dfn[v] <= r)
                {
                    seen.insert(colors[v]);
                }
                pairs += u < v && dis[v] <= 3;
                int path_len = 0;
                h.path(u, v, [&](int a, int b) { path_len += b - a + 1; });
                assert(path_len == dis[v] + 1);
                int edge_len = 0;
                h.path_edge(u, v, [&](int a, int b) { edge_len += b - a + 1; });
                assert(edge_len == dis[v]);
            }
            assert(colors_ans[u] == (int)seen.size());
        }
        assert(ct.work() == pairs && ct.work() == pairs);
        assert(vt.build({}) == 0);
        int root = vt.build({1, n, n});
        assert(root == 1);
        assert(vt.nodes.size() == (n == 1 ? 1U : 2U));
        assert(vt.build({}) == 0);
        h.build(1);
        assert(h.timer == n);
    }
    for (int rep = 0; rep < 60; rep++)
    {
        int n = rnd(2, 9);
        KRT krt(n);
        vector<vector<int>> d(n + 1, vector<int>(n + 1, 1000000));
        for (int i = 1; i <= n; i++)
        {
            d[i][i] = -1000000;
        }
        for (int i = 0; i < 16; i++)
        {
            int u = rnd(1, n), v = rnd(1, n), w = rnd(-4, 9);
            krt.add(u, v, w);
            if (u != v)
            {
                d[u][v] = d[v][u] = min(d[u][v], w);
            }
        }
        krt.build();
        for (int k = 1; k <= n; k++)
        {
            for (int i = 1; i <= n; i++)
            {
                for (int j = 1; j <= n; j++)
                {
                    d[i][j] = min(d[i][j], max(d[i][k], d[k][j]));
                }
            }
        }
        for (int i = 1; i <= n; i++)
        {
            for (int j = i + 1; j <= n; j++)
            {
                assert(krt.connected(i, j) == (d[i][j] < 1000000));
                if (krt.connected(i, j))
                {
                    assert(krt.bottleneck(i, j) == d[i][j]);
                }
            }
            for (int t = -4; t <= 9; t++)
            {
                int want = 1;
                for (int j = 1; j <= n; j++)
                {
                    if (j != i && d[i][j] <= t)
                    {
                        want++;
                    }
                }
                assert(krt.siz[krt.climb(i, t)] == want);
            }
        }
    }
}

void graph_test()
{
    DiffCons diff(2);
    diff.leq(2, 1, -3);
    diff.leq(1, 2, 5);
    auto [ok, values] = diff.solve();
    assert(ok && 3 <= values[2] - values[1] && values[2] - values[1] <= 5);
    diff.leq(1, 2, 2);
    assert(!diff.solve().first);
    for (int rep = 0; rep < 100; rep++)
    {
        int n = rnd(1, 6);
        TwoSAT sat(n);
        vector<array<int, 4>> clauses;
        for (int j = 0; j < 10; j++)
        {
            int u = rnd(1, n), v = rnd(1, n), a = rnd(0, 1), b = rnd(0, 1);
            clauses.push_back({u, a, v, b});
            sat.either(u, a, v, b);
        }
        auto valid = [&](int mask)
        {
            for (auto [u, a, v, b] : clauses)
            {
                if (((mask >> (u - 1)) & 1) != a && ((mask >> (v - 1)) & 1) != b)
                {
                    return false;
                }
            }
            return true;
        };
        bool brute = false;
        for (int mask = 0; mask < (1 << n); mask++)
        {
            brute |= valid(mask);
        }
        assert(sat.satisfiable() == brute);
        if (brute)
        {
            int mask = 0;
            for (int u = 1; u <= n; u++)
            {
                mask |= sat.assignment[u] << (u - 1);
            }
            assert(valid(mask));
        }
        assert(sat.satisfiable() == brute);
        HopcroftKarp hk(n, n);
        vector<pair<int, int>> edges;
        for (int u = 1; u <= n; u++)
        {
            for (int v = 1; v <= n; v++)
            {
                if (rnd(0, 2) == 0)
                {
                    hk.add(u, v);
                    edges.push_back({u, v});
                }
            }
        }
        int matched = hk.work();
        auto [left, right] = hk.cover();
        assert((int)(left.size() + right.size()) == matched);
        for (auto [u, v] : edges)
        {
            assert(find(left.begin(), left.end(), u) != left.end() ||
                   find(right.begin(), right.end(), v) != right.end());
        }
    }
    LowerBoundFlow bounded(2);
    int a = bounded.add(1, 2, 1, 2);
    int b = bounded.add(2, 1, 0, 2);
    assert(bounded.work() && bounded.work());
    assert(bounded.edgeFlow(a) == bounded.edgeFlow(b));
    for (int rep = 0; rep < 60; rep++)
    {
        int n = rnd(1, 5), s = 2 * n + 1, t = s + 1;
        MCMFPotential flow(t);
        vector<vector<int>> cost(n, vector<int>(n));
        for (int i = 0; i < n; i++)
        {
            flow.add(s, i + 1, 1, 0);
            flow.add(n + i + 1, t, 1, 0);
            for (int j = 0; j < n; j++)
            {
                cost[i][j] = rnd(-5, 8);
                flow.add(i + 1, n + j + 1, 1, cost[i][j]);
            }
        }
        vector<int> p(n);
        iota(p.begin(), p.end(), 0);
        int best = INT_MAX;
        do
        {
            int sum = 0;
            for (int i = 0; i < n; i++)
            {
                sum += cost[i][p[i]];
            }
            best = min(best, sum);
        }
        while (next_permutation(p.begin(), p.end()));
        auto [f, c] = flow.flow(s, t, n);
        assert(f == n && c == best);
    }
}

void string_test()
{
    for (int rep = 0; rep < 180; rep++)
    {
        string s;
        for (int i = 0, n = rnd(1, 12); i < n; i++)
        {
            s += char('a' + rnd(0, 2));
        }
        map<string, int> all, pals;
        for (int i = 0; i < (int)s.size(); i++)
        {
            for (int j = i + 1; j <= (int)s.size(); j++)
            {
                string x = s.substr(i, j - i), rev = x;
                all[x]++;
                reverse(rev.begin(), rev.end());
                if (rev == x)
                {
                    pals[x]++;
                }
            }
        }
        SAM sam;
        for (char c : s)
        {
            sam.extend(c);
        }
        sam.calc_occ();
        assert(sam.distinct() == (ll)all.size());
        sam.calc_occ();
        AC ac;
        vector<string> patterns;
        for (auto [pattern, count] : all)
        {
            assert(sam.count(pattern) == count);
            assert((int)kmp_match(s, pattern).size() == count);
            ac.insert(pattern);
            patterns.push_back(pattern);
        }
        ac.build();
        auto counts = ac.count(s);
        for (int i = 0; i < (int)patterns.size(); i++)
        {
            assert(counts[i] == all[patterns[i]]);
        }
        PAM pam;
        pam.build(s);
        pam.calc_occ();
        assert(pam.distinct() == (int)pals.size());
        ll total = 0;
        for (int u = 2; u < (int)pam.tr.size(); u++)
        {
            total += pam.tr[u].occ;
        }
        Manacher man(s);
        assert(total == accumulate(man.d1.begin(), man.d1.end(), 0LL) +
                        accumulate(man.d2.begin(), man.d2.end(), 0LL));
        LCPRMQ lcp(s);
        for (int i = 0; i < (int)s.size(); i++)
        {
            for (int j = 0; j < (int)s.size(); j++)
            {
                int len = 0;
                while (i + len < (int)s.size() && j + len < (int)s.size() && s[i + len] == s[j + len])
                {
                    len++;
                }
                assert(lcp.query(i, j) == len);
            }
        }
        int p = minRepresentation(s);
        string best = s.substr(p) + s.substr(0, p);
        for (int i = 0; i < (int)s.size(); i++)
        {
            assert(best <= s.substr(i) + s.substr(0, i));
        }
    }
}

int main()
{
    tree_test();
    graph_test();
    string_test();
    cout << "PASS C++17 guide interfaces and small random comparisons\n";
}
