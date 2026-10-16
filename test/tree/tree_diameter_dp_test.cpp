#include "code/tree/tree_diameter_dp.hpp"
#include "tree_diameter_reference.hpp"

int main()
{
    check_diameter(WeightedTree{}, tree_diameter_dp);
    check_diameter(WeightedTree(1), tree_diameter_dp);
    check_diameter(WeightedTree(2), tree_diameter_dp);
    WeightedTree g(5);
    add_edge(g, 1, 2, -10);
    add_edge(g, 2, 3, 6);
    add_edge(g, 2, 4, 7);
    check_diameter(g, tree_diameter_dp);
    assert(get<0>(tree_diameter_dp(g)) == 13);
    g.assign(9, {});
    for (int i = 2; i <= 8; i++) add_edge(g, 1, i, -i);
    check_diameter(g, tree_diameter_dp);
    assert(get<0>(tree_diameter_dp(g)) == 0);
    g.assign(5, {});
    for (int i = 1; i < 4; i++) add_edge(g, i, i + 1, 3000000000000000000LL);
    check_diameter(g, tree_diameter_dp);
    for (long long w : {LLONG_MIN, 0LL, LLONG_MAX})
    {
        g.assign(3, {});
        add_edge(g, 1, 2, w);
        check_diameter(g, tree_diameter_dp);
    }
    random_diameters(tree_diameter_dp, false);
    random_diameters(tree_diameter_dp, true);
    g.assign(4001, {});
    for (int i = 1; i < 4000; i++) add_edge(g, i, i + 1, 1);
    auto [len, s, t] = tree_diameter_dp(g);
    assert(len == 3999 && min(s, t) == 1 && max(s, t) == 4000);
    check_diameter(WeightedTree(2), tree_diameter_dp);
    cout << "PASS tree_diameter_dp\n";
}
