#include "check.hpp"
#include "code/graph/dinic.hpp"
#include "code/graph/min_cost_flow_spfa.hpp"
#include "code/graph/min_cost_flow_potential.hpp"
#include "code/graph/gomory_hu.hpp"
#include "code/graph/stoer_wagner.hpp"
#include "code/graph/hopcroft_karp.hpp"
#include "code/graph/hungarian.hpp"
#include "code/graph/lower_bound_flow.hpp"
#include "code/graph/lower_bound_max_flow.hpp"
#include "code/graph/max_weight_closure.hpp"

void check_reverse(const Dinic& flow)
{
    for (int u = 0; u <= flow.n; ++u)
        for (int i = 0; i < (int)flow.g[u].size(); ++i)
        {
            auto e = flow.g[u][i];
            check(flow.g[e.to][e.rev].to == u && flow.g[e.to][e.rev].rev == i, "Dinic reverse edge/self-loop indices");
        }
}

void test_dinic_cuts()
{
    Dinic sample(2);
    sample.add(1, 1, 7); sample.add(1, 2, 1LL << 61);
    check_reverse(sample);
    check(sample.maxflow(1, 2) == (1LL << 61), "Dinic large flow not silently capped");
    check(sample.maxflow(1, 2) == 0 && sample.maxflow(1, 1) == 0, "Dinic residual reuse/same terminals");
    sample.init(2); sample.add(1, 2, 7);
    check(sample.maxflow(1, 2, 3) == 3 && sample.maxflow(1, 2) == 4, "Dinic limited continuation");
    int extra = sample.add_node(); check(extra == 3 && sample.g.size() == 4, "Dinic dynamic node");
    for (int tc = 0; tc < 180; ++tc)
    {
        int n = rand_int(2, 6);
        Dinic flow(n);
        vector<TestEdge> e;
        for (int i = rand_int(0, 14); i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, n), cap = rand_int(0, 10);
            flow.add(u, v, cap); e.push_back({u, v, cap});
        }
        check_reverse(flow);
        ll expected = cut_brute(n, e, 1, n, false), ans = flow.maxflow(1, n);
        check(ans == expected && flow.maxflow(1, n) == 0, "maxflow exhaustive cut reference/reuse");
        auto side = flow.reachable(1);
        check(side[1] && !side[n], "minimum cut terminal sides");
        ll cut = 0;
        for (auto [u, v, cap] : e) if (side[u] && !side[v]) cut += cap;
        check(cut == ans, "minimum cut witness");
    }
    passed("dinic");

    GomoryHu gh(3);
    ll big = (1LL << 61) + 17;
    gh.add(1, 2, big); gh.add(2, 3, big); gh.add(3, 1, big); gh.build();
    check(gh.query(1, 3) == 2 * big, "Gomory-Hu large cut/uncapped query");
    vector<vector<ll>> matrix(4, vector<ll>(4));
    for (int u = 1; u <= 3; ++u)
        for (int v = 1; v <= 3; ++v) if (u != v) matrix[u][v] = big;
    check(stoer_wagner(matrix) == 2 * big, "Stoer-Wagner large cut");
    check(stoer_wagner(vector<vector<ll>>(1)) == 0, "zero-node global cut");
    check(stoer_wagner(vector<vector<ll>>(2, vector<ll>(2))) == 0, "singleton global cut");
    for (int tc = 0; tc < 100; ++tc)
    {
        int n = rand_int(1, 6);
        gh.init(n);
        matrix.assign(n + 1, vector<ll>(n + 1));
        vector<TestEdge> e;
        for (int i = rand_int(0, 15); i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, n), cap = rand_int(0, 10);
            gh.add(u, v, cap); e.push_back({u, v, cap});
            if (u != v)
            {
                matrix[u][v] += cap; matrix[v][u] += cap;
            }
        }
        gh.build();
        for (int u = 1; u <= n; ++u)
            for (int v = u + 1; v <= n; ++v)
                check(gh.query(u, v) == cut_brute(n, e, u, v), "Gomory-Hu every pair cut reference");
        auto before = gh.tree; gh.build(); check(gh.tree == before, "Gomory-Hu repeated build");
        check(stoer_wagner(matrix) == cut_brute(n, e), "global minimum cut exhaustive reference");
        check(stoer_wagner(matrix) == cut_brute(n, e), "global cut does not mutate matrix");
    }
    passed("gomory_hu"); passed("stoer_wagner");
}

struct CostEdge
{
    int u, v, cap;
    ll cost;
};

