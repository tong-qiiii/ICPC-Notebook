#include "code/dp/plug_hamilton_cycle.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(plugHamiltonCycle({"..", ".."}) == 1 &&
           plugHamiltonCycle({"."}) == 0);
    {
        for (int t = 0; t < 400; t++)
        {
            int n = gen() % 3 + 1, m = gen() % 4 + 1;
            vector<string> a(n, string(m, '.'));
            for (auto &r : a)
            {
                for (auto &c : r)
                {
                    if (gen() % 4 == 0)
                    {
                        c = '#';
                    }
                }
            }
            vector<pair<int, int>> p;
            vector<vector<int>> id(n, vector<int>(m, -1));
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    if (a[i][j] != '#')
                    {
                        id[i][j] = p.size();
                        p.push_back({i, j});
                    }
                }
            }
            int N = p.size();
            vector<vector<int>> g(N);
            for (int k = 0; k < N; k++)
            {
                auto [i, j] = p[k];
                for (auto [dx, dy] : vector<pair<int, int>>{
                         {1, 0}, {-1, 0}, {0, 1}, {0, -1}})
                {
                    int x = i + dx, y = j + dy;
                    if (x >= 0 && x < n && y >= 0 && y < m && id[x][y] >= 0)
                    {
                        g[k].push_back(id[x][y]);
                    }
                }
            }
            ll ref = 0;
            auto dfs = [&](auto &&self, int u, int mask) -> void
            {
                if (mask == (1 << N) - 1)
                {
                    if (N >= 4 &&
                        find(g[u].begin(), g[u].end(), 0) != g[u].end())
                    {
                        ref++;
                    }
                    return;
                }
                for (int v : g[u])
                {
                    if (!(mask >> v & 1))
                    {
                        self(self, v, mask | (1 << v));
                    }
                }
            };
            if (N)
            {
                dfs(dfs, 0, 1);
            }
            ref /= 2;
            assert(plugHamiltonCycle(a) == ref);
        }
    }
    cout << "PASS plug_hamilton_cycle\n";
}
