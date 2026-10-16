#include "code/counting/partition_numbers.hpp"
#include "code/math/modint.hpp"

int main()
{
    auto p = partition_numbers<ll>(200);
    vector<ll> ref(201); ref[0] = 1;
    for (int w = 1; w <= 200; w++)
    {
        for (int i = w; i <= 200; i++) ref[i] += ref[i - w];
    }
    assert(p == ref);
    auto pm = partition_numbers<Mint<998244353>>(200);
    for (int i = 0; i <= 200; i++) assert(pm[i] == p[i]);
    assert(partition_numbers<ll>(0) == vector<ll>{1});
    cout << "PASS partitions: pentagonal recurrence vs unbounded knapsack through n=200\n";
}
