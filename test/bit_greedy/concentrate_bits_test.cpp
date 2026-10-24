#include "../../code/bit_greedy/concentrate_bits.hpp"
#include "support.hpp"

ll square_sum(const vector<ll> &a)
{
    ll ans = 0;
    for (ll v : a)
    {
        ans += v * v;
    }
    return ans;
}

ll brute(vector<ll> a)
{
    sort(a.begin(), a.end());
    set<vector<ll>> seen;
    queue<vector<ll>> q;
    seen.insert(a);
    q.push(a);
    ll ans = 0;
    while (!q.empty())
    {
        auto cur = q.front();
        q.pop();
        ans = max(ans, square_sum(cur));
        for (int i = 0; i < (int)cur.size(); i++)
        {
            for (int j = i + 1; j < (int)cur.size(); j++)
            {
                auto next = cur;
                next[i] = cur[i] & cur[j];
                next[j] = cur[i] | cur[j];
                sort(next.begin(), next.end());
                if (seen.insert(next).second)
                {
                    q.push(next);
                }
            }
        }
    }
    return ans;
}

int main()
{
    for (int t = 0; t < 400; t++)
    {
        vector<ll> a(rnd(0, 4) + 1);
        for (int i = 1; i < (int)a.size(); i++)
        {
            a[i] = rnd(0, 7);
        }
        auto ans = concentrate_bits(a);
        assert(ans.size() == a.size());
        assert(square_sum(ans) == brute(vector<ll>(a.begin() + 1, a.end())));
        for (int b = 0; b < 3; b++)
        {
            int ca = 0, cb = 0;
            for (ll v : a)
            {
                ca += v >> b & 1;
            }
            for (ll v : ans)
            {
                cb += v >> b & 1;
            }
            assert(ca == cb);
        }
    }
    auto ans = concentrate_bits({0, 1LL << 61, (1LL << 61) - 1});
    assert(ans == vector<ll>({0, (1LL << 62) - 1, 0}));
    cout << "PASS concentrate_bits: 400 reachable-state BFS cases and high bits\n";
}
