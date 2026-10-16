#include "code/probability/run_moments.hpp"
#include "code/math/modint.hpp"

int main()
{
    mt19937 gen(1110);
    using mint = Mint<998244353>;
    for (int t = 0; t < 200; t++)
    {
        int n = gen() % 11;
        vector<double> p(n + 1);
        vector<mint> pm(n + 1);
        for (int i = 1; i <= n; i++)
        {
            int v = gen() % 3;
            p[i] = v / 2.0; pm[i] = mint(v) / 2;
        }
        array<double, 3> ref{};
        array<mint, 3> rm{};
        for (int s = 0; s < (1 << n); s++)
        {
            double w = 1; mint wm = 1;
            int len = 0;
            array<int, 3> val{};
            for (int i = 1; i <= n + 1; i++)
            {
                bool on = i <= n && (s >> (i - 1) & 1);
                if (i <= n) w *= on ? p[i] : 1 - p[i], wm *= on ? pm[i] : mint(1) - pm[i];
                if (on) len++;
                else
                {
                    val[0] += len; val[1] += len * len; val[2] += len * len * len;
                    len = 0;
                }
            }
            for (int k = 0; k < 3; k++) ref[k] += w * val[k], rm[k] += wm * val[k];
        }
        auto ans = run_moments(p);
        auto am = run_moments(pm);
        for (int k = 0; k < 3; k++) assert(abs(ans[k] - ref[k]) < 1e-10 && am[k] == rm[k]);
    }
    cout << "PASS run moments: 200 bitmask enumerations, modular values and empty/all-zero/all-one\n";
}
