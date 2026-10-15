#include "check.hpp"
#include "code/graph/scc.hpp"
#include "code/graph/cut_bridge.hpp"
#include "code/graph/ebcc.hpp"
#include "code/graph/vbcc.hpp"
#include "code/graph/two_sat.hpp"
#include "code/graph/forward_star.hpp"
#include "code/graph/dsu.hpp"
#include "code/graph/rollback_dsu.hpp"
#include "code/graph/persistent_dsu.hpp"
#include "code/graph/weighted_dsu.hpp"
#include "code/graph/xor_dsu.hpp"
#include "code/graph/mod_dsu.hpp"
#include "code/graph/extended_domain_dsu.hpp"
#include "code/graph/dsu_reference.hpp"
#include "code/graph/topological_sort.hpp"

void test_components()
{
    SCC empty;
    empty.work();
    check(empty.cnt == 0 && empty.dag().size() == 1, "empty SCC");
    SCC sample(4);
    sample.add(1, 2); sample.add(2, 1); sample.add(2, 3);
    sample.work();
    check(sample.cnt == 3 && sample.bel[1] == sample.bel[2], "SCC sample");
    for (int tc = 0; tc < 180; ++tc)
    {
        int n = rand_int(1, 7);
        SCC scc(n);
        vector<vector<int>> g(n + 1);
        for (int i = rand_int(0, 25); i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, n);
            scc.add(u, v); g[u].push_back(v);
        }
        scc.work();
        auto reach = reachability(g), dag = scc.dag();
        int cnt = scc.cnt;
        check(accumulate(scc.siz.begin(), scc.siz.end(), 0) == n, "SCC sizes");
        check(acyclic(dag), "condensation DAG");
        for (int u = 1; u <= n; ++u)
        {
            for (int v = 1; v <= n; ++v)
                check((scc.bel[u] == scc.bel[v]) == bool(reach[u][v] && reach[v][u]), "SCC reachability reference");
            for (int v : g[u])
                check(scc.bel[u] == scc.bel[v] || scc.bel[u] > scc.bel[v], "Tarjan reverse numbering");
        }
        for (auto a : dag)
            check(set<int>(a.begin(), a.end()).size() == a.size(), "DAG deduplication");
        scc.work(); check(scc.cnt == cnt, "SCC repeated work");
        scc.init(1); scc.work(); check(scc.cnt == 1, "SCC reset");
    }
    passed("scc");

    CutBridge empty_cut;
    empty_cut.work(); check(empty_cut.bridges.empty(), "empty CutBridge");
    EBCC empty_edge;
    empty_edge.work(); check(empty_edge.cnt == 0, "empty EBCC");
    for (int tc = 0; tc < 180; ++tc)
    {
        int n = rand_int(1, 7);
        CutBridge cb(n);
        EBCC ebcc(n);
        vector<pair<int, int>> e;
        for (int i = rand_int(0, 14); i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, n);
            e.push_back({u, v}); cb.add(u, v); ebcc.add(u, v);
        }
        cb.work(); ebcc.work();
        int base = components(n, e), bridges = 0;
        vector<pair<int, int>> keep;
        set<pair<int, int>> expected;
        for (int id = 0; id < (int)e.size(); ++id)
        {
            bool bridge = components(n, e, id) > base;
            check(bool(ebcc.is_bridge[id]) == bridge, "bridge deletion reference");
            if (bridge)
            {
                ++bridges;
                expected.insert(minmax(e[id].first, e[id].second));
            }
            else keep.push_back(e[id]);
        }
        set<pair<int, int>> actual;
        for (auto [u, v] : cb.bridges) actual.insert(minmax(u, v));
        check(actual == expected, "CutBridge bridge set");
        for (int u = 1; u <= n; ++u)
            check(bool(cb.is_cut[u]) == (components(n, e, -1, u) > base), "cut vertex deletion reference");
        auto bel = component_ids(n, keep);
        for (int u = 1; u <= n; ++u)
            for (int v = 1; v <= n; ++v)
                check((ebcc.bel[u] == ebcc.bel[v]) == (bel[u] == bel[v]), "EBCC components");
        auto tree = ebcc.bridge_tree();
        int degree = 0;
        for (auto a : tree) degree += (int)a.size();
        check(degree == 2 * bridges && ebcc.cnt == bridges + base, "bridge forest size");
        cb.work(); ebcc.work();
        check(accumulate(ebcc.siz.begin(), ebcc.siz.end(), 0) == n, "EBCC repeated work");
        cb.init(1); ebcc.init(1); cb.work(); ebcc.work();
        check(!cb.is_cut[1] && ebcc.cnt == 1, "bridge reset");
    }
    passed("cut_bridge"); passed("ebcc");

    VBCC vb(5);
    for (auto [u, v] : vector<pair<int, int>>{{1, 2}, {2, 3}, {3, 1}, {3, 4}, {4, 5}, {5, 3}}) vb.add(u, v);
    vb.work(); check(vb.cnt == 2 && vb.is_cut[3], "VBCC shared articulation sample");
    for (int tc = 0; tc < 180; ++tc)
    {
        int n = rand_int(1, 7);
        vb.init(n);
        vector<pair<int, int>> e;
        for (int i = rand_int(0, 14); i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, n);
            if (u == v) continue;
            vb.add(u, v); e.push_back({u, v});
        }
        vb.work();
        int base = components(n, e);
        vector<int> count(n + 1);
        for (int id = 1; id <= vb.cnt; ++id)
        {
            set<int> block(vb.bcc[id].begin(), vb.bcc[id].end());
            check(block.size() == vb.bcc[id].size(), "VBCC duplicate vertex");
            for (int u : block) ++count[u];
        }
        for (int u = 1; u <= n; ++u)
        {
            bool cut = components(n, e, -1, u) > base;
            check(bool(vb.is_cut[u]) == cut, "VBCC articulation reference");
            check(cut ? count[u] >= 2 : count[u] == 1, "VBCC memberships and isolated vertex");
        }
        for (auto [u, v] : e)
        {
            int count_edge = 0;
            for (int id = 1; id <= vb.cnt; ++id)
            {
                auto& block = vb.bcc[id];
                count_edge += find(block.begin(), block.end(), u) != block.end() && find(block.begin(), block.end(), v) != block.end();
            }
            check(count_edge == 1, "each edge belongs to one VBCC");
        }
        auto tree = vb.block_tree();
        vector<pair<int, int>> edges;
        for (int u = 1; u < (int)tree.size(); ++u)
            for (int v : tree[u]) if (u < v) edges.push_back({u, v});
        check((int)edges.size() == n + vb.cnt - base, "block tree forest");
        int cnt = vb.cnt; vb.work(); check(cnt == vb.cnt, "VBCC repeated work");
    }
    passed("vbcc");
}

