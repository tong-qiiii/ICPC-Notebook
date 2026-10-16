#include "code/tree/tree_centroids.hpp"

void check(const vector<vector<int>>& g)
{
    auto before = g;
    auto res = tree_centroids(g);
    assert(tree_centroids(g) == res && g == before);
    sort(res.begin(), res.end());
    vector<int> expected;
    int n = (int)g.size() - 1;
    for (int removed = 1; removed <= n; removed++)
    {
        vector<int> vis(n + 1);
        vis[removed] = 1;
        int mx = 0;
        for (int start = 1; start <= n; start++)
        {
            if (vis[start]) continue;
            queue<int> q;
            q.push(start);
            vis[start] = 1;
            int cnt = 0;
            while (!q.empty())
            {
                int u = q.front();
                q.pop();
                cnt++;
                for (int v : g[u])
                {
                    if (vis[v]) continue;
                    vis[v] = 1;
                    q.push(v);
                }
            }
            mx = max(mx, cnt);
        }
        if (mx <= n / 2) expected.push_back(removed);
    }
    assert(res == expected);
}

int main()
{
    check({});
    check(vector<vector<int>>(1));
    check(vector<vector<int>>(2));
    for (int n = 2; n <= 20; n++)
    {
        vector<vector<int>> g(n + 1);
        for (int i = 1; i < n; i++)
        {
            g[i].push_back(i + 1);
            g[i + 1].push_back(i);
        }
        check(g);
        assert(tree_centroids(g).size() == (n % 2 ? 1U : 2U));
    }
    mt19937 rng(20261016);
    for (int round = 0; round < 600; round++)
    {
        int n = 1 + rng() % 50;
        vector<vector<int>> g(n + 1);
        vector<int> id(n);
        iota(id.begin(), id.end(), 1);
        shuffle(id.begin(), id.end(), rng);
        for (int i = 1; i < n; i++)
        {
            int u = id[i], v = id[rng() % i];
            g[u].push_back(v);
            g[v].push_back(u);
        }
        check(g);
    }
    vector<vector<int>> g(4001);
    for (int i = 1; i < 4000; i++)
    {
        g[i].push_back(i + 1);
        g[i + 1].push_back(i);
    }
    auto res = tree_centroids(g);
    sort(res.begin(), res.end());
    assert((res == vector<int>{2000, 2001}));
    g.assign(10001, {});
    for (int i = 1; i < 10000; i++)
    {
        g[10000].push_back(i);
        g[i].push_back(10000);
    }
    assert((tree_centroids(g) == vector<int>{10000}));
    check(vector<vector<int>>(2));
    cout << "PASS tree_centroids\n";
}
