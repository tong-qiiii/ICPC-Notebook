#include "code/math/bsgs.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(exBSGS(2, 8, 13) == 3 && exBSGS(2, 4, 8) == 2 &&
           exBSGS(2, 3, 8) == -1);
    {
        for (ll m = 1; m <= 100; m++)
        {
            for (ll a = -3; a <= m; a++)
            {
                for (ll b = 0; b < m; b++)
                {
                    ll cur = 1 % m, ref = -1;
                    set<ll> vis;
                    for (int x = 0; !vis.count(cur); x++)
                    {
                        if (cur == b)
                        {
                            ref = x;
                            break;
                        }
                        vis.insert(cur);
                        cur = (__int128)cur * ((a % m + m) % m) % m;
                    }
                    assert(exBSGS(a, b, m) == ref);
                    if (gcd(a, m) == 1)
                    {
                        assert(BSGS(a, b, m) == ref);
                    }
                }
            }
        }
    }
    cout << "PASS bsgs\n";
}
