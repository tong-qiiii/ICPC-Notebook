#include "code/data_structure/sliding_window.hpp"
int main()
{
    mt19937 gen(20261016);
    vector<int> a{0, 3, 1, 2};
    assert((window_min(a, 2) == vector<int>{1, 1}));
    assert((window_max(a, 2) == vector<int>{3, 2}));
    {
        for (int n = 0; n <= 50; n++)
        {
            vector<int> a(n + 1);
            for (int i = 1; i <= n; i++)
            {
                a[i] = int(gen() % 21) - 10;
            }
            for (int k = 1; k <= n; k++)
            {
                vector<int> mn, mx;
                for (int i = k; i <= n; i++)
                {
                    mn.push_back(
                        *min_element(a.begin() + i - k + 1, a.begin() + i + 1));
                    mx.push_back(
                        *max_element(a.begin() + i - k + 1, a.begin() + i + 1));
                }
                assert(window_min(a, k) == mn);
                assert(window_max(a, k) == mx);
            }
            assert(window_min(a, 0).empty());
        }
    }
    cout << "PASS sliding_window\n";
}
