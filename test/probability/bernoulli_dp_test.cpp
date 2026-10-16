#include "code/probability/bernoulli_dp.hpp"
#include "code/math/modint.hpp"

int main()
{
    mt19937 gen(1101);
    using mint = Mint<998244353>;
    for (int t = 0; t < 250; t++)
    {
        int n = gen() % 11;
        vector<double> p(n + 1), ref(n + 1);
        vector<mint> pm(n + 1), rm(n + 1);
        for (int i = 1; i <= n; i++)
        {
            int v = gen() % 5;
            p[i] = v / 4.0; pm[i] = mint(v) / 4;
        }
        for (int s = 0; s < (1 << n); s++)
        {
            double w = 1; mint wm = 1;
            for (int i = 1; i <= n; i++)
            {
                w *= (s >> (i - 1) & 1) ? p[i] : 1 - p[i];
                wm *= (s >> (i - 1) & 1) ? pm[i] : mint(1) - pm[i];
            }
            int k = __builtin_popcount((unsigned)s);
            ref[k] += w; rm[k] += wm;
        }
        auto ans = bernoulli_dp(p);
        auto am = bernoulli_dp(pm);
        for (int k = 0; k <= n; k++)
        {
            assert(abs(ans[k] - ref[k]) < 1e-12);
            assert(am[k] == rm[k]);
        }
    }
    auto dp = bernoulli_dp(vector<double>{0, .3, .6, .8});
    assert(abs(dp[2] + dp[3] - .612) < 1e-12);
    dp = bernoulli_dp(vector<double>{0, .42, .01, .42, .99, .42});
    assert(abs(dp[3] + dp[4] + dp[5] - .3821815872) < 1e-12);
    assert(bernoulli_dp(vector<double>{}) == vector<double>{1});
    cout << "PASS Bernoulli DP: 250 subset enumerations, modular and official Coins samples\n";
}
