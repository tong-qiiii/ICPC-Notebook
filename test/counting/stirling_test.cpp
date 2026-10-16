#include "code/counting/stirling.hpp"
#include "code/math/modint.hpp"

int main()
{
    Stirling<ll> st(8);
    for (int n = 0; n <= 8; n++)
    {
        vector<ll> cycles(n + 1), blocks(n + 1);
        vector<int> p(n); iota(p.begin(), p.end(), 0);
        do
        {
            vector<int> vis(n);
            int cnt = 0;
            for (int i = 0; i < n; i++)
            {
                if (vis[i]) continue;
                cnt++;
                for (int u = i; !vis[u]; u = p[u]) vis[u] = 1;
            }
            cycles[cnt]++;
        } while (next_permutation(p.begin(), p.end()));
        auto dfs = [&](auto &&self, int pos, int cnt) -> void
        {
            if (pos == n) { blocks[cnt]++; return; }
            for (int k = 0; k <= cnt; k++) self(self, pos + 1, cnt + (k == cnt));
        };
        dfs(dfs, 0, 0);
        for (int k = 0; k <= n; k++)
        {
            assert(st.s1[n][k] == cycles[k]);
            assert(st.s2[n][k] == blocks[k]);
        }
        for (int k = 0; k <= n; k++)
        {
            ll val = 0;
            for (int j = k; j <= n; j++) val += ((n - j) & 1 ? -1 : 1) * st.s1[n][j] * st.s2[j][k];
            assert(val == (n == k));
        }
    }
    st.init(2);
    assert(st.s2.size() == 3 && st.s2[2][1] == 1);
    Stirling<Mint<998244353>> sm(30);
    assert(sm.s2[0][0] == 1 && sm.s1[1][0] == 0);
    cout << "PASS Stirling: permutation cycles, restricted-growth partitions, matrix inverse and rebuild\n";
}
