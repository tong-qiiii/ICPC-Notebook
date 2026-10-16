#include "code/probability/dag_expect.hpp"

int main()
{
    mt19937 gen(1103);
    for (int t = 0; t < 200; t++)
    {
        int n = 2 + gen() % 8;
        vector<vector<pair<int, double>>> g(n + 1);
        vector<double> cost(n + 1), ref(n + 1);
        for (int u = n - 1; u >= 1; u--)
        {
            int v = u + 1 + gen() % (n - u);
            g[u] = {{u, .5}, {u + 1, .25}, {v, .25}};
            cost[u] = gen() % 11;
            ref[u] = 2 * cost[u] + (ref[u + 1] + ref[v]) / 2;
        }
        auto ans = dag_expect(g, cost);
        for (int u = 1; u <= n; u++) assert(abs(ans[u] - ref[u]) < 1e-10);
    }
    vector<vector<pair<int, double>>> g(2);
    g[1] = {{1, 1}};
    bool caught = false;
    try { dag_expect(g, vector<double>{0, 1}); }
    catch (const domain_error &) { caught = true; }
    assert(caught);
    g.resize(3); g[1] = {{2, 1}}; g[2] = {{1, 1}};
    caught = false;
    try { dag_expect(g, vector<double>{0, 1, 1}); }
    catch (const domain_error &) { caught = true; }
    assert(caught);
    g[2].clear(); g[1] = {{1, 1 - 1e-12}, {2, 1e-12}};
    auto ans = dag_expect(g, vector<double>{0, 1, 0});
    assert(abs(ans[1] / 1e12 - 1) < 1e-12);
    cout << "PASS DAG expectation: random self-loops, cycles rejected, tiny exit and reuse\n";
}