pair<ll, ll> cost_brute(int n, const vector<CostEdge>& e, int limit)
{
    vector<int> net(n + 1);
    pair<ll, ll> best{-1, 0};
    auto dfs = [&](auto&& dfs, int i, ll cost) -> void
    {
        if (i == (int)e.size())
        {
            for (int u = 2; u < n; ++u) if (net[u]) return;
            if (net[1] < 0 || net[1] > limit || net[n] != -net[1]) return;
            if (net[1] > best.first || (net[1] == best.first && cost < best.second)) best = {net[1], cost};
            return;
        }
        auto [u, v, cap, w] = e[i];
        for (int f = 0; f <= cap; ++f)
        {
            net[u] += f; net[v] -= f;
            dfs(dfs, i + 1, cost + f * w);
            net[u] -= f; net[v] += f;
        }
    };
    dfs(dfs, 0, 0);
    return best;
}

template <class Flow>
void test_cost_type()
{
    Flow big(2);
    big.add(1, 2, 1LL << 61, 0);
    check(big.flow(1, 2) == pair<ll, ll>{1LL << 61, 0}, "cost flow uncapped large capacity");
    check(big.flow(1, 2) == pair<ll, ll>{0, 0} && big.flow(1, 1) == pair<ll, ll>{0, 0}, "cost flow exhausted/same terminals");
    Flow product(2);
    product.add(1, 2, 1000000, 1000000000000LL);
    check(product.flow(1, 2) == pair<ll, ll>{1000000, 1000000000000000000LL}, "cost flow large nonzero fee product");
    Flow reroute(4);
    for (auto [u, v, cap, cost] : vector<CostEdge>{{1, 2, 1, 0}, {2, 3, 1, -1}, {3, 4, 1, 0}, {1, 3, 1, 0}, {2, 4, 1, 0}})
        reroute.add(u, v, cap, cost);
    check(reroute.flow(1, 4) == pair<ll, ll>{2, 0}, "cost flow reroutes through reverse edges");
    for (int tc = 0; tc < 120; ++tc)
    {
        int n = rand_int(2, 5);
        Flow flow(n);
        vector<CostEdge> e;
        for (int i = rand_int(0, 7); i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, n), cap = rand_int(0, 2), cost = rand_int(-5, 7);
            if (u >= v) continue;
            e.push_back({u, v, cap, cost}); flow.add(u, v, cap, cost);
        }
        int limit = rand_int(0, 4);
        auto expected = cost_brute(n, e, limit), actual = flow.flow(1, n, limit);
        check(actual == expected, "limited minimum-cost flow exhaustive reference");
        auto more = flow.flow(1, n);
        check(pair<ll, ll>{actual.first + more.first, actual.second + more.second} == cost_brute(n, e, 100), "cost flow incremental continuation");
        flow.init(2); flow.add(1, 1, 3, 0); flow.add(1, 2, 2, -7);
        check(flow.flow(1, 2) == pair<ll, ll>{2, -14}, "cost flow reset/self-loop/negative edge");
    }
}

void test_matching()
{
    for (int tc = 0; tc < 150; ++tc)
    {
        int n = rand_int(0, 6), m = rand_int(0, 6);
        HopcroftKarp hk(n, m);
        vector<vector<int>> g(n + 1);
        for (int i = n && m ? rand_int(0, 20) : 0; i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, m);
            hk.add(u, v); g[u].push_back(v);
        }
        auto dfs = [&](auto&& dfs, int u, int used) -> int
        {
            if (u > n) return 0;
            int ans = dfs(dfs, u + 1, used);
            for (int v : g[u])
                if (!(used >> (v - 1) & 1)) ans = max(ans, 1 + dfs(dfs, u + 1, used | (1 << (v - 1))));
            return ans;
        };
        int ans = hk.work();
        check(ans == dfs(dfs, 1, 0) && hk.work() == ans, "Hopcroft-Karp exhaustive reference/reuse");
        int count = 0;
        for (int u = 1; u <= n; ++u)
            if (hk.matchL[u])
            {
                int v = hk.matchL[u]; ++count;
                check(hk.matchR[v] == u && find(g[u].begin(), g[u].end(), v) != g[u].end(), "matching witness");
            }
        check(count == ans, "matching size witness");
    }
    passed("hopcroft_karp");
    check(hungarian({}) == pair<ll, vector<int>>{0, {}}, "empty Hungarian");
    for (int tc = 0; tc < 120; ++tc)
    {
        int n = rand_int(1, 5), m = rand_int(n, 7);
        vector<vector<ll>> a(n, vector<ll>(m));
        for (auto& row : a) for (auto& cost : row) cost = rand_int(-20, 20);
        auto dfs = [&](auto&& dfs, int u, int used) -> ll
        {
            if (u == n) return 0;
            ll ans = LLONG_MAX;
            for (int v = 0; v < m; ++v)
                if (!(used >> v & 1)) ans = min(ans, a[u][v] + dfs(dfs, u + 1, used | (1 << v)));
            return ans;
        };
        auto [ans, match] = hungarian(a);
        check(ans == dfs(dfs, 0, 0) && hungarian(a).first == ans, "Hungarian exhaustive assignment/reuse");
        set<int> used;
        ll sum = 0;
        for (int u = 0; u < n; ++u)
        {
            check(used.insert(match[u]).second, "Hungarian distinct columns"); sum += a[u][match[u]];
        }
        check(sum == ans, "Hungarian assignment witness");
    }
    passed("hungarian");
}

