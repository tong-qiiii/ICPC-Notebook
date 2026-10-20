#include "code/math/determinant.hpp"

int main()
{
    mt19937 rg(738);
    for (int z = 0; z < 500; z++)
    {
        int n = rg() % 7, mod = (z % 2 ? 2 : 101);
        vector<vector<ll>> a(n, vector<ll>(n));
        for (auto &v : a) for (ll &x : v) x = int(rg() % 21) - 10;
        vector<int> p(n);
        iota(p.begin(), p.end(), 0);
        ll brute = 0;
        do
        {
            ll val = 1;
            int inv = 0;
            for (int i = 0; i < n; i++)
            {
                val = val * a[i][p[i]] % mod;
                for (int j = 0; j < i; j++) inv += p[j] > p[i];
            }
            brute = (brute + (inv % 2 ? -val : val)) % mod;
        } while (next_permutation(p.begin(), p.end()));
        brute = (brute + mod) % mod;
        assert(det_mod(a, mod) == brute);
    }
    assert(det_mod({{LLONG_MIN}}, 2147483647) == (LLONG_MIN % 2147483647 + 2147483647));
    cout << "PASS determinant: permutation expansion, row sign, singular, empty, negative\n";
}
