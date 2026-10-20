#include "code/math/ex_lucas.hpp"

int main()
{
    ExLucas c;
    for (int mod = 1; mod <= 250; mod++)
    {
        c.init(mod);
        vector<vector<ll>> f(81, vector<ll>(81));
        for (int n = 0; n <= 80; n++)
        {
            f[n][0] = 1 % mod;
            for (int k = 1; k <= n; k++) f[n][k] = (f[n - 1][k - 1] + f[n - 1][k]) % mod;
            for (int k = -1; k <= n + 1; k++) assert(c.C(n, k) == (k < 0 || k > n ? 0 : f[n][k]));
        }
        ll n = 1000000000000000000LL;
        assert(c.C(n, 0) == 1 % mod);
        assert(c.C(n, 1) == n % mod);
        assert(c.C(n, 2) == (ll)((__int128_t)n * (n - 1) / 2 % mod));
        assert(c.C(n, n - 2) == c.C(n, 2));
    }
    cout << "PASS ex_lucas: Pascal all moduli 1..250, invalid k, large n, prime powers, CRT\n";
}
