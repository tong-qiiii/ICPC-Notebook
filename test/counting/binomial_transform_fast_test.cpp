#include "code/counting/binomial_transform_fast.hpp"
#include "code/counting/binomial_transform.hpp"
#include "code/math/combination.hpp"
#include "code/poly/ntt.hpp"

using Z = Mint<998244353>;

int main()
{
    auto conv = [](const vector<Z> &a, const vector<Z> &b)
    {
        vector<int> u, v;
        for (auto t : a) u.push_back(t.v);
        for (auto t : b) v.push_back(t.v);
        auto w = poly998::convolution(u, v);
        return vector<Z>(w.begin(), w.end());
    };
    Comb<998244353> cb(4096);
    mt19937 gen(1212);
    int count = 0;
    for (int n = 0; n <= 100; n++)
    {
        vector<Z> a(n);
        for (auto &v : a) v = gen();
        for (bool upper : {false, true})
        {
            for (bool inv : {false, true})
            {
                auto want = binom_transform(a, [&](int i, int j) { return cb.C(i, j); }, inv, upper);
                auto got = binom_fast(a, cb.fac, cb.ifac, conv, inv, upper);
                assert(got == want);
                assert(binom_fast(got, cb.fac, cb.ifac, conv, !inv, upper) == a);
                count++;
            }
        }
    }
    vector<Z> a(4096, 1);
    auto b = binom_fast(a, cb.fac, cb.ifac, conv);
    Z power = 1;
    for (auto v : b)
    {
        assert(v == power);
        power *= 2;
    }
    assert(binom_fast(b, cb.fac, cb.ifac, conv, true) == a);
    bool caught = false;
    try
    {
        binom_fast(a, vector<Z>{1}, vector<Z>{1}, conv);
    }
    catch (const out_of_range &)
    {
        caught = true;
    }
    assert(caught);
    cout << "PASS fast binomial transform: " << count << " NTT vs quadratic cases, roundtrips and length 4096\n";
}
