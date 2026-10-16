#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include <bits/stdc++.h>
using namespace std;

template <class F> vector<ll> divideConquerDP(int n, int K, F cost)
{
    const ll INF = LLONG_MAX / 4;

    vector<ll> pre(n + 1, INF), cur(n + 1, INF);
    pre[0] = 0;

    for (int t = 1; t <= K; ++t)
    {
        fill(cur.begin(), cur.end(), INF);

        auto solve = [&](auto &&self, int l, int r, int optL,
                         int optR) -> void
        {
            if (l > r)
            {
                return;
            }

            int mid = (l + r) >> 1;
            ll best = INF;
            int bestPos = -1;
            int R = min(mid - 1, optR);

            for (int j = optL; j <= R; ++j)
            {
                if (pre[j] >= INF / 2)
                {
                    continue;
                }

                ll val = pre[j] + cost(j + 1, mid);
                if (val < best)
                {
                    best = val;
                    bestPos = j;
                }
            }

            if (bestPos == -1)
            {
                bestPos = optL;
            }
            cur[mid] = best;

            if (l == r)
            {
                return;
            }
            self(self, l, mid - 1, optL, bestPos);
            self(self, mid + 1, r, bestPos, optR);
        };

        if (t <= n)
        {
            solve(solve, t, n, t - 1, n - 1);
        }
        pre.swap(cur);
    }

    return pre;
}
