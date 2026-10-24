#include "code/math/combination.hpp"
#include "code/counting/binomial_transform.hpp"
#include "code/counting/divisor_transform.hpp"

using Z = Mint<1000000007>;

vector<Z> neq(int n, int m)
{
    Comb<1000000007> cb(m);
    vector<Z> g(n + 1);
    for (int i = 0; i <= n; i++) g[i] = cb.A(m, n) * cb.C(n, i) * cb.A(m - i, n - i);
    return binom_transform(g, [&](int i, int j) { return cb.C(i, j); }, true, true);
}

Z gcd_sum(int n, int k)
{
    vector<Z> f(k + 1);
    for (int d = 1; d <= k; d++) f[d] = Z(k / d).pow(n);
    f = multiple_transform(f, true);
    Z ans = 0;
    for (int d = 1; d <= k; d++) ans += Z(d) * f[d];
    return ans;
}

Z necklace(int n, int m)
{
    Z ans = 0;
    for (int r = 0; r < n; r++) ans += Z(m).pow(gcd(n, r));
    return ans / Z(n);
}

int main()
{
    int pairs = 0;
    for (int m = 1; m <= 5; m++)
    {
        vector<int> p(m); iota(p.begin(), p.end(), 1);
        for (int n = 0; n <= m; n++)
        {
            set<vector<int>> seq;
            do
            {
                seq.insert(vector<int>(p.begin(), p.begin() + n));
            }
            while (next_permutation(p.begin(), p.end()));
            vector<int> want(n + 1);
            for (auto a : seq)
            {
                for (auto b : seq)
                {
                    int same = 0;
                    for (int i = 0; i < n; i++) same += a[i] == b[i];
                    want[same]++; pairs++;
                }
            }
            auto got = neq(n, m);
            for (int i = 0; i <= n; i++) assert(got[i].v == want[i]);
        }
    }
    assert(neq(2, 2)[0].v == 2 && neq(2, 3)[0].v == 18);
    // 大样例只求无坏事件：用交替和，不能使用 O(n^2) 的完整分布。
    int n = 141421, m = 356237;
    Comb<1000000007> cb(m);
    Z large = 0;
    for (int i = 0; i <= n; i++)
    {
        Z t = cb.A(m, n) * cb.C(n, i) * cb.A(m - i, n - i);
        if (i & 1) large -= t;
        else large += t;
    }
    assert(large.v == 881613484);
    for (int n = 1; n <= 5; n++)
    {
        for (int k = 1; k <= 5; k++)
        {
            int want = 0;
            auto dfs = [&](auto &&self, int dep, int g) -> void
            {
                if (dep == n)
                {
                    want += g;
                    return;
                }
                for (int v = 1; v <= k; v++) self(self, dep + 1, gcd(g, v));
            };
            dfs(dfs, 0, 0);
            assert(gcd_sum(n, k).v == want);
        }
    }
    assert(gcd_sum(3, 2).v == 9);
    assert(gcd_sum(3, 200).v == 10813692);
    assert(gcd_sum(100000, 100000).v == 742202979);
    for (int n = 1; n <= 7; n++)
    {
        for (int m = 1; m <= 3; m++)
        {
            set<vector<int>> classes;
            vector<int> s(n);
            auto dfs = [&](auto &&self, int pos) -> void
            {
                if (pos == n)
                {
                    auto t = s, best = s;
                    for (int r = 1; r < n; r++)
                    {
                        rotate(t.begin(), t.begin() + 1, t.end());
                        best = min(best, t);
                    }
                    classes.insert(best);
                    return;
                }
                for (int c = 0; c < m; c++) s[pos] = c, self(self, pos + 1);
            };
            dfs(dfs, 0);
            assert(necklace(n, m).v == (int)classes.size());
        }
    }
    assert(necklace(4, 3).v == 24);
    assert(isfinite((double)necklace(1000000, 1000000).v));
    cout << "PASS contest models: " << pairs << " NEQ pairs, exact gcd tuples, necklace rotation classes and official samples\n";
}
