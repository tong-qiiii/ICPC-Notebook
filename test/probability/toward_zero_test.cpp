#include "code/probability/toward_zero.hpp"

int main()
{
    assert(toward_zero(0, 2, 1, 1) == 0);
    assert(abs(toward_zero(3, 2, 10, 20) - 20) < 1e-10);
    assert(abs(toward_zero(1, 2, 100, 1) - 1.2) < 1e-10);
    db got = toward_zero(314159265358979323LL, 4, 223606797, 173205080);
    assert(abs(got / 6418410657.7408381 - 1) < 1e-9);
    int count = 0;
    for (int n = 1; n <= 7; n++)
    {
        for (int a = 2; a <= 6; a++)
        {
            for (int x = 1; x <= 4; x++)
            {
                for (int y = 1; y <= 4; y++)
                {
                    db want = 1e100;
                    // 枚举每个正整数状态的固定动作，再独立求该策略的期望。
                    for (int mask = 0; mask < (1 << n); mask++)
                    {
                        vector<db> f(n + 1);
                        for (int v = 1; v <= n; v++)
                        {
                            if (mask >> (v - 1) & 1) f[v] = x + f[v / a];
                            else
                            {
                                db sum = 6 * y;
                                for (int d = 2; d <= 6; d++) sum += f[v / d];
                                f[v] = sum / 5;
                            }
                        }
                        want = min(want, f[n]);
                    }
                    assert(abs(toward_zero(n, a, x, y) - want) < 1e-10);
                    count++;
                }
            }
        }
    }
    got = toward_zero(1000000000000000000LL, 6, 1e9, 1e9);
    assert(isfinite(got) && got > 0 && got <= 61e9);
    cout << "PASS optimal expectation: " << count << " cases vs all stationary policies, official large sample\n";
}
