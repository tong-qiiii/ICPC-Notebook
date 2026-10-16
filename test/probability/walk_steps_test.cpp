#include "code/probability/walk_steps.hpp"
#include "code/math/modint.hpp"

int main()
{
    using mint = Mint<998244353>;
    auto game = [](int n, int m, int k)
    {
        vector<vector<pair<int, mint>>> g(n + 2);
        for (int u = 0; u < n; u++)
        {
            for (int d = 1; d <= m; d++)
            {
                int v = u + d;
                if (v > n) v = 2 * n - v;
                g[u + 1].push_back({v + 1, mint(1) / m});
            }
        }
        g[n + 1].push_back({n + 1, 1});
        return walk_steps(g, 1, k)[n + 1];
    };
    assert(game(2, 2, 1).v == 499122177);
    assert(game(10, 5, 6).v == 184124175);
    assert(game(100, 1, 99).v == 0);
    mt19937 gen(1102);
    for (int t = 0; t < 200; t++)
    {
        int n = 1 + gen() % 4, k = gen() % 7;
        vector<vector<pair<int, double>>> g(n + 1);
        for (int u = 1; u <= n; u++)
        {
            g[u] = {{1 + (int)(gen() % n), .25}, {1 + (int)(gen() % n), .75}};
        }
        vector<double> ref(n + 1);
        auto dfs = [&](auto &&self, int u, int len, double p) -> void
        {
            if (!len) { ref[u] += p; return; }
            for (auto [v, w] : g[u]) self(self, v, len - 1, p * w);
        };
        dfs(dfs, 1, k, 1);
        auto ans = walk_steps(g, 1, k);
        for (int u = 1; u <= n; u++) assert(abs(ans[u] - ref[u]) < 1e-12);
    }
    cout << "PASS walk steps: 200 path enumerations, all Sugoroku 4 samples\n";
}
