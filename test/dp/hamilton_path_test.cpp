#include "code/dp/hamilton_path.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(
        hamilton_path(vector<vector<ll>>{{0, 2, 10}, {2, 0, 3}, {10, 3, 0}},
                      0, 2) == 5);
    {
        for (int n = 1; n <= 8; n++)
        {
            for (int t = 0; t < 15; t++)
            {
                vector<vector<ll>> w(n, vector<ll>(n));
                for (auto &r : w)
                {
                    for (auto &v : r)
                    {
                        v = gen() % 20;
                    }
                }
                vector<int> p(n);
                iota(p.begin(), p.end(), 0);
                ll best = LLONG_MAX;
                do
                {
                    if (p.front() != 0 || p.back() != n - 1)
                    {
                        continue;
                    }
                    ll sum = 0;
                    for (int i = 1; i < n; i++)
                    {
                        sum += w[p[i - 1]][p[i]];
                    }
                    best = min(best, sum);
                } while (next_permutation(p.begin(), p.end()));
                assert(hamilton_path(w, 0, n - 1) == best);
            }
        }
    }
    cout << "PASS hamilton_path\n";
}
