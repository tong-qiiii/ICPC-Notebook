#include "code/math/game_sg.hpp"

int main()
{
    mt19937 rg(736);
    assert(nim_move({}).first == -1);
    for (int z = 0; z < 1000; z++)
    {
        vector<ll> a(rg() % 10);
        ll s = 0;
        for (ll &x : a) s ^= (x = rg() % 100);
        auto [i, x] = nim_move(a);
        if (!s) assert(i == -1);
        else assert(i >= 0 && x < a[i] && (s ^ a[i] ^ x) == 0);
        vector<int> moves;
        for (int j = 1; j <= 8; j++) if (rg() % 2) moves.push_back(j);
        auto f = sg_subtract(100, moves);
        vector<int> brute(101);
        for (int j = 1; j <= 100; j++)
        {
            set<int> seen;
            for (int k : moves) if (k <= j) seen.insert(brute[j - k]);
            while (seen.count(brute[j])) brute[j]++;
        }
        assert(f == brute);
        int n = 20;
        vector<vector<int>> g(n + 1);
        vector<int> expected(n + 1);
        for (int u = n; u; u--)
        {
            set<int> seen;
            for (int v = u + 1; v <= n; v++) if (rg() % 3 == 0)
                g[u].push_back(v), seen.insert(expected[v]);
            while (seen.count(expected[u])) expected[u]++;
        }
        assert(sg_dag(g) == expected);
    }
    cout << "PASS game_sg: mex brute, Nim moves, random DAG\n";
}