struct BoundEdge
{
    int u, v, low, high;
};

int bound_brute(int n, const vector<BoundEdge>& e, int s = 0, int t = 0)
{
    vector<int> net(n + 1);
    int ans = -1;
    auto dfs = [&](auto&& dfs, int i) -> void
    {
        if (i == (int)e.size())
        {
            for (int u = 1; u <= n; ++u) if (u != s && u != t && net[u]) return;
            if (!s)
            {
                ans = 0; return;
            }
            if (net[s] >= 0 && net[t] == -net[s]) ans = max(ans, net[s]);
            return;
        }
        auto [u, v, low, high] = e[i];
        for (int f = low; f <= high; ++f)
        {
            net[u] += f; net[v] -= f; dfs(dfs, i + 1); net[u] -= f; net[v] += f;
        }
    };
    dfs(dfs, 0);
    return ans;
}

void test_bounds_closure()
{
    LowerBoundFlow empty(0);
    check(empty.work() && empty.work(), "empty bounded circulation/repeated work");
    for (int tc = 0; tc < 130; ++tc)
    {
        int n = rand_int(2, 4);
        LowerBoundFlow flow(n);
        LowerBoundMaxFlow maxflow(n);
        vector<BoundEdge> e;
        for (int i = rand_int(0, 6); i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, n), low = rand_int(0, 2), high = rand_int(low, 3);
            e.push_back({u, v, low, high}); flow.add(u, v, low, high); maxflow.add(u, v, low, high);
        }
        bool ok = flow.work();
        check(ok == (bound_brute(n, e) != -1) && flow.work() == ok, "bounded circulation exhaustive reference/cached work");
        if (ok)
        {
            vector<ll> net(n + 1);
            for (int id = 0; id < (int)e.size(); ++id)
            {
                auto [u, v, low, high] = e[id];
                ll f = flow.edgeFlow(id);
                check(low <= f && f <= high, "bounded edge flow recovery");
                net[u] += f; net[v] -= f;
            }
            check(all_of(net.begin(), net.end(), [](ll w)
            {
                return w == 0; }), "bounded flow conservation");
        }
        ll ans = maxflow.work(1, n);
        check(ans == bound_brute(n, e, 1, n) && maxflow.work(1, n) == ans, "bounded maximum flow exhaustive reference/cached work");
    }
    passed("lower_bound_flow"); passed("lower_bound_max_flow");
    MaxWeightClosure big(2);
    big.addWeight(1, 1LL << 61); big.addWeight(2, -(1LL << 61)); big.addDependency(1, 2);
    check(big.work().first == 0, "closure large-capacity dependency");
    MaxWeightClosure none(0); check(none.work().first == 0, "empty closure");
    for (int tc = 0; tc < 150; ++tc)
    {
        int n = rand_int(1, 8);
        MaxWeightClosure closure(n);
        vector<ll> w(n + 1);
        vector<pair<int, int>> dep;
        for (int u = 1; u <= n; ++u)
        {
            w[u] = rand_int(-20, 20); closure.addWeight(u, w[u]);
        }
        for (int i = rand_int(0, 15); i; --i)
        {
            int u = rand_int(1, n), v = rand_int(1, n);
            dep.push_back({u, v}); closure.addDependency(u, v);
        }
        ll best = 0;
        for (int mask = 0; mask < (1 << n); ++mask)
        {
            bool ok = true;
            for (auto [u, v] : dep) if ((mask >> (u - 1) & 1) && !(mask >> (v - 1) & 1)) ok = false;
            if (!ok) continue;
            ll sum = 0;
            for (int u = 1; u <= n; ++u) if (mask >> (u - 1) & 1) sum += w[u];
            best = max(best, sum);
        }
        auto [ans, chosen] = closure.work();
        check(ans == best && closure.work().first == ans, "maximum closure exhaustive reference/reuse");
        vector<int> take(n + 1);
        ll sum = 0;
        for (int u : chosen)
        {
            take[u] = 1; sum += w[u];
        }
        check(sum == ans, "closure weight witness");
        for (auto [u, v] : dep) check(!take[u] || take[v], "closure dependency witness");
    }
    passed("max_weight_closure");
}

int main()
{
    test_dinic_cuts();
    test_cost_type<MCMF>(); passed("min_cost_flow_spfa");
    test_cost_type<MCMFPotential>(); passed("min_cost_flow_potential");
    test_matching(); test_bounds_closure();
}
