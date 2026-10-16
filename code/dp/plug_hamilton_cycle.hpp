#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;

inline int plugGet(ull s, int p)
{
    return (s >> (p * 2)) & 3ULL;
}

inline ull plugSet(ull s, int p, int v)
{
    ull mask = 3ULL << (p * 2);
    return (s & ~mask) | (ull(v) << (p * 2));
}

inline void addState(unordered_map<ull, ll> &mp, ull s, ll v)
{
    mp[s] += v;
}

inline ll plugHamiltonCycle(vector<string> a)
{
    if (a.empty() || a[0].empty())
    {
        return 0;
    }

    int n = a.size();
    int m = a[0].size();

    if (m > n)
    {
        vector<string> b(m, string(n, '.'));

        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                b[j][i] = a[i][j];
            }
        }

        a.swap(b);
        swap(n, m);
    }

    assert(m <= 31);
    int lastFree = -1;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (a[i][j] != '#')
            {
                lastFree = i * m + j;
            }
        }
    }

    if (lastFree == -1)
    {
        return 0;
    }

    unordered_map<ull, ll> dp, ndp;
    dp.reserve(1 << min(m, 16));
    dp[0] = 1;

    ll ans = 0;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            ndp.clear();
            ndp.reserve(dp.size() * 2 + 16);

            bool freeCell = a[i][j] != '#';
            bool down = i + 1 < n && a[i + 1][j] != '#';
            bool right = j + 1 < m && a[i][j + 1] != '#';

            for (auto [s, ways] : dp)
            {
                int L = plugGet(s, j);
                int U = plugGet(s, j + 1);

                ull base = plugSet(plugSet(s, j, 0), j + 1, 0);

                if (!freeCell)
                {
                    if (L == 0 && U == 0)
                    {
                        addState(ndp, base, ways);
                    }
                    continue;
                }

                if (L == 0 && U == 0)
                {
                    if (down && right)
                    {
                        ull ns = plugSet(plugSet(base, j, 1), j + 1, 2);
                        addState(ndp, ns, ways);
                    }
                    continue;
                }

                if ((L == 0) ^ (U == 0))
                {
                    int p = L ? L : U;
                    if (down)
                    {
                        addState(ndp, plugSet(base, j, p), ways);
                    }
                    if (right)
                    {
                        addState(ndp, plugSet(base, j + 1, p), ways);
                    }
                    continue;
                }

                if (L == 1 && U == 1)
                {
                    int bal = 1;

                    for (int k = j + 2; k <= m; ++k)
                    {
                        int x = plugGet(s, k);
                        if (x == 1)
                        {
                            ++bal;
                        }
                        if (x == 2)
                        {
                            --bal;
                        }

                        if (bal == 0)
                        {
                            addState(ndp, plugSet(base, k, 1), ways);
                            break;
                        }
                    }
                }
                else if (L == 2 && U == 2)
                {
                    int bal = 1;

                    for (int k = j - 1; k >= 0; --k)
                    {
                        int x = plugGet(s, k);
                        if (x == 2)
                        {
                            ++bal;
                        }
                        if (x == 1)
                        {
                            --bal;
                        }

                        if (bal == 0)
                        {
                            addState(ndp, plugSet(base, k, 2), ways);
                            break;
                        }
                    }
                }
                else if (L == 2 && U == 1)
                {
                    addState(ndp, base, ways);
                }
                else if (L == 1 && U == 2)
                {
                    if (base == 0 && i * m + j == lastFree)
                    {
                        ans += ways;
                    }
                }
            }

            dp.swap(ndp);
        }

        if (i + 1 < n)
        {
            unordered_map<ull, ll> shifted;
            shifted.reserve(dp.size() * 2 + 16);

            for (auto [s, ways] : dp)
            {
                addState(shifted, s << 2, ways);
            }

            dp.swap(shifted);
        }
    }

    return ans;
}
