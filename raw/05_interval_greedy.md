# 5 区间贪心 / 双指针

> **统一约定**：本章普通数组、`vector`、区间编号和双指针均使用 `1-indexed`，有效范围写作 `1..n` 或 `1..m`，`0` 号位置留空。

## 5.1 区间集合合法二元组计数

> **用途**：统计 `1 <= l <= r <= n` 中，满足存在给定区间 `[a,b] ⊆ [l,r]` 的二元组 `(l,r)` 数量。
> **复杂度**：小值域时间 `O(n + m)`、空间 `O(n + m)`；大值域时间 `O(m log m)`、空间 `O(m)`。
> **下标**：`segs[1..m]`，值域 `1..n`。
> **限制**：输入区间满足 `1 <= a <= b <= n`，答案需能放入 `ll`。

### 调用

```cpp
vector<pll> segs(m + 1);

ll ans = count_pairs_small(n, segs); // n 不大
// n 很大、m 较小时用 count_pairs_large(n, segs)
```

### 模板

```cpp
ll count_pairs_small(int n, const vector<pll> &segs)
{
    int m = segs.size() - 1;
    vector<vector<int>> by_l(n + 1);

    for (int i = 1; i <= m; i++)
    {
        auto [l, r] = segs[i];
        by_l[l].push_back(r);
    }

    ll ans = 0;
    int mn = n + 1;

    for (int l = n; l >= 1; l--)
    {
        for (int r : by_l[l])
        {
            mn = min(mn, r);
        }

        if (mn <= n)
        {
            ans += n - mn + 1;
        }
    }

    return ans;
}

ll count_pairs_large(ll n, vector<pll> segs)
{
    int m = segs.size() - 1;
    sort(segs.begin() + 1, segs.end(), greater<pll>());

    ll ans = 0;
    ll mn = n + 1;

    for (int i = 1; i <= m; )
    {
        ll l = segs[i].x;
        int j = i;

        while (j <= m && segs[j].x == l)
        {
            mn = min(mn, segs[j].y);
            j++;
        }

        ll nl = (j <= m ? segs[j].x : 0);

        if (mn <= n)
        {
            ans += (l - nl) * (n - mn + 1);
        }

        i = j;
    }

    return ans;
}
```

### 现场记忆

固定左端点 `l`，只看所有满足 `a >= l` 的给定区间。

其中最小右端点记为 `mn`，则合法 `r` 恰好是 `mn..n`。

小值域从 `n` 往 `1` 扫；大值域中 `mn` 只会在某个区间左端点处改变。

一段左端点长度 `len` 的贡献就是 `len * (n - mn + 1)`。

## 5.2 区间最小点覆盖 / 最大不相交区间数

> **用途**：求覆盖所有闭区间的最少点集，或最多选出多少个两两不相交闭区间。
> **复杂度**：时间 `O(n log n)`，空间 `O(n)`。
> **下标**：`segs[1..n]`；`min_points()` 返回 `points[1..k]`。
> **注意**：按闭区间处理，`[a,b]` 与 `[b,c]` 算相交；若题目允许端点相接，判断改成对应的 `>= / <=`。

### 调用

```cpp
auto points = min_points(segs);
int k = points.size() - 1;

int cnt = max_disjoint(segs);
```

### 模板

```cpp
vector<ll> min_points(vector<pll> segs)
{
    int n = segs.size() - 1;

    sort(segs.begin() + 1, segs.end(), [](const pll &a, const pll &b)
    {
        if (a.y != b.y)
        {
            return a.y < b.y;
        }
        return a.x < b.x;
    });

    vector<ll> points(1);
    ll last = 0;
    bool has = false;

    for (int i = 1; i <= n; i++)
    {
        auto [l, r] = segs[i];

        if (!has || last < l)
        {
            last = r;
            has = true;
            points.push_back(r);
        }
    }

    return points;
}

int max_disjoint(vector<pll> segs)
{
    int n = segs.size() - 1;

    sort(segs.begin() + 1, segs.end(), [](const pll &a, const pll &b)
    {
        if (a.y != b.y)
        {
            return a.y < b.y;
        }
        return a.x < b.x;
    });

    int ans = 0;
    ll last = 0;
    bool has = false;

    for (int i = 1; i <= n; i++)
    {
        auto [l, r] = segs[i];

        if (!has || l > last)
        {
            ans++;
            last = r;
            has = true;
        }
    }

    return ans;
}
```

### 现场记忆

两个问题都先按右端点从小到大排序。

最小点覆盖：当前已选点不在新区间内，就选新区间右端点。

最大不相交：新区间满足 `l > last` 才选，再令 `last = r`。

闭区间端点相等仍然相交。

## 5.3 区间分组

