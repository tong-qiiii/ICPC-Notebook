#include "code/tree/tree_diameter_dfs.hpp"
#include "tree_diameter_reference.hpp"

int main()
{
    check_diameter(WeightedTree{}, tree_diameter_dfs);
    check_diameter(WeightedTree(1), tree_diameter_dfs);
    check_diameter(WeightedTree(2), tree_diameter_dfs);
    WeightedTree g(5);
    add_edge(g, 1, 2, 2);
    add_edge(g, 2, 3, 3);
    add_edge(g, 2, 4, 4);
    check_diameter(g, tree_diameter_dfs);
    g.assign(9, {});
    for (int i = 2; i <= 8; i++) add_edge(g, 1, i, 0);
    check_diameter(g, tree_diameter_dfs);
    g.assign(5, {});
    for (int i = 1; i < 4; i++) add_edge(g, i, i + 1, 3000000000000000000LL);
    check_diameter(g, tree_diameter_dfs);
    g.assign(3, {});
    add_edge(g, 1, 2, LLONG_MAX);
    check_diameter(g, tree_diameter_dfs);
    random_diameters(tree_diameter_dfs, false);
    g.assign(4001, {});
    for (int i = 1; i < 4000; i++) add_edge(g, i, i + 1, 1);
    auto [len, s, t] = tree_diameter_dfs(g);
    assert(len == 3999 && min(s, t) == 1 && max(s, t) == 4000);
    check_diameter(WeightedTree(2), tree_diameter_dfs);
    cout << "PASS tree_diameter_dfs\n";
}
