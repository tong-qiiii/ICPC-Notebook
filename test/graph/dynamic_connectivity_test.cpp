#include "code/graph/dynamic_connectivity.hpp"

int main()
{
    assert(DynCon(0).work().empty());
    mt19937 rg(734);
    for (int z = 0; z < 500; z++)
    {
        int n = 1 + rg() % 10;
        DynCon d(n);
        map<pair<int, int>, int> cnt;
        vector<int> expected;
        for (int t = 0; t < 150; t++)
        {
            int u = 1 + rg() % n, v = 1 + rg() % n;
            if (u > v) swap(u, v);
            int type = int(rg() % 3) - 1;
            if (type == -1 && !cnt[{u, v}]) type = 1;
            d.add(type, u, v);
            if (type) cnt[{u, v}] += type;
            else
            {
                vector<int> vis(n + 1);
                queue<int> q;
                q.push(u);
                vis[u] = 1;
                while (!q.empty())
                {
                    int x = q.front(); q.pop();
                    for (auto [e, c] : cnt)
                    {
                        auto [a, b] = e;
                        if (!c || (a != x && b != x)) continue;
                        int y = a ^ b ^ x;
                        if (!vis[y]) vis[y] = 1, q.push(y);
                    }
                }
                expected.push_back(vis[v]);
            }
        }
        assert(d.work() == expected);
        assert(d.work() == expected);
        d.op.clear();
        assert(d.work().empty());
    }
    cout << "PASS dynamic_connectivity\n";
    cout << "BFS brute, multiedges, self-loops, repeat work\n";
}
