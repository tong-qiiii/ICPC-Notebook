#include "code/dp/lis.hpp"

int main()
{
    mt19937 rg(742);
    assert(lis({}).empty());
    for (int z = 0; z < 2000; z++)
    {
        int n = rg() % 40;
        vector<ll> a(n + 1);
        for (int i = 1; i <= n; i++) a[i] = int(rg() % 11) - 5;
        for (bool strict : {false, true})
        {
            vector<int> f(n + 1, 1);
            int best = 0;
            for (int i = 1; i <= n; i++)
            {
                for (int j = 1; j < i; j++) if (strict ? a[j] < a[i] : a[j] <= a[i]) f[i] = max(f[i], f[j] + 1);
                best = max(best, f[i]);
            }
            auto p = lis(a, strict);
            assert((int)p.size() == best);
            for (int i = 1; i < (int)p.size(); i++)
                assert(p[i - 1] < p[i] && (strict ? a[p[i - 1]] < a[p[i]] : a[p[i - 1]] <= a[p[i]]));
        }
    }
    cout << "PASS lis: quadratic oracle, both modes, recovered indices, duplicates, empty\n";
}
