#include "../../code/bit_greedy/borrow_chain.hpp"
#include "support.hpp"

int diff_count(const string &u, const string &v)
{
    int borrow = 0, cnt = 0;
    for (int i = (int)u.size() - 1; i >= 0; i--)
    {
        int d = (u[i] - '0') - (v[i] - '0') - borrow;
        borrow = d < 0;
        cnt += d + 2 * borrow;
    }
    assert(!borrow);
    return cnt;
}

void validate(int a, int b, int k, const pair<string, string> &ans)
{
    auto [u, v] = ans;
    assert((int)u.size() == a + b && v.size() == u.size());
    assert(u[0] == '1' && v[0] == '1' && u >= v);
    assert(count(u.begin(), u.end(), '1') == b);
    assert(count(v.begin(), v.end(), '1') == b);
    assert(count(u.begin(), u.end(), '0') == a);
    assert(count(v.begin(), v.end(), '0') == a);
    assert(diff_count(u, v) == k);
}

int main()
{
    int cases = 0;
    for (int n = 1; n <= 10; n++)
    {
        for (int b = 1; b <= n; b++)
        {
            vector<int> vals;
            for (int v = 1 << (n - 1); v < (1 << n); v++)
            {
                if (__builtin_popcount((unsigned)v) == b)
                {
                    vals.push_back(v);
                }
            }
            vector<int> possible(n + 1);
            for (int u : vals)
            {
                for (int v : vals)
                {
                    if (u >= v)
                    {
                        possible[__builtin_popcount((unsigned)(u - v))] = 1;
                    }
                }
            }
            for (int k = 0; k <= n; k++)
            {
                auto ans = borrow_chain(n - b, b, k);
                assert(!ans.first.empty() == (bool)possible[k]);
                assert(ans.first.empty() == ans.second.empty());
                if (possible[k])
                {
                    validate(n - b, b, k, ans);
                }
                cases++;
            }
        }
    }
    validate(199998, 2, 199998, borrow_chain(199998, 2, 199998));
    validate(1, 199999, 199998, borrow_chain(1, 199999, 199998));
    validate(100000, 100000, 0, borrow_chain(100000, 100000, 0));
    cout << "PASS borrow_chain: " << cases << " exhaustive small existence cases, 200000-bit witnesses\n";
}