> **用途**：把若干闭区间分到最少组，使每组内部区间两两不相交，并返回每个区间的组号。
> **复杂度**：时间 `O(n log n)`，空间 `O(n)`。
> **下标**：输入 `segs[1..n]`，`bel[1..n]` 对应原区间，组号为 `1..cnt`。
> **注意**：闭区间只有 `上一段.r < 当前段.l` 才能复用同一组；半开区间改成 `<=`。

### 调用

```cpp
auto [cnt, bel] = interval_group(segs);
```

### 模板

```cpp
pair<int, vector<int>> interval_group(const vector<pll> &segs)
{
    int n = segs.size() - 1;
    vector<int> ord(n + 1);
    iota(ord.begin() + 1, ord.end(), 1);

    sort(ord.begin() + 1, ord.end(), [&](int a, int b)
    {
        if (segs[a].x != segs[b].x)
        {
            return segs[a].x < segs[b].x;
        }
        return segs[a].y < segs[b].y;
    });

    priority_queue<pll, vector<pll>, greater<pll>> q;
    vector<int> bel(n + 1);
    int cnt = 0;

    for (int i = 1; i <= n; i++)
    {
        int idx = ord[i];
        auto [l, r] = segs[idx];
        int id;

        if (!q.empty() && q.top().x < l)
        {
            id = q.top().y;
            q.pop();
        }
        else
        {
            id = ++cnt;
        }

        bel[idx] = id;
        q.push({r, id});
    }

    return {cnt, bel};
}
```

### 现场记忆

先按左端点排序，小根堆维护每组当前最后一个区间的右端点。

`q.top().x < l`：最早结束的组可以复用。

否则连最早结束的组都冲突，所有已有组都不能用，必须新开组。

堆中始终每组只有一个当前结束位置。

## 5.4 区间覆盖目标区间

> **用途**：从若干闭区间中选择最少个，使其连续覆盖目标闭区间 `[L,R]`。
> **复杂度**：时间 `O(n log n)`，空间 `O(n)`。
> **下标**：`segs[1..n]`。
> **限制**：目标区间满足 `L <= R`；无解返回 `-1`。
> **注意**：这里是实数意义下的连续覆盖，相邻区间至少要端点相接，不能留下空隙。

### 调用

```cpp
int ans = min_cover(segs, L, R);
```

### 模板

```cpp
int min_cover(vector<pll> segs, ll L, ll R)
{
    if (L > R)
    {
        return -1;
    }

    int n = segs.size() - 1;

    if (L == R)
    {
        for (int i = 1; i <= n; i++)
        {
            auto [l, r] = segs[i];

            if (l <= L && L <= r)
            {
                return 1;
            }
        }
        return -1;
    }

    sort(segs.begin() + 1, segs.end());

    int i = 1;
    int cnt = 0;
    ll cur = L;

    while (cur < R)
    {
        ll far = cur;

        while (i <= n && segs[i].x <= cur)
        {
            far = max(far, segs[i].y);
            i++;
        }

        if (far <= cur)
        {
            return -1;
        }

        cur = far;
        cnt++;
    }

    return cnt;
}
```

### 现场记忆

`cur` 表示当前已经连续覆盖到的最右位置。

每轮吃掉所有 `l <= cur` 的区间，只保留其中最大的 `r`。

`far <= cur` 说明无法继续扩展，直接无解。

每选一个区间后令 `cur = far`；`L == R` 要单独判断是否有区间包含这个点。

## 5.5 双指针模板

> **用途**：维护具有单调性的连续窗口，或在有序序列上让左右指针单向逼近。
> **复杂度**：只要两个指针都不回退，通常时间 `O(n)`；额外空间取决于窗口维护的数据结构。
> **下标**：`1-indexed`。
> **限制**：移动某个指针后必须具有单调性；没有单调性不能硬套双指针。

### 最长 / 一般合法窗口

```cpp
int l = 1;

for (int r = 1; r <= n; r++)
{
    add(r);

    while (l <= r && !ok())
    {
        del(l);
        l++;
    }

    if (l <= r)
    {
        update(l, r);
    }
}
```

### 最短合法窗口

```cpp
int l = 1;

for (int r = 1; r <= n; r++)
{
    add(r);

    while (l <= r && ok())
    {
        update(l, r);
        del(l);
        l++;
    }
}
```

### 有序序列左右逼近

```cpp
int l = 1, r = n;

while (l < r)
{
    if (too_small(l, r))
    {
        l++;
    }
    else if (too_large(l, r))
    {
        r--;
    }
    else
    {
        update(l, r);

        // 找到答案后如何移动按题意决定，不一定同时移动两端
    }
}
```

### 现场记忆

同向窗口：右指针只扩展，左指针只收缩。

最长合法：`不合法 -> 缩左端`，恢复合法后更新答案。

最短合法：`合法 -> 先更新，再继续缩左端`。

左右逼近的核心不是写法，而是当前状态能确定只移动哪一侧，并且不会漏答案。
