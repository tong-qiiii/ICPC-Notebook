#include "code/dp/monotone_queue_dp.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(monotone_queue_dp(vector<ll>{0, 2, 3, 1}, 2)[3] == 3);
    {
        for (int n = 0; n < 100; n++)
        {
            for (int k = 1; k < 15; k++)
            {
                vector<ll> a(n + 1), ref(n + 1, LLONG_MAX / 4);
                for (int i = 1; i <= n; i++)
                {
                    a[i] = int(gen() % 51) - 25;
                }
                ref[0] = 0;
                for (int i = 1; i <= n; i++)
                {
                    for (int j = max(0, i - k); j < i; j++)
                    {
                        ref[i] = min(ref[i], ref[j] + a[i]);
                    }
                }
                assert(monotone_queue_dp(a, k) == ref);
            }
        }
    }
    cout << "PASS monotone_queue_dp\n";
}
