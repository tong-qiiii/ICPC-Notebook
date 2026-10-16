#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// 专用：区间加、区间和。永久标记，不下传。
struct Seg
{
    int n = 0, m = 2;
    vector<ll> sum = vector<ll>(4), tag = vector<ll>(4);

    Seg() = default;
    Seg(const vector<ll> &a)
    {
        build(a);
    }

    void build(const vector<ll> &a)
    {
        n = max(0, (int)a.size() - 1);
        m = 2;
        while (m < n + 2) m <<= 1;
        sum.assign(m << 1, 0);
        tag.assign(m << 1, 0);
        for (int i = 1; i <= n; i++) sum[m + i] = a[i];
        for (int p = m - 1; p; p--) sum[p] = sum[p << 1] + sum[p << 1 | 1];
    }

    void add(int l, int r, ll v)
    {
        if (l > r) return;
        assert(1 <= l && r <= n);
        int nl = 0, nr = 0, k = 1;
        // (l,r) 为开区间；nl/nr 是边界子树内已覆盖的长度。
        for (l += m - 1, r += m + 1; (l ^ r) != 1; l >>= 1, r >>= 1, k <<= 1)
        {
            sum[l] += v * nl;
            sum[r] += v * nr;
            if (!(l & 1))
            {
                tag[l ^ 1] += v;
                sum[l ^ 1] += v * k;
                nl += k;
            }
            if (r & 1)
            {
                tag[r ^ 1] += v;
                sum[r ^ 1] += v * k;
                nr += k;
            }
        }
        sum[l] += v * nl;
        sum[r] += v * nr;
        while (l >>= 1) sum[l] += v * (nl + nr);
    }

    ll query(int l, int r) const
    {
        if (l > r) return 0;
        assert(1 <= l && r <= n);
        ll ans = 0;
        int nl = 0, nr = 0, k = 1;
        for (l += m - 1, r += m + 1; (l ^ r) != 1; l >>= 1, r >>= 1, k <<= 1)
        {
            ans += tag[l] * nl + tag[r] * nr;
            if (!(l & 1)) ans += sum[l ^ 1], nl += k;
            if (r & 1) ans += sum[r ^ 1], nr += k;
        }
        ans += tag[l] * nl + tag[r] * nr;
        // 完整块的 sum 不含祖先标记，必须继续补到根。
        while (l >>= 1) ans += tag[l] * (nl + nr);
        return ans;
    }
};