void test_sat_topo()
{
    TwoSAT none;
    check(none.satisfiable(), "zero variable SAT");
    TwoSAT one(1);
    one.force(1, true); check(one.satisfiable() && one.assignment[1], "SAT force");
    one.force(1, false); check(!one.satisfiable(), "SAT contradiction");
    one.init(2); one.eq(1, 2); one.force(1, true);
    check(one.satisfiable() && one.assignment[2], "SAT equality");
    one.init(2); one.neq(1, 2); one.force(1, true);
    check(one.satisfiable() && !one.assignment[2], "SAT inequality");
    for (int tc = 0; tc < 180; ++tc)
    {
        int n = rand_int(1, 5);
        TwoSAT sat(n);
        vector<array<int, 4>> clauses;
        for (int i = rand_int(0, 18); i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, n), a = rand_int(0, 1), b = rand_int(0, 1);
            sat.either(u, a, v, b); clauses.push_back({u, a, v, b});
        }
        bool expected = false;
        for (int mask = 0; mask < (1 << n); ++mask)
        {
            bool ok = true;
            for (auto [u, a, v, b] : clauses)
                ok &= (mask >> (u - 1) & 1) == a || (mask >> (v - 1) & 1) == b;
            expected |= ok;
        }
        bool ok = sat.satisfiable();
        check(ok == expected && sat.satisfiable() == ok, "SAT exhaustive reference/reuse");
        if (ok)
            for (auto [u, a, v, b] : clauses)
                check(sat.assignment[u] == a || sat.assignment[v] == b, "SAT assignment witness");
    }
    passed("two_sat");

    ForwardStar fs(2, 4);
    check(fs.add(1, 2, 7) == 0 && fs.add(1, 1, -3) == 1, "forward star IDs");
    check(fs.head[1] == 1 && fs.e[1].nxt == 0 && fs.e[0].nxt == -1, "forward star reverse insertion");
    fs.init(1); check(fs.e.empty() && fs.head[1] == -1 && fs.add(1, 1) == 0, "forward star reset");
    passed("forward_star");
    check(topo(vector<vector<int>>(1)).empty(), "zero node topo");
    for (int tc = 0; tc < 180; ++tc)
    {
        int n = rand_int(1, 7);
        vector<vector<int>> g(n + 1);
        for (int i = rand_int(0, 20); i; --i) g[rand_int(1, n)].push_back(rand_int(1, n));
        auto order = topo(g);
        check(((int)order.size() == n) == acyclic(g), "topo cycle reference");
        check(topo(g) == order, "topo repeat without degree mutation");
        if (!order.empty())
        {
            vector<int> pos(n + 1);
            for (int i = 0; i < n; ++i) pos[order[i]] = i;
            for (int u = 1; u <= n; ++u)
                for (int v : g[u]) check(pos[u] < pos[v], "topological order witness");
        }
    }
    passed("topological_sort");
}

