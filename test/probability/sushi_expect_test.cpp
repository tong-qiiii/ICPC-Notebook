#include "code/probability/sushi_expect.hpp"

int main()
{
    assert(abs(sushi_expect({0, 1, 1, 1}) - 5.5) < 1e-12);
    assert(abs(sushi_expect({0, 3}) - 3) < 1e-12);
    assert(abs(sushi_expect({0, 1, 2}) - 4.5) < 1e-12);
    assert(abs(sushi_expect({0, 1, 3, 2, 3, 3, 2, 3, 2, 1, 3}) - 54.48064457488221) < 1e-10);
    mt19937 gen(1105);
    for (int t = 0; t < 100; t++)
    {
        int n = 1 + gen() % 6;
        vector<int> a(n + 1), state(n);
        for (int i = 1; i <= n; i++) a[i] = state[i - 1] = gen() % 4;
        map<vector<int>, double> memo;
        auto dfs = [&](auto &&self, vector<int> s) -> double
        {
            if (memo.count(s)) return memo[s];
            int cnt = 0;
            double val = n;
            for (int i = 0; i < n; i++)
            {
                if (!s[i]) continue;
                cnt++; s[i]--;
                val += self(self, s);
                s[i]++;
            }
            return memo[s] = cnt ? val / cnt : 0;
        };
        assert(abs(sushi_expect(a) - dfs(dfs, state)) < 1e-10);
    }
    assert(sushi_expect({}) == 0 && sushi_expect({0, 0, 0}) == 0);
    vector<int> full(301, 3);
    full[0] = 0;
    assert(isfinite(sushi_expect(full)));
    cout << "PASS Sushi: 100 uncompressed-state comparisons, all samples and n=300\n";
}
