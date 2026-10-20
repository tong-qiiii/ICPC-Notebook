#include "code/math/gauss_xor.hpp"

int main()
{
    mt19937 rg(737);
    for (int z = 0; z < 3000; z++)
    {
        int n = rg() % 10, m = rg() % 9;
        vector<bitset<64>> a(n);
        for (auto &v : a) for (int j = 0; j <= m; j++) v[j] = rg() & 1;
        int cnt = 0;
        for (int s = 0; s < (1 << m); s++)
        {
            bool ok = true;
            for (auto v : a)
            {
                int sum = 0;
                for (int j = 0; j < m; j++) sum ^= (s >> j & 1) & v[j];
                if (sum != (int)v[m]) ok = false;
            }
            cnt += ok;
        }
        bitset<64> ans;
        ans.set();
        int rank = gauss_xor<64>(a, m, ans);
        if (!cnt) assert(rank == -1 && ans.none());
        else
        {
            assert(rank >= 0 && cnt == (1 << (m - rank)));
            for (auto v : a) assert(((v & ans).count() & 1) == v[m]);
        }
    }
    cout << "PASS gauss_xor: exhaustive solutions, rank, inconsistency, zero variables\n";
}
