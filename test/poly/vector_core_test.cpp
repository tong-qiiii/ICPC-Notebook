#include "code/poly/vector_core.hpp"
#include "code/poly/division_evaluation.hpp"
#include "code/poly/multipoint.hpp"
#include "code/poly/fast_interpolation.hpp"

int main()
{
    mt19937 gen(712368);
    static_assert(is_same_v<::Poly, vector<int>>);
    assert(&::ntt == &poly998::ntt);
    assert(polyInv({}, 0).empty());
    assert(polyMod({}, {1}).empty());
    assert(Multipoint({}).eval({1, 2}).empty());
    assert(fastInterpolation({}, {}).empty());
    for (int t = 0; t < 400; t++)
    {
        int n = 1 + gen() % 120, m = 1 + gen() % 90;
        ::Poly a(n), b(m);
        for (int &x : a) x = gen() % MOD;
        for (int &x : b) x = gen() % MOD;
        a[0] = 1 + gen() % (MOD - 1);
        b.back() = 1 + gen() % (MOD - 1);
        ::Poly ref(n);
        ref[0] = modPow(a[0], MOD - 2);
        for (int i = 1; i < n; i++)
        {
            long long sum = 0;
            for (int j = 1; j <= i; j++)
                sum = (sum + 1LL * a[j] * ref[i - j]) % MOD;
            ref[i] = (MOD - sum) * ref[0] % MOD;
        }
        assert(polyInv(a, n) == ref);
        assert(poly998::Poly(a).inv(n) == poly998::Poly(ref));
        ::Poly r = a;
        int iv = modPow(b.back(), MOD - 2);
        for (int i = n - 1; i >= m - 1; i--)
        {
            int q = 1LL * r[i] * iv % MOD;
            for (int j = 0; j < m; j++)
                r[i - m + 1 + j] = poly998::norm(r[i - m + 1 + j] - 1LL * q * b[j]);
        }
        r.resize(min(n, m - 1));
        ::norm(r);
        assert(polyMod(a, b) == r);
        assert(poly998::Poly(a) % poly998::Poly(b) == poly998::Poly(r));
        assert(mul(a, b) == poly998::convolution(a, b));
        assert(poly998::Poly(addPoly(a, b)) == poly998::Poly(a) + poly998::Poly(b));
    }
    // Vector and class variants can share one translation unit and one NTT.
    vector<int> pts{-5, 0, 3, 17}, f{9, 2, 7, 1};
    Multipoint mp(pts);
    auto ys = mp.eval(f);
    assert(ys == poly998::Poly(f).eval(pts));
    assert(ys == mp.eval(f));
    assert(fastInterpolation(pts, ys) == f);
    assert(poly998::Poly::interpolate(pts, ys) == poly998::Poly(f));
    cout << "PASS vector_core: inverse / remainder oracles / shared APIs\n";
}