void test_dsu()
{
    DSU dsu(8);
    vector<int> bel(9);
    iota(bel.begin(), bel.end(), 0);
    for (int i = 0; i < 200; ++i)
    {
        int u = rand_int(1, 8), v = rand_int(1, 8);
        bool changed = bel[u] != bel[v];
        check(dsu.merge(u, v) == changed, "DSU merge result");
        int old = bel[u], now = bel[v];
        for (int j = 1; j <= 8; ++j) if (bel[j] == old) bel[j] = now;
        for (int a = 1; a <= 8; ++a)
        {
            int cnt = 0;
            for (int b = 1; b <= 8; ++b)
            {
                cnt += bel[a] == bel[b];
                check(dsu.same(a, b) == (bel[a] == bel[b]), "DSU connectivity reference");
            }
            check(dsu.size(a) == cnt, "DSU size reference");
        }
    }
    dsu.init(1); check(dsu.size(1) == 1 && !dsu.merge(1, 1), "DSU singleton/reset");
    passed("dsu");

    RollbackDSU rollback(8);
    iota(bel.begin(), bel.end(), 0);
    vector<vector<int>> history{bel};
    for (int i = 0; i < 400; ++i)
    {
        if (history.size() > 1 && rand_int(0, 2) == 0)
        {
            int snap = rand_int(0, (int)history.size() - 1);
            rollback.rollback(snap); history.resize(snap + 1);
        }
        else
        {
            int u = rand_int(1, 8), v = rand_int(1, 8);
            auto state = history.back();
            check(rollback.merge(u, v) == (state[u] != state[v]), "rollback merge");
            int old = state[u], now = state[v];
            for (int j = 1; j <= 8; ++j) if (state[j] == old) state[j] = now;
            history.push_back(state);
        }
        check(rollback.snapshot() == (int)history.size() - 1, "rollback records no-op merge");
        for (int u = 1; u <= 8; ++u)
        {
            int cnt = 0;
            for (int v = 1; v <= 8; ++v)
            {
                cnt += history.back()[u] == history.back()[v];
                check(rollback.same(u, v) == (history.back()[u] == history.back()[v]), "rollback saved-state reference");
            }
            check(rollback.size(u) == cnt, "rollback size restoration");
        }
    }
    rollback.rollback(0); rollback.merge(1, 1); rollback.undo();
    check(rollback.snapshot() == 0, "rollback empty state/no-op undo");
    passed("rollback_dsu");

    PDSU persistent(8);
    vector<int> roots{persistent.base};
    iota(bel.begin(), bel.end(), 0);
    history = {bel};
    for (int i = 0; i < 600; ++i)
    {
        int version = rand_int(0, (int)roots.size() - 1);
        int u = rand_int(1, 8), v = rand_int(1, 8);
        auto state = history[version];
        int old = state[u], now = state[v];
        for (int j = 1; j <= 8; ++j) if (state[j] == old) state[j] = now;
        roots.push_back(persistent.merge(roots[version], u, v)); history.push_back(state);
        for (int trial = 0; trial < 5; ++trial)
        {
            int k = rand_int(0, (int)roots.size() - 1), a = rand_int(1, 8), b = rand_int(1, 8);
            check(persistent.same(roots[k], a, b) == (history[k][a] == history[k][b]), "persistent branching versions");
        }
    }
    persistent.init(1); check(persistent.same(persistent.base, 1, 1), "persistent reset/singleton");
    passed("persistent_dsu");
}

