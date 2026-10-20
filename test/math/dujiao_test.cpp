#include "code/math/dujiao.hpp"

int main()
{
    MultiSieve s(200000);
    vector<ll> p(200001), m(200001);
    for (int i = 1; i <= 200000; i++) p[i] = p[i - 1] + s.phi[i], m[i] = m[i - 1] + s.mu[i];
    mt19937 rg(741);
    DuJiao d(50);
    for (int limit : {1, 7, 100, 1000})
    {
        d.init(limit);
        for (int z = 0; z < 300; z++)
        {
            int n = rg() % 200001;
            auto [x, y] = d.sum(n);
            assert(x == p[n] && y == m[n]);
            assert(d.sum(n) == make_pair(x, y));
        }
        assert(d.sum(0).first == 0);
    }
    d.init(100000);
    ll n = 10000000000LL;
    __int128_t x = 0, y = 0;
    for (ll l = 1, r; l <= n; l = r + 1)
    {
        r = n / (n / l);
        auto [a, b] = d.sum(n / l);
        x += (__int128_t)(r - l + 1) * a;
        y += (__int128_t)(r - l + 1) * b;
    }
    assert(x == (__int128_t)n * (n + 1) / 2 && y == 1);
    assert(d.sum(n).first > LLONG_MAX);
    cout << "PASS dujiao: sieve oracle, reinit/cache, 1e10 convolution identities, i128\n";
}
