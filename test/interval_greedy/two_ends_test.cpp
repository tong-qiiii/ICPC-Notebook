#include "code/interval_greedy/two_ends.hpp"
int main()
{
    mt19937 gen(20261016);
    vector<int> a{0, 1, 2, 4};
    bool found = false;
    two_ends(
        3, [&](int l, int r) { return a[l] + a[r] < 5; },
        [&](int l, int r) { return a[l] + a[r] > 5; },
        [&](int, int)
        {
            found = true;
            return 0;
        });
    assert(found);
    {
        for (int n = 0; n < 50; n++)
        {
            vector<int> a(n + 1);
            for (int i = 1; i <= n; i++)
            {
                a[i] = gen() % 50;
            }
            sort(a.begin() + 1, a.end());
            for (int target = 0; target < 100; target++)
            {
                bool found = false, ref = false;
                two_ends(
                    n, [&](int l, int r) { return a[l] + a[r] < target; },
                    [&](int l, int r) { return a[l] + a[r] > target; },
                    [&](int, int)
                    {
                        found = true;
                        return 0;
                    });
                for (int i = 1; i <= n; i++)
                {
                    for (int j = i + 1; j <= n; j++)
                    {
                        ref |= a[i] + a[j] == target;
                    }
                }
                assert(found == ref);
            }
        }
    }
    cout << "PASS two_ends\n";
}