void test_weighted()
{
    for (int tc = 0; tc < 100; ++tc)
    {
        WeightedDSU add(8);
        XorDSU bit(8);
        int k = rand_int(1, 20);
        ModDSU mod(8, k);
        vector<ll> val(9), parity(9);
        for (int i = 1; i <= 8; ++i)
        {
            val[i] = rand_int(-100, 100); parity[i] = rand_int(0, 63);
        }
        for (int i = 0; i < 50; ++i)
        {
            int u = rand_int(1, 8), v = rand_int(1, 8);
            check(add.merge(u, v, val[v] - val[u]), "weighted valid merge");
            check(bit.merge(u, v, parity[u] ^ parity[v]), "XOR valid merge");
            check(mod.merge(u, v, val[v] - val[u]), "mod valid merge");
            for (int a = 1; a <= 8; ++a)
                for (int b = 1; b <= 8; ++b)
                    if (add.same(a, b))
                    {
                        check(add.diff(a, b) == val[b] - val[a], "weighted potential direction");
                        check(bit.diff(a, b) == (parity[a] ^ parity[b]), "XOR compressed relation");
                        ll expected = (val[b] - val[a]) % k;
                        if (expected < 0) expected += k;
                        check(mod.diff(a, b) == expected, "mod independent potential reference");
                    }
            check(!add.merge(u, v, val[v] - val[u] + 1), "weighted contradiction");
            check(!bit.merge(u, v, (parity[u] ^ parity[v]) ^ 1), "XOR contradiction");
            if (k > 1) check(!mod.merge(u, v, val[v] - val[u] + 1), "mod contradiction");
        }
        add.init(1); bit.init(1); mod.init(1, 2);
        check(add.diff(1, 1) == 0 && bit.diff(1, 1) == 0 && mod.diff(1, 1) == 0, "weighted variants reset");
    }
    ModDSU wide(2, INT_MAX);
    check(wide.merge(1, 2, LLONG_MIN) && wide.diff(1, 2) == INT_MAX - 2LL, "mod minimum input normalization");
    ModDSU modular(2, 3);
    check(modular.merge(1, 2, 1) && modular.merge(1, 2, 4), "mod equivalent constraints");
    check(!modular.merge(1, 2, 2) && modular.diff(1, 2) == 1, "mod contradiction preserves relation");
    ModDSU large(4, INT_MAX);
    large.merge(1, 2, INT_MAX - 1LL); large.merge(3, 4, 0);
    check(large.merge(3, 1, INT_MAX - 1LL) && large.diff(3, 2) == INT_MAX - 2LL, "mod intermediate sum exceeds int range");
    WeightedDSU additive(3);
    additive.merge(1, 2, 1LL << 60); additive.merge(1, 3, -(1LL << 60));
    check(additive.diff(2, 3) == -(1LL << 61), "weighted large signed relation");
    passed("weighted_dsu"); passed("xor_dsu"); passed("mod_dsu");

    DSU domain(6);
    check(merge_domain(domain, 3, 1, 2, true), "same domain");
    check(merge_domain(domain, 3, 2, 3, false), "different domain");
    check(!merge_domain(domain, 3, 1, 3, true), "domain contradiction");
    check(merge_domain(domain, 3, 1, 3, false), "domain repeat");
    check(!merge_domain(domain, 3, 1, 1, false), "domain self contradiction");
    passed("extended_domain_dsu");

    int S = 4;
    WeightedDSU reference(4);
    check(!get_value(reference, S, 1), "unfixed reference value");
    check(reference.merge(S, 1, 5) && reference.merge(1, 2, -3), "reference constraints");
    check(reference.find(S) != S, "reference need not remain root");
    check(get_value(reference, S, 1) == 5 && get_value(reference, S, 2) == 2, "reference absolute values");
    check(!reference.merge(S, 2, 3) && !get_value(reference, S, 3), "reference contradictory/unfixed value");
    passed("dsu_reference");
}

int main()
{
    test_components(); test_sat_topo(); test_dsu(); test_weighted();
}
