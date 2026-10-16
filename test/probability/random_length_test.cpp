#include "code/probability/random_length.hpp"

int main()
{
    mt19937 gen(1109);
    for (int t = 0; t < 300; t++)
    {
        int n = 1 + gen() % 30;
        vector<db> mass(n + 1), tail(n + 1);
        db total = 0;
        for (auto &v : mass) v = gen() % 100, total += v;
        for (auto &v : mass) v /= total;
        db e = 0, e2 = 0;
        for (int k = n; k >= 1; k--)
        {
            tail[k] = mass[k] + (k == n ? 0 : tail[k + 1]);
            e += k * mass[k]; e2 += k * k * mass[k];
        }
        auto ans = tail_moments(tail);
        assert(abs(ans[0] - e) < 1e-10);
        assert(abs(ans[1] - e2) < 1e-9);
        assert(abs(ans[2] - (e2 - e * e)) < 1e-9);
    }
    for (int n = 1; n <= 5; n++)
    {
        db ref = 0;
        auto dfs = [&](auto &&self, int s, int cnt, db p) -> void
        {
            for (int v = 0; v < n; v++)
            {
                if (s >> v & 1) ref += (cnt + 1) * p / n;
                else self(self, s | (1 << v), cnt + 1, p / n);
            }
        };
        dfs(dfs, 0, 0, 1);
        assert(abs(birthday_expect(n) - ref) < 1e-10);
    }
    assert(tail_moments(vector<db>{})[0] == 0);
    cout << "PASS random length: 300 moment identities and exhaustive birthday stopping paths\n";
}
