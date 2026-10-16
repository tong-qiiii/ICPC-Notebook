#include "code/counting/count_sequences.hpp"
#include "code/math/modint.hpp"

int main()
{
    auto d = derangements<ll>(8);
    for (int n = 0; n <= 8; n++)
    {
        vector<int> p(n); iota(p.begin(), p.end(), 0);
        ll der = 0;
        vector<ll> row(n + 1);
        do
        {
            bool good = true;
            int cnt = 0;
            for (int i = 0; i < n; i++) good &= p[i] != i;
            for (int i = 1; i < n; i++) cnt += p[i - 1] > p[i];
            der += good; row[cnt]++;
        } while (next_permutation(p.begin(), p.end()));
        assert(d[n] == der && eulerian_numbers<ll>(n) == row);
    }
    auto c = catalan_numbers<ll>(9);
    for (int n = 0; n <= 9; n++)
    {
        ll ref = 0;
        for (int s = 0; s < (1 << (2 * n)); s++)
        {
            int bal = 0; bool good = true;
            for (int i = 0; i < 2 * n; i++)
            {
                bal += (s >> i & 1) ? 1 : -1;
                if (bal < 0) good = false;
            }
            ref += good && bal == 0;
        }
        assert(c[n] == ref);
    }
    auto cm = catalan_numbers<Mint<998244353>>(9);
    for (int n = 0; n <= 9; n++) assert(cm[n] == c[n]);
    cout << "PASS sequences: exhaustive derangements, descents, Dyck paths and base cases\n";
}
