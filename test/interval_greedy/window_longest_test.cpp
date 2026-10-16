#include "code/interval_greedy/window_longest.hpp"
int main()
{
    mt19937 gen(20261016);
    vector<int> a{0, 1, 2, 3};
    int sum = 0, best = 0;
    window_longest(
        3, [&](int p) { sum += a[p]; }, [&](int p) { sum -= a[p]; },
        [&]() { return sum <= 3; }, [&](int l, int r) { best = max(best, r - l + 1); });
    assert(best == 2);
    {
        for (int n = 0; n < 60; n++)
        {
            vector<int> a(n + 1);
            for (int i = 1; i <= n; i++)
            {
                a[i] = gen() % 8;
            }
            int cap = gen() % 25, sum = 0, best = 0, ref = 0;
            window_longest(
                n, [&](int p) { sum += a[p]; }, [&](int p) { sum -= a[p]; },
                [&]() { return sum <= cap; },
                [&](int l, int r) { best = max(best, r - l + 1); });
            for (int l = 1; l <= n; l++)
            {
                int s = 0;
                for (int r = l; r <= n; r++)
                {
                    s += a[r];
                    if (s <= cap)
                    {
                        ref = max(ref, r - l + 1);
                    }
                }
            }
            assert(best == ref);
        }
    }
    cout << "PASS window_longest\n";
}
