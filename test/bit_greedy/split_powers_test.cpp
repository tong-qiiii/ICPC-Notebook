#include "../../code/bit_greedy/split_powers.hpp"
#include "support.hpp"

int brute(vector<int> a, int target)
{
    if (accumulate(a.begin(), a.end(), 0) < target)
    {
        return -1;
    }
    sort(a.begin(), a.end());
    queue<pair<vector<int>, int>> q;
    set<vector<int>> seen;
    q.push({a, 0});
    seen.insert(a);
    while (!q.empty())
    {
        auto [cur, steps] = q.front();
        q.pop();
        vector<int> dp(target + 1);
        dp[0] = 1;
        for (int v : cur)
        {
            for (int s = target; s >= v; s--)
            {
                dp[s] |= dp[s - v];
            }
        }
        if (dp[target])
        {
            return steps;
        }
        for (int i = 0; i < (int)cur.size(); i++)
        {
            if (cur[i] == 1 || (i && cur[i] == cur[i - 1]))
            {
                continue;
            }
            auto next = cur;
            int half = cur[i] / 2;
            next.erase(next.begin() + i);
            next.push_back(half);
            next.push_back(half);
            sort(next.begin(), next.end());
            if (seen.insert(next).second)
            {
                q.push({next, steps + 1});
            }
        }
    }
    return -1;
}

int main()
{
    for (int t = 0; t < 350; t++)
    {
        vector<int> material(rnd(0, 4));
        vector<ll> a(1);
        for (int &v : material)
        {
            v = 1 << rnd(0, 3);
            a.push_back(v);
        }
        int target = rnd(0, 24);
        assert(split_powers(a, target) == brute(material, target));
    }
    assert(split_powers({0, 1LL << 60}, 1) == 60);
    assert(split_powers({0, 1LL << 60, 1LL << 60}, (1LL << 61) - 1) == 60);
    assert(split_powers({0}, 0) == 0);
    assert(split_powers({0}, 1) == -1);
    cout << "PASS split_powers: 350 BFS split-state cases, empty and 60-bit cases\n";
}
