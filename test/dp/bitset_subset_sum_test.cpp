#include "code/dp/bitset_subset_sum.hpp"
int main()
{
    mt19937 gen(20261016);
    auto d = subset_sum<100>(vector<int>{0, 2, 3});
    assert(d[5] && !d[4] && d[0]);
    {
        for (int run = 0; run < 500; run++)
        {
            vector<int> a(1);
            int n = gen() % 15;
            for (int i = 0; i < n; i++)
            {
                a.push_back(gen() % 20);
            }
            set<int> ref{0};
            for (int i = 1; i <= n; i++)
            {
                auto old = ref;
                for (int x : old)
                {
                    ref.insert(x + a[i]);
                }
            }
            auto d = subset_sum<100>(a);
            for (int s = 0; s <= 100; s++)
            {
                assert(d[s] == bool(ref.count(s)));
            }
        }
    }
    cout << "PASS bitset_subset_sum\n";
}
