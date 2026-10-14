## 4.1 单调队列：滑动窗口最小值 / 最大值

> **用途**：`O(n)` 求每个长度为 `k` 的滑动窗口最小值 / 最大值。
> **复杂度**：时间 `O(n)`，空间 `O(k)`
> **下标**：`1-indexed`
> **限制**：`1 <= k <= n`

```cpp
vector<int> window_min(const vector<int> &a, int k) {
    int n = a.size() - 1;
    deque<int> q;
    vector<int> ans;
    ans.reserve(n - k + 1);

    for (int i = 1; i <= n; ++i) {
        while (!q.empty() && q.front() <= i - k)
            q.pop_front();

        while (!q.empty() && a[q.back()] >= a[i])
            q.pop_back();

        q.push_back(i);

        if (i >= k)
            ans.push_back(a[q.front()]);
    }

    return ans;
}

vector<int> window_max(const vector<int> &a, int k) {
    int n = a.size() - 1;
    deque<int> q;
    vector<int> ans;
    ans.reserve(n - k + 1);

    for (int i = 1; i <= n; ++i) {
        while (!q.empty() && q.front() <= i - k)
            q.pop_front();

        while (!q.empty() && a[q.back()] <= a[i])
            q.pop_back();

        q.push_back(i);

        if (i >= k)
            ans.push_back(a[q.front()]);
    }

    return ans;
}
```

### 现场记忆

```text
a[1..n]，队列存下标，队首是当前窗口答案。

先弹过期位置：q.front() <= i - k
再从队尾维护单调性。

最小值：值递增，>= a[i] 弹。
最大值：值递减，<= a[i] 弹。

i >= k 时产生第一个答案。
```

## 4.2 单调栈：左右最近严格更小元素

> **用途**：`O(n)` 求每个位置左 / 右侧第一个满足大小关系的位置。
> **复杂度**：时间 `O(n)`，空间 `O(n)`
> **下标**：`1-indexed`

```cpp
vector<int> a;

vector<int> solve(int n) {
    vector<int> res(n + 1);
    vector<int> stk;

    for (int i = 1; i <= n; ++i) {
        // 左侧第一个严格小于 a[i] 的位置
        while (!stk.empty() && a[stk.back()] >= a[i]) {
            stk.pop_back();
        }

        res[i] = stk.empty() ? 0 : stk.back();
        stk.push_back(i);
    }

    return res;
}
```

### 现场记忆

```text
栈存下标。

严格小于：弹 >=
小于等于：弹 >
严格大于：弹 <=
大于等于：弹 <

找左边：1 -> n
找右边：n -> 1
```

## 笛卡尔树 - 单调栈建树

> **用途**：给定 `a[1..n]`，构造下标满足 BST、权值满足小根堆性质的笛卡尔树。
> **复杂度**：时间 `O(n)`，空间 `O(n)`
> **下标**：`1-indexed`

```cpp
vector<int> a, ls, rs;

int solve(int n) {
    vector<int> stk;
    ls.assign(n + 1, 0);
    rs.assign(n + 1, 0);

    for (int i = 1; i <= n; ++i) {
        int last = 0;
        while (!stk.empty() && a[stk.back()] > a[i]) {
            last = stk.back();
            stk.pop_back();
        }

        ls[i] = last;
        if (!stk.empty()) rs[stk.back()] = i;
        stk.push_back(i);
    }

    return stk[0];
}
```

### 现场记忆

```text
维护权值单调递增栈。

插入 i：
弹出所有 > a[i] 的点
最后弹出的点 -> ls[i]
弹完后的栈顶 -> 它的 rs = i

最后 stk[0] 是根。
大根堆反转比较符号。
```

## 4.3 ST 表：区间最大值

> **用途**：静态数组预处理后，`O(1)` 查询区间最大值。
> **复杂度**：预处理 `O(n log n)`，单次查询 `O(1)`，空间 `O(n log n)`
> **下标**：`1-indexed`
> **限制**：数组静态不修改；`1 <= l <= r <= n`

### 调用

```cpp
vector<int> a(n + 1);
for (int i = 1; i <= n; ++i)
    cin >> a[i];

STMax st(a);

cout << st.query(l, r) << '\n';
```

### 模板

```cpp
struct STMax {
    int n = 0;
    vector<int> lg;
    vector<vector<int>> st;

    STMax() = default;

    STMax(const vector<int> &a) {
        build(a);
    }

    void build(const vector<int> &a) {
        n = (int)a.size() - 1;

        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; ++i)
            lg[i] = lg[i >> 1] + 1;

        int LOG = lg[n] + 1;
        st.assign(LOG, vector<int>(n + 1));

        for (int i = 1; i <= n; ++i)
            st[0][i] = a[i];

        for (int k = 1; k < LOG; ++k) {
            int len = 1 << k;
            int half = len >> 1;

            for (int i = 1; i + len - 1 <= n; ++i)
                st[k][i] = max(st[k - 1][i], st[k - 1][i + half]);
        }
    }

    int query(int l, int r) const {
        int k = lg[r - l + 1];
        return max(st[k][l], st[k][r - (1 << k) + 1]);
    }
};
```

### 现场记忆

`st[k][i]`：从 `i` 开始、长度 `2^k` 的最大值。

查询长度 `len`，取 `k = floor(log2(len))`。

两个长度 `2^k` 的块允许重叠：
`[l, l+2^k-1]` 和 `[r-2^k+1, r]`。

## 4.4 Fenwick 树：动态集合第 k 小

> **用途**：离散值域上动态插入 / 删除，并查询当前集合第 `k` 小。
> **复杂度**：修改、前缀和、第 `k` 小均为 `O(log n)`，空间 `O(n)`
> **下标**：值域 `1-indexed`
> **限制**：树中维护的是非负频次；调用 `kth(k)` 时需满足 `1 <= k <= total()`

### 调用

```cpp
BITKth bit(n);

bit.add(x, 1);   // 插入 x
bit.add(x, -1);  // 删除 x

int x = bit.kth(k);
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `init(n)` | 初始化值域 `[1,n]` |
| `add(x,v)` | `x` 的出现次数增加 `v` |
| `sum(x)` | `[1,x]` 中元素数量 |
| `total()` | 当前元素总数 |
| `kth(k)` | 第 `k` 小对应的值域位置 |

### 模板

```cpp
struct BITKth {
    int n = 0;
    vector<int> tree;

    BITKth(int n = 0) {
        if (n)
            init(n);
    }

    void init(int n_) {
        n = n_;
        tree.assign(n + 1, 0);
    }

    void add(int x, int v) {
        for (; x <= n; x += x & -x)
            tree[x] += v;
    }

    int sum(int x) const {
        int res = 0;

        for (; x; x -= x & -x)
            res += tree[x];

        return res;
    }

    int total() const {
        return sum(n);
    }

    int kth(int k) const {
        int pos = 0;
        int cur = 0;

        int step = 1;
        while ((step << 1) <= n)
            step <<= 1;

        for (; step; step >>= 1) {
            int nxt = pos + step;

            if (nxt <= n && cur + tree[nxt] < k) {
                pos = nxt;
                cur += tree[nxt];
            }
        }

        return pos + 1;
    }
};
```

### 现场记忆

`kth` 找最小的 `pos`，使前缀和 `sum(pos) >= k`。

二进制下降维护：
`cur = sum(pos)`。

若 `cur + tree[nxt] < k`，说明答案还在右边，跳到 `nxt`。

删除后必须保证每个位置频次非负，否则前缀和不再单调，`kth` 失效。


## 4.5 线性基

> **用途**：维护一组数的异或线性空间，支持插入、可表示性、最大异或、第 `k` 小异或值等操作。
> **复杂度**：插入 / 查询 `O(B)`；`rebuild()` 为 `O(B^2)`；空间 `O(B)`，其中 `B = 63`
> **下标**：二进制位 `0 ~ 62`
> **限制**：默认处理 `[0, 2^63)` 内的非负 `long long`
> **注意**：`kth()` / `rank()` 使用前必须 `rebuild()`；之后若再次 `insert()` / `merge()`，需要重新 `rebuild()`。

### 调用

```cpp
Basis bs;

for (int i = 1; i <= n; ++i) {
    long long x;
    cin >> x;
    bs.insert(x);
}

cout << bs.max_xor() << '\n';

bs.rebuild();

cout << bs.kth(k) << '\n';      // k 从 0 开始
cout << bs.rank(x) << '\n';     // 不可表示返回 -1
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `init()` | 清空线性基 |
| `insert(x)` | 插入；独立返回 `true`，相关返回 `false` |
| `contains(x)` | 是否能异或表示出 `x` |
| `max_xor()` | 可表示出的最大异或值 |
| `min_nonzero()` | 最小非零异或值；空基返回 `0` |
| `merge(other)` | 合并另一个线性基 |
| `rebuild()` | 转为 `kth / rank` 所需的规范基 |
| `distinct()` | 不同异或值数量 |
| `kth(k)` | 第 `k` 小不同异或值，`k` 从 `0` 开始 |
| `rank(x)` | `x` 在不同异或值中的 `0-indexed` 排名 |
| `cnt` | 线性基维数 |

### 模板

```cpp
struct Basis {
    static constexpr int LOG = 62;

    long long basis[LOG + 1]{};
    vector<long long> vec;
    int cnt = 0;
    bool ready = false;

    Basis() {
        init();
    }

    void init() {
        fill(basis, basis + LOG + 1, 0);
        vec.clear();
        cnt = 0;
        ready = false;
    }

    bool insert(long long x) {
        ready = false;

        for (int i = LOG; i >= 0; --i) {
            if (!((x >> i) & 1))
                continue;

            if (!basis[i]) {
                basis[i] = x;
                ++cnt;
                return true;
            }

            x ^= basis[i];
        }

        return false;
    }

    bool contains(long long x) const {
        for (int i = LOG; i >= 0; --i) {
            if (!((x >> i) & 1))
                continue;

            if (!basis[i])
                return false;

            x ^= basis[i];
        }

        return true;
    }

    long long max_xor(long long x = 0) const {
        for (int i = LOG; i >= 0; --i)
            x = max(x, x ^ basis[i]);

        return x;
    }

    long long min_nonzero() const {
        for (int i = 0; i <= LOG; ++i)
            if (basis[i])
                return basis[i];

        return 0;
    }

    void merge(const Basis &other) {
        for (int i = 0; i <= LOG; ++i)
            if (other.basis[i])
                insert(other.basis[i]);
    }

    void rebuild() {
        for (int i = LOG; i >= 0; --i) {
            if (!basis[i])
                continue;

            for (int j = i - 1; j >= 0; --j)
                if ((basis[i] >> j) & 1)
                    basis[i] ^= basis[j];
        }

        vec.clear();
        for (int i = 0; i <= LOG; ++i)
            if (basis[i])
                vec.push_back(basis[i]);

        ready = true;
    }

    unsigned long long distinct() const {
        return 1ULL << cnt;
    }

    long long kth(unsigned long long k) const {
        if (!ready || k >= (1ULL << cnt))
            return -1;

        long long res = 0;

        for (int i = 0; i < cnt; ++i)
            if ((k >> i) & 1)
                res ^= vec[i];

        return res;
    }

    long long rank(long long x) const {
        if (!ready || !contains(x))
            return -1;

        long long res = 0;

        for (int i = cnt - 1; i >= 0; --i) {
            if ((x ^ vec[i]) < x) {
                x ^= vec[i];
                res |= 1LL << i;
            }
        }

        return res;
    }
};
```

### 现场记忆

插入从最高位向下消元：
该最高位没有基就放进去，否则 `x ^= basis[i]`。

`insert(x) == false` ⇔ `x` 与现有基线性相关。

最大异或：从高位到低位，只要 `res ^ basis[i]` 更大就异或。

`rebuild()` 消掉每个基向量中的低位主元，再按主元从低到高放入 `vec`。

规范化后，`k` 的二进制位直接决定选哪些 `vec[i]`，因此可得到排序后的第 `k` 小不同异或值。

任何后续 `insert / merge` 都会破坏规范基，`kth / rank` 前重新 `rebuild()`。


## 4.6 普通莫队

> **用途**：离线处理静态区间询问，适合加入 / 删除一个位置能快速维护答案的问题。
> **复杂度**：块长为 `B` 时约 `O(mB + n^2/B)` 次指针移动；`m≈n` 时为 `O((n+m)√n)`
> **下标**：数组、区间均 `1-indexed`

```cpp
using ll = long long;

struct Query {
    int l, r, id;
};

void solve() {
    int n, m;
    cin >> n >> m;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> a[i];

    vector<Query> q(m);
    for (int i = 0; i < m; ++i) {
        cin >> q[i].l >> q[i].r;
        q[i].id = i;
    }

    int B = max(1, n / max(1, (int)sqrt(m)));

    sort(q.begin(), q.end(), [&](const Query &A, const Query &B_) {
        int ba = (A.l - 1) / B;
        int bb = (B_.l - 1) / B;

        if (ba != bb)
            return ba < bb;

        // 奇偶块优化
        if (ba & 1)
            return A.r > B_.r;

        return A.r < B_.r;
    });

    ll now = 0;

    auto add = [&](int pos) {
        int x = a[pos];

        // TODO
    };

    auto del = [&](int pos) {
        int x = a[pos];

        // TODO
    };

    vector<ll> ans(m);

    int L = 1, R = 0;

    for (auto [l, r, id] : q) {
        while (L > l) add(--L);
        while (R < r) add(++R);
        while (L < l) del(L++);
        while (R > r) del(R--);

        ans[id] = now;
    }

    for (ll x : ans)
        cout << x << '\n';
}
```

### 现场记忆

莫队只负责移动 `[L,R]`，题意全部塞进 `add / del`。

左端点分块，同块 `r` 奇偶反向排序减少折返。

块长可取 `B≈n/√m`；`m≈n` 时就是 `√n`。


## 4.7 带修莫队

> **用途**：离线处理区间询问 + 单点修改。
> **复杂度**：询问、修改与 `n` 同阶时，取 `B≈n^(2/3)`，约 `O(n^(5/3))` 次状态转移
> **下标**：数组、区间、修改均 `1-indexed`
> **注意**：修改点在当前区间内时，必须 `del -> 改值 -> add`。

```cpp
using ll = long long;

struct Query {
    int l, r, t, id;
};

struct Change {
    int pos, pre, now;
};

void solve() {
    int n, m;
    cin >> n >> m;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> a[i];

    vector<Query> q;
    vector<Change> chg(1);  // 修改从 1 开始
    vector<int> tmp = a;

    q.reserve(m);
    chg.reserve(m + 1);

    for (int i = 0; i < m; ++i) {
        char op;
        int x, y;
        cin >> op >> x >> y;

        if (op == 'Q') {
            q.push_back({x, y, (int)chg.size() - 1, (int)q.size()});
        } else {
            chg.push_back({x, tmp[x], y});
            tmp[x] = y;
        }
    }

    vector<int> disc;
    disc.reserve(n + 2 * ((int)chg.size() - 1));

    for (int i = 1; i <= n; ++i)
        disc.push_back(a[i]);

    for (int t = 1; t < (int)chg.size(); ++t) {
        disc.push_back(chg[t].pre);
        disc.push_back(chg[t].now);
    }

    sort(disc.begin(), disc.end());
    disc.erase(unique(disc.begin(), disc.end()), disc.end());

    auto get_id = [&](int x) {
        return (int)(lower_bound(disc.begin(), disc.end(), x) - disc.begin());
    };

    for (int i = 1; i <= n; ++i)
        a[i] = get_id(a[i]);

    for (int t = 1; t < (int)chg.size(); ++t) {
        chg[t].pre = get_id(chg[t].pre);
        chg[t].now = get_id(chg[t].now);
    }

    int B = max(1, (int)cbrt(1LL * n * n));

    sort(q.begin(), q.end(), [&](const Query &A, const Query &B_) {
        int al = (A.l - 1) / B;
        int bl = (B_.l - 1) / B;

        if (al != bl)
            return al < bl;

        int ar = (A.r - 1) / B;
        int br = (B_.r - 1) / B;

        if (ar != br) {
            if (al & 1)
                return ar > br;
            return ar < br;
        }

        // 时间维奇偶优化
        if (ar & 1)
            return A.t > B_.t;

        return A.t < B_.t;
    });

    vector<int> cur = a;
    vector<int> cnt(disc.size());

    ll now = 0;
    int L = 1, R = 0, T = 0;

    auto add = [&](int pos) {
        int x = cur[pos];

        // TODO
    };

    auto del = [&](int pos) {
        int x = cur[pos];

        // TODO
    };

    auto apply = [&](int t) {
        int pos = chg[t].pos;

        if (L <= pos && pos <= R) {
            del(pos);
            cur[pos] = chg[t].now;
            add(pos);
        } else {
            cur[pos] = chg[t].now;
        }
    };

    auto undo = [&](int t) {
        int pos = chg[t].pos;

        if (L <= pos && pos <= R) {
            del(pos);
            cur[pos] = chg[t].pre;
            add(pos);
        } else {
            cur[pos] = chg[t].pre;
        }
    };

    vector<ll> ans(q.size());

    for (auto [l, r, t, id] : q) {
        while (L > l) add(--L);
        while (R < r) add(++R);
        while (L < l) del(L++);
        while (R > r) del(R--);

        while (T < t) apply(++T);
        while (T > t) undo(T--);

        ans[id] = now;
    }

    for (ll x : ans)
        cout << x << '\n';
}
```

### 现场记忆

状态从 `(L,R)` 变成 `(L,R,T)`。

第 `t` 次修改记录 `pre -> now`；同一位置连续修改时，`pre` 必须用读入时的临时数组维护。

修改点在 `[L,R]`：
`del(pos) -> cur[pos]=新值 -> add(pos)`。

撤销完全相反，只把新值换回 `pre`。

## 4.8 只增回滚莫队

> **用途**：删除困难，但加入容易、临时加入可以回滚时处理静态区间询问。
> **复杂度**：约 `O(mB + n^2/B)` 次状态操作；`m≈n` 时取 `B≈√n`
> **下标**：数组、区间均 `1-indexed`
> **注意**：右扩是永久状态，左扩是临时状态；两者可以分开维护以减少回滚常数。

```cpp
using ll = long long;

struct Query {
    int l, r, id;
};

void solve() {
    int n, m;
    cin >> n >> m;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> a[i];

    vector<Query> q(m);
    for (int i = 0; i < m; ++i) {
        cin >> q[i].l >> q[i].r;
        q[i].id = i;
    }

    int B = max(1, n / max(1, (int)sqrt(m)));

    sort(q.begin(), q.end(), [&](const Query &A, const Query &B_) {
        int ba = (A.l - 1) / B;
        int bb = (B_.l - 1) / B;

        if (ba != bb)
            return ba < bb;

        return A.r < B_.r;
    });

    ll now = 0;

    // 永久右扩，不需要记录回滚信息
    auto add_right = [&](int pos) {
        int x = a[pos];

        // TODO
    };

    // 临时左扩，需要支持回滚
    auto add_left = [&](int pos) {
        int x = a[pos];

        // TODO
        // 保存被修改状态的旧值
    };

    auto rollback_left = [&](int pos) {
        int x = a[pos];

        // TODO
        // 撤销最近一次 add_left
    };

    auto clear = [&]() {
        // TODO: 清空所有题意状态和回滚栈
        now = 0;
    };

    vector<ll> ans(m);

    int last_block = -1;
    int R = 0;

    for (auto [l, r, id] : q) {
        int b = (l - 1) / B;
        int block_r = min(n, (b + 1) * B);

        if (b != last_block) {
            last_block = b;

            clear();
            R = block_r;
        }

        // 完全位于左端点块内：直接暴力
        if (r <= block_r) {
            ll backup = now;

            for (int i = l; i <= r; ++i)
                add_left(i);

            ans[id] = now;

            for (int i = r; i >= l; --i)
                rollback_left(i);

            now = backup;
            continue;
        }

        // 右侧永久状态，只增不退
        while (R < r)
            add_right(++R);

        ll backup = now;

        // 左侧临时加入
        for (int i = block_r; i >= l; --i)
            add_left(i);

        ans[id] = now;

        // 严格逆序撤销左扩
        for (int i = l; i <= block_r; ++i)
            rollback_left(i);

        now = backup;
    }

    for (ll x : ans)
        cout << x << '\n';
}
```

### 常用维护：区间相同值最大下标差

`a[i]` 先离散化到 `[0,V)`。

```cpp
vector<int> lmn(V, n + 1);
vector<int> rmx(V, -1);

vector<array<int, 3>> stk;

int now = 0;

auto add_right = [&](int pos) {
    int x = a[pos];

    lmn[x] = min(lmn[x], pos);
    rmx[x] = max(rmx[x], pos);

    now = max(now, rmx[x] - lmn[x]);
};

auto add_left = [&](int pos) {
    int x = a[pos];

    stk.push_back({x, lmn[x], rmx[x]});

    lmn[x] = min(lmn[x], pos);
    rmx[x] = max(rmx[x], pos);

    now = max(now, rmx[x] - lmn[x]);
};

auto rollback_left = [&](int pos) {
    auto [x, old_l, old_r] = stk.back();
    stk.pop_back();

    lmn[x] = old_l;
    rmx[x] = old_r;
};
```

复杂维护优先记快照：

```cpp
ll backup = now;
int snapshot = stk.size();

for (int i = block_r; i >= l; --i)
    add_left(i);

ans[id] = now;

while ((int)stk.size() > snapshot)
    rollback_left(0);

now = backup;
```

### 现场记忆

每个左端点块重新开始；同块询问按 `r` 升序，右指针只增。

右边 `[block_r+1,R]` 是永久状态，不必记录左扩所需的回滚信息。

左边 `[l,block_r]` 临时加入，查询后回滚。

临时加入顺序 `block_r -> l`，撤销顺序必须 `l -> block_r`。

复杂状态直接 `snapshot = stk.size()`，回滚到 snapshot 最稳。

## 4.9 只删回滚莫队

> **用途**：加入困难，但删除容易、最近的删除可以回滚时处理静态区间询问。
> **复杂度**：约 `O(mB + n^2/B)` 次状态操作；`m≈n` 时取 `B≈√n`
> **下标**：数组、区间均 `1-indexed`
> **注意**：保留块间状态转移；`L` 全程只向右推进，不为每个块重新建整个状态。

```cpp
using ll = long long;

struct Query {
    int l, r, id;
};

void solve() {
    int n, m;
    cin >> n >> m;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> a[i];

    vector<Query> q(m);
    for (int i = 0; i < m; ++i) {
        cin >> q[i].l >> q[i].r;
        q[i].id = i;
    }

    int B = max(1, n / max(1, (int)sqrt(m)));

    sort(q.begin(), q.end(), [&](const Query &A, const Query &B_) {
        int ba = (A.l - 1) / B;
        int bb = (B_.l - 1) / B;

        if (ba != bb)
            return ba < bb;

        // 同块 r 降序，右端点只向左删除
        return A.r > B_.r;
    });

    ll now = 0;

    auto init = [&]() {
        // TODO:
        // 初始化完整区间 [1,n] 的数据结构和 now
        // 清空回滚栈
    };

    auto del = [&](int pos) {
        int x = a[pos];

        // TODO:
        // 删除 pos
        // 保存足够的信息供 rollback 恢复
    };

    auto rollback = [&](int pos) {
        int x = a[pos];

        // TODO:
        // 撤销最近一次 del(pos)
        // 可以只恢复数据结构，now 由外部 backup 恢复
    };

    vector<ll> ans(m);

    init();

    int last_block = -1;

    int L = 1;
    int R = n;

    // 当前块基准状态 [block_l,n] 的答案
    ll block_now = now;

    for (auto [l, r, id] : q) {
        int b = (l - 1) / B;
        int block_l = b * B + 1;

        if (b != last_block) {
            if (last_block != -1) {
                // 恢复上一块右侧永久删除
                while (R < n)
                    rollback(++R);

                now = block_now;
            }

            last_block = b;

            // 从上一块基准直接推进到新块
            // L 整个算法过程中只向右
            while (L < block_l)
                del(L++);

            block_now = now;
            R = n;
        }

        // 同块 r 降序，右端点永久向左删除
        while (R > r)
            del(R--);

        ll backup = now;

        // 左端点临时删除
        while (L < l)
            del(L++);

        ans[id] = now;

        // 左端点严格逆序回滚
        while (L > block_l)
            rollback(--L);

        now = backup;
    }

    for (ll x : ans)
        cout << x << '\n';
}
```

### 现场记忆

初始维护完整 `[1,n]`。

每块基准状态是 `[block_l,n]`；`L` 跨块只向右，因此块间不用重新建状态。

同块 `r` 降序，右侧删除顺序：
`n, n-1, ...`。

换块前按相反顺序 `R+1, R+2, ..., n` 回滚右侧，再恢复 `block_now`。

左侧临时删除后必须严格逆序回滚；回滚函数若不维护答案，直接用 `backup` 恢复 `now`。

## 4.10 树上莫队

> **用途**：通过双 Euler 序把树上路径询问转成莫队区间询问。
> **复杂度**：Euler / LCA 预处理 `O(n log n)`；莫队约 `O((n+m)√n)` 次节点切换
> **下标**：树节点 `1-indexed`
> **注意**：非祖先路径使用 `[out[u],in[v]]`，LCA 需要额外加入一次。

```cpp
using ll = long long;

struct Query {
    int l, r, id, extra;
};

void solve() {
    int n, m;
    cin >> n >> m;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> a[i];

    vector<vector<int>> g(n + 1);

    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;

        g[u].push_back(v);
        g[v].push_back(u);
    }

    int LOG = 1;
    while ((1LL << LOG) <= n)
        ++LOG;

    vector<vector<int>> fa(LOG, vector<int>(n + 1));
    vector<int> dep(n + 1);
    vector<int> in(n + 1), out(n + 1);
    vector<int> euler(2 * n + 1);

    int timer = 0;

    auto dfs = [&](auto &&self, int u, int f) -> void {
        dep[u] = dep[f] + 1;
        fa[0][u] = f;

        for (int k = 1; k < LOG; ++k)
            fa[k][u] = fa[k - 1][fa[k - 1][u]];

        in[u] = ++timer;
        euler[timer] = u;

        for (int v : g[u]) {
            if (v == f)
                continue;

            self(self, v, u);
        }

        out[u] = ++timer;
        euler[timer] = u;
    };

    dfs(dfs, 1, 0);

    auto lca = [&](int u, int v) {
        if (dep[u] < dep[v])
            swap(u, v);

        int d = dep[u] - dep[v];

        for (int k = LOG - 1; k >= 0; --k)
            if ((d >> k) & 1)
                u = fa[k][u];

        if (u == v)
            return u;

        for (int k = LOG - 1; k >= 0; --k) {
            if (fa[k][u] != fa[k][v]) {
                u = fa[k][u];
                v = fa[k][v];
            }
        }

        return fa[0][u];
    };

    vector<Query> q(m);

    for (int id = 0; id < m; ++id) {
        int u, v;
        cin >> u >> v;

        if (in[u] > in[v])
            swap(u, v);

        int p = lca(u, v);

        if (p == u)
            q[id] = {in[u], in[v], id, 0};
        else
            q[id] = {out[u], in[v], id, p};
    }

    int len = 2 * n;
    int B = max(1, len / max(1, (int)sqrt(m)));

    sort(q.begin(), q.end(), [&](const Query &A, const Query &B_) {
        int ba = (A.l - 1) / B;
        int bb = (B_.l - 1) / B;

        if (ba != bb)
            return ba < bb;

        // 奇偶块优化
        if (ba & 1)
            return A.r > B_.r;

        return A.r < B_.r;
    });

    vector<int> vis(n + 1);

    ll now = 0;

    auto add_node = [&](int u) {
        int x = a[u];

        // TODO
    };

    auto del_node = [&](int u) {
        int x = a[u];

        // TODO
    };

    auto flip = [&](int pos) {
        int u = euler[pos];

        if (vis[u])
            del_node(u);
        else
            add_node(u);

        vis[u] ^= 1;
    };

    vector<ll> ans(m);

    int L = 1, R = 0;

    for (auto [l, r, id, extra] : q) {
        while (L > l) flip(--L);
        while (R < r) flip(++R);
        while (L < l) flip(L++);
        while (R > r) flip(R--);

        if (extra) {
            add_node(extra);
            ans[id] = now;
            del_node(extra);
        } else {
            ans[id] = now;
        }
    }

    for (ll x : ans)
        cout << x << '\n';
}
```

### 现场记忆

Euler 序每个节点出现两次；当前区间中出现奇数次的节点就是路径节点。

先保证 `in[u] <= in[v]`。

若 `lca(u,v)==u`：
`[in[u],in[v]]`。

否则：
`[out[u],in[v]] + lca(u,v)`。

额外 LCA 不在莫队区间中，记录答案时临时 `add_node` 再 `del_node`。

树可能退化成链且 `n` 很大时，递归 DFS 注意爆栈。

## 4.11 二次离线莫队：二元组贡献

> **用途**：处理区间答案可写成 `ans(l,r)=Σw(i,j)`（`l<=i<j<=r`）的二元组贡献问题。
> **复杂度**：事件总扫描长度与莫队指针移动量同阶，约 `O(mB+n^2/B)` 次题意查询
> **下标**：数组、区间均 `1-indexed`
> **限制**：需要能快速维护 `Σ_{j<=x}w(j,i)` 与 `Σ_{j>=x}w(i,j)`。
> **注意**：`query_pre / query_suf` 应尽量做到 `O(1)` 或很小的复杂度，否则二次离线仍可能超时。

```cpp
using ll = long long;

struct Query {
    int l, r, id;
};

struct Event {
    int l, r, id, op;
};

void solve() {
    int n, m;
    cin >> n >> m;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> a[i];

    vector<Query> q(m);

    for (int i = 0; i < m; ++i) {
        cin >> q[i].l >> q[i].r;
        q[i].id = i;
    }

    // pre[i] = ans(1,i)
    // suf[i] = ans(i,n)
    vector<ll> pre(n + 1);
    vector<ll> suf(n + 2);

    // TODO: 根据题意预处理 pre / suf

    int B = max(1, n / max(1, (int)sqrt(m)));

    sort(q.begin(), q.end(), [&](const Query &A, const Query &B_) {
        int ba = (A.l - 1) / B;
        int bb = (B_.l - 1) / B;

        if (ba != bb)
            return ba < bb;

        // 奇偶块优化
        if (ba & 1)
            return A.r > B_.r;

        return A.r < B_.r;
    });

    vector<vector<Event>> pre_event(n + 1);
    vector<vector<Event>> suf_event(n + 2);

    // delta[id]：莫队排序后第 id 个询问相对前一个询问的答案变化
    vector<ll> delta(m);

    int L = 1, R = 0;

    for (int id = 0; id < m; ++id) {
        int l = q[id].l;
        int r = q[id].r;

        if (R < r) {
            delta[id] += pre[r] - pre[R];

            if (L > 1)
                pre_event[L - 1].push_back({R + 1, r, id, -1});
        } else if (R > r) {
            delta[id] -= pre[R] - pre[r];

            if (L > 1)
                pre_event[L - 1].push_back({r + 1, R, id, 1});
        }

        R = r;

        if (L > l) {
            delta[id] += suf[l] - suf[L];

            if (R < n)
                suf_event[R + 1].push_back({l, L - 1, id, -1});
        } else if (L < l) {
            delta[id] -= suf[L] - suf[l];

            if (R < n)
                suf_event[R + 1].push_back({L, l - 1, id, 1});
        }

        L = l;
    }

    /*
        前缀第二次离线：

        扫描到 x 时维护
        query_pre(i) = sum_{j <= x} w(j,i)
    */

    auto init_pre = [&]() {
        // TODO
    };

    auto insert_pre = [&](int x) {
        // TODO
    };

    auto query_pre = [&](int i) -> ll {
        // TODO
        return 0;
    };

    init_pre();

    for (int x = 1; x <= n; ++x) {
        insert_pre(x);

        for (auto [l, r, id, op] : pre_event[x]) {
            ll res = 0;

            for (int i = l; i <= r; ++i)
                res += query_pre(i);

            delta[id] += 1LL * op * res;
        }
    }

    /*
        后缀第二次离线：

        扫描到 x 时维护
        query_suf(i) = sum_{j >= x} w(i,j)
    */

    auto init_suf = [&]() {
        // TODO
    };

    auto insert_suf = [&](int x) {
        // TODO
    };

    auto query_suf = [&](int i) -> ll {
        // TODO
        return 0;
    };

    init_suf();

    for (int x = n; x >= 1; --x) {
        insert_suf(x);

        for (auto [l, r, id, op] : suf_event[x]) {
            ll res = 0;

            for (int i = l; i <= r; ++i)
                res += query_suf(i);

            delta[id] += 1LL * op * res;
        }
    }

    vector<ll> ans(m);

    ll now = 0;

    for (int id = 0; id < m; ++id) {
        now += delta[id];
        ans[q[id].id] = now;
    }

    for (ll x : ans)
        cout << x << '\n';
}
```

### 现场记忆

设：

`pre[i] = ans(1,i)`，`suf[i] = ans(i,n)`。

右扩：
`pre[r]-pre[R]`，再减去左边界外的贡献。

右缩：
`-(pre[R]-pre[r])`，再补回左边界外的贡献。

左扩 / 左缩同理使用 `suf`，修正右边界外的贡献。

修正项不当场算，只挂事件；第二次扫描统一处理：

`query_pre(i) = Σ_{j<=x} w(j,i)`

`query_suf(i) = Σ_{j>=x} w(i,j)`

事件区间总长度与莫队移动量同阶。

## 4.12 值域分块：频次 / 第 k 小 / 前驱后继

> **用途**：在离散值域上维护动态多重集合，支持频次修改、排名、第 `k` 小、前驱和后继。
> **复杂度**：修改 `O(1)`；查询 `O(√V)`；空间 `O(V)`
> **下标**：值域 `1 ~ n`
> **限制**：需要保证每个值的频次始终非负；值域过大时先离散化。

### 调用

```cpp
ValueBlock blk(n);

blk.add(x, 1);   // 插入 x
blk.add(x, -1);  // 删除 x

cout << blk.count_le(x) << '\n';
cout << blk.kth(k) << '\n';
cout << blk.prev(x) << '\n';
cout << blk.next(x) << '\n';
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `init(n)` | 初始化值域 `[1,n]` |
| `add(x,v)` | `x` 的出现次数增加 `v` |
| `count_le(x)` | `<= x` 的元素数量 |
| `count_lt(x)` | `< x` 的元素数量 |
| `kth(k)` | 第 `k` 小，`k` 从 `1` 开始 |
| `prev(x)` | 严格小于 `x` 的最大值 |
| `next(x)` | 严格大于 `x` 的最小值 |
| `tot` | 当前元素总数 |

### 模板

```cpp
struct ValueBlock {
    int n = 0;
    int len = 0;
    int num = 0;
    int tot = 0;

    vector<int> freq;
    vector<int> sum;
    vector<int> bel;
    vector<int> L, R;

    ValueBlock() = default;

    ValueBlock(int n) {
        init(n);
    }

    void init(int n_) {
        n = n_;
        len = max(1, (int)sqrt(n));
        num = (n + len - 1) / len;
        tot = 0;

        freq.assign(n + 1, 0);
        sum.assign(num + 1, 0);
        bel.assign(n + 1, 0);
        L.assign(num + 1, 0);
        R.assign(num + 1, 0);

        for (int b = 1; b <= num; ++b) {
            L[b] = (b - 1) * len + 1;
            R[b] = min(n, b * len);

            for (int i = L[b]; i <= R[b]; ++i)
                bel[i] = b;
        }
    }

    void add(int x, int v) {
        freq[x] += v;
        sum[bel[x]] += v;
        tot += v;
    }

    int count_le(int x) const {
        if (x <= 0)
            return 0;

        if (x >= n)
            return tot;

        int b = bel[x];
        int res = 0;

        for (int i = 1; i < b; ++i)
            res += sum[i];

        for (int i = L[b]; i <= x; ++i)
            res += freq[i];

        return res;
    }

    int count_lt(int x) const {
        return count_le(x - 1);
    }

    int kth(int k) const {
        if (k < 1 || k > tot)
            return -1;

        for (int b = 1; b <= num; ++b) {
            if (k > sum[b]) {
                k -= sum[b];
                continue;
            }

            for (int x = L[b]; x <= R[b]; ++x) {
                if (k > freq[x])
                    k -= freq[x];
                else
                    return x;
            }
        }

        return -1;
    }

    int prev(int x) const {
        if (x <= 1)
            return -1;

        int p = min(n, x - 1);
        int b = bel[p];

        for (int i = p; i >= L[b]; --i)
            if (freq[i])
                return i;

        for (--b; b >= 1; --b) {
            if (!sum[b])
                continue;

            for (int i = R[b]; i >= L[b]; --i)
                if (freq[i])
                    return i;
        }

        return -1;
    }

    int next(int x) const {
        if (x >= n)
            return -1;

        int p = max(1, x + 1);
        int b = bel[p];

        for (int i = p; i <= R[b]; ++i)
            if (freq[i])
                return i;

        for (++b; b <= num; ++b) {
            if (!sum[b])
                continue;

            for (int i = L[b]; i <= R[b]; ++i)
                if (freq[i])
                    return i;
        }

        return -1;
    }
};
```

### 现场记忆

按“值”分块，不是按数组位置分块。

`freq[x]` 维护单值频次，`sum[b]` 维护整块元素数量。

第 `k` 小：
先整块减 `sum[b]`，定位块后再逐值减 `freq[x]`。

排名：
`rank(x) = count_lt(x) + 1`。

前驱 / 后继：
先扫当前残块，再跳过 `sum[b] == 0` 的整块。

值域太大先离散化；此时 `kth()` 返回离散后的下标。

## 4.13 序列分块：区间加 / 区间和

> **用途**：按数组位置平方分块，维护区间加、区间和，并支持单点赋值。
> **复杂度**：建块 `O(n)`；修改 / 查询 `O(√n)`；空间 `O(n)`
> **下标**：数组、区间均 `1-indexed`

### 调用

```cpp
vector<ll> a(n + 1);
for (int i = 1; i <= n; ++i)
    cin >> a[i];

SeqBlock blk(a);

blk.modify(l, r, v);
blk.change(pos, v);

cout << blk.query(l, r) << '\n';
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `build(a)` | 按 `a[1..n]` 重新建块 |
| `change(pos,v)` | 单点赋值 `a[pos] = v` |
| `modify(l,r,v)` | 区间 `[l,r] += v` |
| `query(l,r)` | 查询 `[l,r]` 区间和 |

### 模板

```cpp
using ll = long long;

struct SeqBlock {
    int n = 0;
    int len = 0;
    int num = 0;

    vector<ll> a;
    vector<ll> sum;
    vector<ll> tag;

    vector<int> bel;
    vector<int> L, R;

    SeqBlock() = default;

    SeqBlock(const vector<ll> &a) {
        build(a);
    }

    void build(const vector<ll> &src) {
        a = src;
        n = (int)a.size() - 1;

        len = max(1, (int)sqrt(n));
        num = (n + len - 1) / len;

        sum.assign(num + 1, 0);
        tag.assign(num + 1, 0);

        bel.assign(n + 1, 0);
        L.assign(num + 1, 0);
        R.assign(num + 1, 0);

        for (int b = 1; b <= num; ++b) {
            L[b] = (b - 1) * len + 1;
            R[b] = min(n, b * len);

            for (int i = L[b]; i <= R[b]; ++i) {
                bel[i] = b;
                sum[b] += a[i];
            }
        }
    }

    void pushdown(int b) {
        if (!tag[b])
            return;

        for (int i = L[b]; i <= R[b]; ++i)
            a[i] += tag[b];

        tag[b] = 0;
    }

    void rebuild(int b) {
        sum[b] = 0;

        for (int i = L[b]; i <= R[b]; ++i)
            sum[b] += a[i];
    }

    void change(int pos, ll v) {
        int b = bel[pos];

        pushdown(b);

        a[pos] = v;

        rebuild(b);
    }

    void modify(int l, int r, ll v) {
        int bl = bel[l];
        int br = bel[r];

        if (bl == br) {
            pushdown(bl);

            for (int i = l; i <= r; ++i)
                a[i] += v;

            rebuild(bl);
            return;
        }

        pushdown(bl);

        for (int i = l; i <= R[bl]; ++i)
            a[i] += v;

        rebuild(bl);

        for (int b = bl + 1; b < br; ++b) {
            tag[b] += v;
            sum[b] += 1LL * (R[b] - L[b] + 1) * v;
        }

        pushdown(br);

        for (int i = L[br]; i <= r; ++i)
            a[i] += v;

        rebuild(br);
    }

    ll query(int l, int r) const {
        int bl = bel[l];
        int br = bel[r];

        ll res = 0;

        if (bl == br) {
            for (int i = l; i <= r; ++i)
                res += a[i] + tag[bl];

            return res;
        }

        for (int i = l; i <= R[bl]; ++i)
            res += a[i] + tag[bl];

        for (int b = bl + 1; b < br; ++b)
            res += sum[b];

        for (int i = L[br]; i <= r; ++i)
            res += a[i] + tag[br];

        return res;
    }
};
```

### 现场记忆

按“位置”分块，`bel[i]` 表示下标 `i` 所属块。

`a[i]` 不含尚未下放的 `tag[b]`；
`sum[b]` 始终保存整块真实总和。

整块加：

`tag[b] += v`

`sum[b] += 块长 * v`

需要直接修改残块时：

`pushdown -> 修改 a[i] -> rebuild`

查询残块用 `a[i] + tag[b]`；
查询整块直接用 `sum[b]`。

以后维护块内排序、最大值、最小值等，主要修改 `rebuild()` 和整块查询逻辑。

## 4.14 InfoTag 线段树

> **用途**：通用懒标记线段树框架；默认实现区间加、区间和，换题时主要修改 `Tag / Info / operator+`。
> **复杂度**：建树 `O(n)`；区间修改 / 查询 `O(log n)`；空间 `O(n)`
> **下标**：数组、区间均 `1-indexed`
> **注意**：`Tag::apply(t)` 表示在已有旧标记之后，再执行新标记 `t`；非交换操作时合并顺序不能写反。

### 调用

```cpp
vector<ll> a(n + 1);
for (int i = 1; i <= n; ++i)
    cin >> a[i];

Seg seg(a);

seg.modify(l, r, Tag(v));        // [l,r] += v

cout << seg.query(l, r).sum << '\n';
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `build(a)` | 按 `a[1..n]` 重新建树 |
| `modify(l,r,t)` | 对 `[l,r]` 应用标记 `t` |
| `query(l,r)` | 返回 `[l,r]` 的 `Info` |
| `Tag::apply(t)` | 合并懒标记 |
| `Info::apply(t)` | 标记作用到区间信息 |
| `operator+` | 合并左右儿子信息 |

### 模板

```cpp
using ll = long long;

// 默认：区间加
struct Tag {
    ll add = 0;

    Tag(ll add = 0) : add(add) {}

    void apply(const Tag &t) {
        add += t.add;
    }

    bool empty() const {
        return add == 0;
    }
};

// 默认：区间和
struct Info {
    ll sum = 0;
    int len = 0;

    Info() = default;

    Info(ll x) : sum(x), len(1) {}

    void apply(const Tag &t) {
        sum += 1LL * len * t.add;
    }
};

Info operator+(const Info &A, const Info &B) {
    Info res;
    res.sum = A.sum + B.sum;
    res.len = A.len + B.len;
    return res;
}

struct Seg {
    int n = 0;
    vector<Info> info;
    vector<Tag> tag;

    Seg() = default;

    Seg(const vector<ll> &a) {
        build(a);
    }

    void build(const vector<ll> &a) {
        n = (int)a.size() - 1;
        info.assign(n * 4 + 5, Info());
        tag.assign(n * 4 + 5, Tag());
        build(1, 1, n, a);
    }

    void pushup(int p) {
        info[p] = info[p << 1] + info[p << 1 | 1];
    }

    void apply(int p, const Tag &t) {
        info[p].apply(t);
        tag[p].apply(t);
    }

    void pushdown(int p) {
        if (tag[p].empty())
            return;

        apply(p << 1, tag[p]);
        apply(p << 1 | 1, tag[p]);
        tag[p] = Tag();
    }

    void build(int p, int l, int r, const vector<ll> &a) {
        if (l == r) {
            info[p] = Info(a[l]);
            return;
        }

        int mid = (l + r) >> 1;

        build(p << 1, l, mid, a);
        build(p << 1 | 1, mid + 1, r, a);

        pushup(p);
    }

    void modify(int p, int l, int r, int ql, int qr, const Tag &t) {
        if (ql <= l && r <= qr) {
            apply(p, t);
            return;
        }

        pushdown(p);

        int mid = (l + r) >> 1;

        if (ql <= mid)
            modify(p << 1, l, mid, ql, qr, t);
        if (qr > mid)
            modify(p << 1 | 1, mid + 1, r, ql, qr, t);

        pushup(p);
    }

    void modify(int l, int r, const Tag &t) {
        modify(1, 1, n, l, r, t);
    }

    Info query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr)
            return info[p];

        pushdown(p);

        int mid = (l + r) >> 1;

        if (qr <= mid)
            return query(p << 1, l, mid, ql, qr);

        if (ql > mid)
            return query(p << 1 | 1, mid + 1, r, ql, qr);

        return query(p << 1, l, mid, ql, qr)
             + query(p << 1 | 1, mid + 1, r, ql, qr);
    }

    Info query(int l, int r) {
        return query(1, 1, n, l, r);
    }
};
```

### 现场记忆

`Info` 保存当前区间的真实信息，`Tag` 保存尚未下传给儿子的操作。

整段修改一定同时做：

`info[p].apply(t)`  
`tag[p].apply(t)`

即答案先更新，标记只负责以后下传。

`Tag::apply(t)` 的语义：
**旧操作之后执行新操作 `t`**。
若把标记看成函数，应合成为 `t ∘ old`。

下探儿子前必须 `pushdown()`，修改儿子后必须 `pushup()`。

换题通常只改三处：

`Tag`：标记内容 + 标记合并  
`Info`：区间信息 + 标记作用  
`operator+`：左右区间合并


## 4.15 Info 线段树：单点修改

> **用途**：通用单点修改线段树框架；默认实现单点赋值、区间和，换题时主要修改 `Info / operator+`。
> **复杂度**：建树 `O(n)`；单点修改 / 区间查询 `O(log n)`；空间 `O(n)`
> **下标**：数组、区间均 `1-indexed`

### 调用

```cpp
vector<ll> a(n + 1);
for (int i = 1; i <= n; ++i)
    cin >> a[i];

Seg seg(a);

seg.modify(x, Info(v));           // a[x] = v

cout << seg.query(l, r).sum << '\n';
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `build(a)` | 按 `a[1..n]` 重新建树 |
| `modify(x,v)` | 将位置 `x` 的信息改为 `v` |
| `query(l,r)` | 返回 `[l,r]` 的 `Info` |
| `operator+` | 合并左右区间信息 |

### 模板

```cpp
using ll = long long;

// 默认：区间和
struct Info {
    ll sum = 0;

    Info() = default;

    Info(ll x) : sum(x) {}
};

Info operator+(const Info &A, const Info &B) {
    Info res;
    res.sum = A.sum + B.sum;
    return res;
}

struct Seg {
    int n = 0;
    vector<Info> info;

    Seg() = default;

    Seg(const vector<ll> &a) {
        build(a);
    }

    void build(const vector<ll> &a) {
        n = (int)a.size() - 1;
        info.assign(n * 4 + 5, Info());
        build(1, 1, n, a);
    }

    void pushup(int p) {
        info[p] = info[p << 1] + info[p << 1 | 1];
    }

    void build(int p, int l, int r, const vector<ll> &a) {
        if (l == r) {
            info[p] = Info(a[l]);
            return;
        }

        int mid = (l + r) >> 1;

        build(p << 1, l, mid, a);
        build(p << 1 | 1, mid + 1, r, a);

        pushup(p);
    }

    void modify(int p, int l, int r, int x, const Info &v) {
        if (l == r) {
            info[p] = v;
            return;
        }

        int mid = (l + r) >> 1;

        if (x <= mid)
            modify(p << 1, l, mid, x, v);
        else
            modify(p << 1 | 1, mid + 1, r, x, v);

        pushup(p);
    }

    void modify(int x, const Info &v) {
        modify(1, 1, n, x, v);
    }

    Info query(int p, int l, int r, int ql, int qr) const {
        if (ql <= l && r <= qr)
            return info[p];

        int mid = (l + r) >> 1;

        if (qr <= mid)
            return query(p << 1, l, mid, ql, qr);

        if (ql > mid)
            return query(p << 1 | 1, mid + 1, r, ql, qr);

        return query(p << 1, l, mid, ql, qr)
             + query(p << 1 | 1, mid + 1, r, ql, qr);
    }

    Info query(int l, int r) const {
        return query(1, 1, n, l, r);
    }
};
```

### 现场记忆

单点修改一路递归到叶子，直接 `info[p] = v`。

回溯时不断 `pushup()` 重算祖先。

无懒标记，因此没有 `Tag / apply / pushdown`。

换题通常只改两处：

`Info`：节点维护的信息  
`operator+`：左右区间如何合并


## 4.16 李超线段树：整数最大值

> **用途**：动态加入直线 `y = kx + b`，查询整数横坐标处的最大值；支持只在指定横坐标区间生效的线段。
> **复杂度**：加整条直线 `O(log X)`，查询 `O(log X)`，加线段最坏 `O(log^2 X)`，`X = R-L+1`
> **横坐标**：整数闭区间 `[L, R]`
> **限制**：查询点必须在 `[L, R]`；最终答案需落在 `long long` 范围内。

### 调用

```cpp
LiChao t(L, R);

t.add_line(k, b);
t.add_seg(k, b, ql, qr);

long long ans = t.query(x);
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `init(L, R)` | 清空并设置整数横坐标范围 |
| `add_line(k, b)` | 全区间加入 `y = kx + b` |
| `add_seg(k, b, l, r)` | 仅在 `[l, r]` 加入直线 |
| `query(x)` | 查询 `x` 处最大值 |

### 模板

```cpp
struct LiChao {
    using ll = long long;
    using i128 = __int128_t;

    static constexpr ll NEG_INF = std::numeric_limits<ll>::lowest();

    struct Line {
        ll k = 0;
        ll b = NEG_INF;

        i128 eval(ll x) const {
            return (i128)k * x + b;
        }
    };

    struct Node {
        int lc = 0;
        int rc = 0;
        Line line;
    };

    ll L = 0, R = -1;
    std::vector<Node> tr;

    LiChao() = default;

    LiChao(ll l, ll r) {
        init(l, r);
    }

    void init(ll l, ll r) {
        L = l;
        R = r;
        tr.assign(2, Node{});
    }

    void add_line(ll k, ll b) {
        insert_line(1, L, R, Line{k, b});
    }

    void add_seg(ll k, ll b, ll ql, ll qr) {
        ql = std::max(ql, L);
        qr = std::min(qr, R);
        if (ql > qr) return;

        insert_seg(1, L, R, ql, qr, Line{k, b});
    }

    ll query(ll x) const {
        return (ll)query_impl(1, L, R, x);
    }

private:
    static ll midpoint(ll l, ll r) {
        return l + (ll)(((i128)r - l) / 2);
    }

    int new_node() {
        tr.push_back(Node{});
        return (int)tr.size() - 1;
    }

    void insert_line(int p, ll l, ll r, Line line) {
        ll mid = midpoint(l, r);

        bool lef = line.eval(l) > tr[p].line.eval(l);
        bool mid_better = line.eval(mid) > tr[p].line.eval(mid);

        if (mid_better)
            std::swap(line, tr[p].line);

        if (l == r)
            return;

        if (lef != mid_better) {
            if (!tr[p].lc) {
                int q = new_node();
                tr[p].lc = q;
            }
            insert_line(tr[p].lc, l, mid, line);
        } else {
            if (!tr[p].rc) {
                int q = new_node();
                tr[p].rc = q;
            }
            insert_line(tr[p].rc, mid + 1, r, line);
        }
    }

    void insert_seg(int p, ll l, ll r, ll ql, ll qr, Line line) {
        if (ql <= l && r <= qr) {
            insert_line(p, l, r, line);
            return;
        }

        ll mid = midpoint(l, r);

        if (ql <= mid) {
            if (!tr[p].lc) {
                int q = new_node();
                tr[p].lc = q;
            }
            insert_seg(tr[p].lc, l, mid, ql, qr, line);
        }

        if (qr > mid) {
            if (!tr[p].rc) {
                int q = new_node();
                tr[p].rc = q;
            }
            insert_seg(tr[p].rc, mid + 1, r, ql, qr, line);
        }
    }

    i128 query_impl(int p, ll l, ll r, ll x) const {
        if (!p)
            return (i128)NEG_INF;

        i128 res = tr[p].line.eval(x);

        if (l == r)
            return res;

        ll mid = midpoint(l, r);

        if (x <= mid)
            return std::max(res, query_impl(tr[p].lc, l, mid, x));

        return std::max(res, query_impl(tr[p].rc, mid + 1, r, x));
    }
};
```

### 现场记忆

```text
每个节点保留“在 mid 更优”的直线。

插入新线：
先记录它在 l、mid 是否更优；
若 mid 更优，交换新线和节点线。

l 与 mid 的优劣关系不同 -> 新线去左边；
否则 -> 去右边。

区间加线 = 线段树区间分解 + 李超插线，所以最坏 O(log^2 X)。
```



## 4.17 李超线段树：浮点系数最大值（整数横坐标）

> **用途**：动态加入浮点系数直线 `y = kx + b`，查询整数横坐标处的最大值；支持区间生效的线段。
> **复杂度**：加整条直线 `O(log X)`，查询 `O(log X)`，加线段最坏 `O(log^2 X)`，`X = R-L+1`
> **横坐标**：整数闭区间 `[L, R]`
> **注意**：比较优劣直接使用 `>`，不要用 `EPS` 改变李超树的递归判断；精度要求高时可整体改为 `long double`。

### 调用

```cpp
LiChaoD t(L, R);

t.add_line(k, b);
t.add_seg(k, b, ql, qr);

double ans = t.query(x);
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `init(L, R)` | 清空并设置整数横坐标范围 |
| `add_line(k, b)` | 全区间加入 `y = kx + b` |
| `add_seg(k, b, l, r)` | 仅在 `[l, r]` 加入直线 |
| `query(x)` | 查询 `x` 处最大值 |

### 模板

```cpp
struct LiChaoD {
    using ll = long long;

    static constexpr double NEG_INF =
        -std::numeric_limits<double>::infinity();

    struct Line {
        double k = 0;
        double b = NEG_INF;

        double eval(ll x) const {
            return k * x + b;
        }
    };

    struct Node {
        int lc = 0;
        int rc = 0;
        Line line;
    };

    ll L = 0, R = -1;
    std::vector<Node> tr;

    LiChaoD() = default;

    LiChaoD(ll l, ll r) {
        init(l, r);
    }

    void init(ll l, ll r) {
        L = l;
        R = r;
        tr.assign(2, Node{});
    }

    void add_line(double k, double b) {
        insert_line(1, L, R, Line{k, b});
    }

    void add_seg(double k, double b, ll ql, ll qr) {
        ql = std::max(ql, L);
        qr = std::min(qr, R);
        if (ql > qr) return;

        insert_seg(1, L, R, ql, qr, Line{k, b});
    }

    double query(ll x) const {
        return query_impl(1, L, R, x);
    }

private:
    static ll midpoint(ll l, ll r) {
        return l + (ll)(((__int128_t)r - l) / 2);
    }

    int new_node() {
        tr.push_back(Node{});
        return (int)tr.size() - 1;
    }

    void insert_line(int p, ll l, ll r, Line line) {
        ll mid = midpoint(l, r);

        bool lef = line.eval(l) > tr[p].line.eval(l);
        bool mid_better = line.eval(mid) > tr[p].line.eval(mid);

        if (mid_better)
            std::swap(line, tr[p].line);

        if (l == r)
            return;

        if (lef != mid_better) {
            if (!tr[p].lc) {
                int q = new_node();
                tr[p].lc = q;
            }
            insert_line(tr[p].lc, l, mid, line);
        } else {
            if (!tr[p].rc) {
                int q = new_node();
                tr[p].rc = q;
            }
            insert_line(tr[p].rc, mid + 1, r, line);
        }
    }

    void insert_seg(int p, ll l, ll r, ll ql, ll qr, Line line) {
        if (ql <= l && r <= qr) {
            insert_line(p, l, r, line);
            return;
        }

        ll mid = midpoint(l, r);

        if (ql <= mid) {
            if (!tr[p].lc) {
                int q = new_node();
                tr[p].lc = q;
            }
            insert_seg(tr[p].lc, l, mid, ql, qr, line);
        }

        if (qr > mid) {
            if (!tr[p].rc) {
                int q = new_node();
                tr[p].rc = q;
            }
            insert_seg(tr[p].rc, mid + 1, r, ql, qr, line);
        }
    }

    double query_impl(int p, ll l, ll r, ll x) const {
        if (!p)
            return NEG_INF;

        double res = tr[p].line.eval(x);

        if (l == r)
            return res;

        ll mid = midpoint(l, r);

        if (x <= mid)
            return std::max(res, query_impl(tr[p].lc, l, mid, x));

        return std::max(res, query_impl(tr[p].rc, mid + 1, r, x));
    }
};
```

### 现场记忆

```text
结构与整数李超树完全相同，只是 k、b、答案改成浮点。

节点仍保存 mid 处更优的线：
mid 更优先 swap；
l / mid 优劣不同走左，否则走右。

这里 x 仍是整数离散坐标，不是连续实数域。
不要用 EPS 改写“谁更优”的严格比较。
```

## 4.18 栈维护凸包最值

> **用途**：斜率按非降序加入直线，维护 `y = kx + b` 的最大值；任意查询点可二分，查询点非降时可用单调指针。
> **复杂度**：加线均摊 `O(1)`；`query(x)` 为 `O(log n)`；`query_mono(x)` 在 `x` 非降时均摊 `O(1)`
> **限制**：加入直线的斜率 `k` 必须非降；`query_mono()` 的查询横坐标必须非降；最终答案需落在 `long long` 范围内。

### 调用

```cpp
LineHull cht;

for (...) {
    cht.add_line(k, b);  // k 非降
}

long long ans1 = cht.query(x);       // x 任意
long long ans2 = cht.query_mono(x);  // x 非降
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `clear()` | 清空凸包和单调指针 |
| `add_line(k, b)` | 按非降斜率加入直线 |
| `query(x)` | 任意 `x` 二分查询最大值 |
| `query_mono(x)` | `x` 非降时单调指针查询 |

### 模板

```cpp
struct LineHull {
    using ll = long long;
    using i128 = __int128_t;

    static constexpr ll NEG_INF = std::numeric_limits<ll>::lowest();

    struct Line {
        ll k = 0;
        ll b = 0;

        i128 eval(ll x) const {
            return (i128)k * x + b;
        }
    };

    std::vector<Line> hull;
    int ptr = 0;

    void clear() {
        hull.clear();
        ptr = 0;
    }

    void add_line(ll k, ll b) {
        Line line{k, b};

        if (!hull.empty() && hull.back().k == k) {
            if (hull.back().b >= b)
                return;
            pop_back();
        }

        while ((int)hull.size() >= 2 &&
               bad(hull[(int)hull.size() - 2], hull.back(), line)) {
            pop_back();
        }

        hull.push_back(line);
    }

    ll query(ll x) const {
        if (hull.empty())
            return NEG_INF;

        int l = 0, r = (int)hull.size() - 1;

        while (l < r) {
            int mid = (l + r) >> 1;

            if (hull[mid].eval(x) <= hull[mid + 1].eval(x))
                l = mid + 1;
            else
                r = mid;
        }

        return (ll)hull[l].eval(x);
    }

    ll query_mono(ll x) {
        if (hull.empty())
            return NEG_INF;

        ptr = std::min(ptr, (int)hull.size() - 1);

        while (ptr + 1 < (int)hull.size() &&
               hull[ptr + 1].eval(x) >= hull[ptr].eval(x)) {
            ++ptr;
        }

        return (ll)hull[ptr].eval(x);
    }

private:
    static bool bad(const Line &a, const Line &b, const Line &c) {
        return ((i128)a.b - b.b) * ((i128)c.k - b.k) >=
               ((i128)b.b - c.b) * ((i128)b.k - a.k);
    }

    void pop_back() {
        hull.pop_back();

        if (hull.empty())
            ptr = 0;
        else
            ptr = std::min(ptr, (int)hull.size() - 1);
    }
};
```

### 现场记忆

```text
最大值 + 斜率递增。

同斜率只保留 b 更大的线。

三条线 a,b,c：
若交点 x(a,b) >= x(b,c)，则 b 永远无用，弹掉。
交叉乘必须用 __int128。

任意 x：比较相邻函数值二分峰值。
x 非降：while next(x) >= cur(x)，指针右移。
```

## 4.19 动态开点线段树：区间赋值 + 剩余数量

> **用途**：在巨大整数值域上维护 `0/1` 状态，支持区间赋值，并快速查询全局剩余数量。
> **复杂度**：区间赋值 `O(log U)`，查询总剩余数量 `O(1)`；空间 `O(q log U)`
> **下标**：`1-indexed`，值域 `[1, n]`
> **约定**：初始所有位置均为 `1`；`1` 表示剩余，`0` 表示占用。

### 调用

```cpp
DynSeg seg(n);

seg.assign(l, r, 0);  // 占用
seg.assign(l, r, 1);  // 恢复

long long left = seg.remain();
long long used = seg.occupied();
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `init(n)` | 重置为 `[1,n]` 全部剩余 |
| `assign(l,r,v)` | 将 `[l,r]` 全部赋为 `v` |
| `remain()` | 当前剩余数量 |
| `occupied()` | 当前占用数量 |

### 模板

```cpp
struct DynSeg {
    using ll = long long;

    struct Node {
        int lc = 0;
        int rc = 0;
        ll sum = 0;
        int tag = -1;
    };

    ll n = 0;
    int root = 0;
    std::vector<Node> tr;

    DynSeg() = default;

    DynSeg(ll n) {
        init(n);
    }

    void init(ll n_) {
        n = n_;
        tr.assign(1, Node{});
        root = n ? new_node(1, n, 1) : 0;
    }

    void assign(ll ql, ll qr, int value) {
        if (!root) return;

        ql = std::max(ql, 1LL);
        qr = std::min(qr, n);
        if (ql > qr) return;

        root = assign_impl(root, 1, n, ql, qr, value);
    }

    ll remain() const {
        return root ? tr[root].sum : 0;
    }

    ll occupied() const {
        return n - remain();
    }

private:
    int new_node(ll l, ll r, int value) {
        tr.push_back(Node{});
        int p = (int)tr.size() - 1;
        tr[p].sum = value * (r - l + 1);
        tr[p].tag = value;
        return p;
    }

    void apply(int p, ll l, ll r, int value) {
        tr[p].sum = value * (r - l + 1);
        tr[p].tag = value;
    }

    void push_down(int p, ll l, ll r) {
        if (tr[p].tag == -1 || l == r)
            return;

        ll mid = l + (r - l) / 2;
        int tag = tr[p].tag;

        int lc = tr[p].lc;
        int rc = tr[p].rc;

        if (!lc)
            lc = new_node(l, mid, tag);
        else
            apply(lc, l, mid, tag);

        if (!rc)
            rc = new_node(mid + 1, r, tag);
        else
            apply(rc, mid + 1, r, tag);

        tr[p].lc = lc;
        tr[p].rc = rc;
        tr[p].tag = -1;
    }

    void pull(int p) {
        tr[p].sum = tr[tr[p].lc].sum + tr[tr[p].rc].sum;
    }

    int assign_impl(int p, ll l, ll r,
                    ll ql, ll qr, int value) {
        if (!p)
            p = new_node(l, r, 1);

        if (ql <= l && r <= qr) {
            apply(p, l, r, value);
            return p;
        }

        push_down(p, l, r);

        ll mid = l + (r - l) / 2;

        if (ql <= mid)
            tr[p].lc = assign_impl(tr[p].lc, l, mid,
                                   ql, qr, value);

        if (qr > mid)
            tr[p].rc = assign_impl(tr[p].rc, mid + 1, r,
                                   ql, qr, value);

        pull(p);
        return p;
    }
};
```

### 现场记忆

```text
节点新建时代表整段全为 1，不是“未知”。

tag = 0/1：整段统一；
tag = -1：已经下传、左右状态不同。

部分修改前必须 push_down：
未创建的儿子要继承父亲当前 tag。

不要把 tr[p].lc / rc 按引用传进会 push_back 的递归函数。
```


## 4.20 线段树合并：点计数 + 区间最值

> **用途**：维护多棵动态权值线段树，支持单点计数，并将两棵树直接合并；常用于子树颜色统计等问题。
> **复杂度**：单点修改 `O(log V)`；一次合并与实际同时存在的节点数有关，若每棵树只被破坏性合并一次，总合并复杂度 `O(总节点数)`
> **下标**：权值域 `[1, V]`
> **限制**：默认计数保持非负；最大值相同时返回较小的权值。
> **注意**：`merge(x,y)` 为破坏性合并，返回的新根可能直接复用 `y` 的节点，合并后不要再独立使用旧树 `y`。

### 调用

```cpp
SegMerge seg(n, V);

seg.add(u, value, 1);

seg.root[u] = seg.merge(seg.root[u], seg.root[v]);

int best_value = seg.tr[seg.root[u]].pos;
long long best_cnt = seg.tr[seg.root[u]].mx;
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `init(n,V)` | 初始化 `n` 个根，权值域 `[1,V]` |
| `add(id,pos,delta)` | 第 `id` 棵树的 `pos` 增加 `delta` |
| `merge(x,y)` | 合并两个根并返回新根 |
| `root[id]` | 第 `id` 棵线段树的根 |
| `tr[p].mx` | 当前区间最大计数 |
| `tr[p].pos` | 最大计数对应的最小权值 |

### 模板

```cpp
struct SegMerge {
    using ll = long long;

    struct Node {
        int lc = 0;
        int rc = 0;
        ll mx = 0;
        int pos = 0;
    };

    int V = 0;
    std::vector<Node> tr;
    std::vector<int> root;

    SegMerge() = default;

    SegMerge(int n, int V) {
        init(n, V);
    }

    void init(int n, int V_) {
        V = V_;
        tr.assign(1, Node{});
        root.assign(n + 1, 0);
    }

    void add(int id, int pos, ll delta) {
        root[id] = add_impl(root[id], 1, V, pos, delta);
    }

    int merge(int x, int y) {
        return merge_impl(x, y, 1, V);
    }

private:
    int new_node() {
        tr.push_back(Node{});
        return (int)tr.size() - 1;
    }

    void pull(int p) {
        int l = tr[p].lc;
        int r = tr[p].rc;

        if (!l) {
            tr[p].mx = tr[r].mx;
            tr[p].pos = tr[r].pos;
        } else if (!r) {
            tr[p].mx = tr[l].mx;
            tr[p].pos = tr[l].pos;
        } else if (tr[l].mx >= tr[r].mx) {
            tr[p].mx = tr[l].mx;
            tr[p].pos = tr[l].pos;
        } else {
            tr[p].mx = tr[r].mx;
            tr[p].pos = tr[r].pos;
        }
    }

    int add_impl(int p, int l, int r,
                 int pos, ll delta) {
        if (!p)
            p = new_node();

        if (l == r) {
            tr[p].mx += delta;
            tr[p].pos = l;
            return p;
        }

        int mid = l + (r - l) / 2;

        if (pos <= mid)
            tr[p].lc = add_impl(tr[p].lc, l, mid,
                                pos, delta);
        else
            tr[p].rc = add_impl(tr[p].rc, mid + 1, r,
                                pos, delta);

        pull(p);
        return p;
    }

    int merge_impl(int x, int y, int l, int r) {
        if (!x || !y)
            return x | y;

        if (l == r) {
            tr[x].mx += tr[y].mx;
            tr[x].pos = l;
            return x;
        }

        int mid = l + (r - l) / 2;

        tr[x].lc = merge_impl(tr[x].lc, tr[y].lc,
                              l, mid);
        tr[x].rc = merge_impl(tr[x].rc, tr[y].rc,
                              mid + 1, r);

        pull(x);
        return x;
    }
};
```

### 现场记忆

```text
单点修改一路动态开点，叶子保存该权值出现次数。

merge(x,y)：
一边为空 -> 直接复用另一边；
叶子 -> 次数相加；
否则左右儿子分别递归合并，再 pull。

pull 保存区间最大次数；
相同次数优先左儿子 -> 权值更小。

merge 是破坏性的，旧的 y 不要再单独修改或使用。
```

## 主席树：Info 单点修改

> **用途**：维护序列的多个历史版本，支持单点修改和区间查询；适合可持久化数组、历史区间和、最值等问题。

> **复杂度**：建树 `O(n)`；单次修改、查询 `O(log n)`；每次修改新增 `O(log n)` 个节点。

> **下标**：序列位置默认 `1-indexed`。

> **限制**：`Info()` 必须是信息合并的单位元，`operator +` 定义左右区间信息合并。

> **注意**：
> - `modify()` 是单点赋值。
> - 初始序列全部为 `Info()` 时，可以直接令 `root[0] = 0`，无需建树。

### 调用

有初始数组：

```cpp
PST pst;

vector<Info> a(n + 1);
for (int i = 1; i <= n; i++)
{
    a[i] = Info(x);
}

vector<int> root(q + 1);

root[0] = pst.build(a, 1, n);
root[1] = pst.modify(root[0], 1, n, pos, Info(x));

Info ans = pst.query(root[1], 1, n, l, r);
```

初始全部为单位元：

```cpp
root[0] = 0;
root[i] = pst.modify(root[ver], 1, n, pos, Info(x));
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `build(a,l,r)` | 建立初始版本 |
| `modify(pre,l,r,pos,v)` | 基于 `pre` 单点赋值，返回新根 |
| `query(root,l,r,L,R)` | 查询某个版本区间信息 |
| `Info()` | 合并单位元 |
| `operator +` | 合并左右区间信息 |

### 模板

```cpp
struct Info
{
    ll sum = 0;

    Info() {}

    Info(ll x)
    {
        sum = x;
    }

    friend Info operator + (const Info &a, const Info &b)
    {
        return Info(a.sum + b.sum);
    }
};

struct PST
{
    struct Node
    {
        int lc = 0;
        int rc = 0;
        Info info;
    };

    vector<Node> tr;

    PST()
    {
        init();
    }

    void init()
    {
        tr.assign(1, {});
    }

    int clone(int p)
    {
        tr.push_back(tr[p]);
        return (int)tr.size() - 1;
    }

    void pull(int p)
    {
        tr[p].info = tr[tr[p].lc].info + tr[tr[p].rc].info;
    }

    int build(const vector<Info> &a, int l, int r)
    {
        int p = clone(0);

        if (l == r)
        {
            tr[p].info = a[l];
            return p;
        }

        int mid = (l + r) >> 1;
        tr[p].lc = build(a, l, mid);
        tr[p].rc = build(a, mid + 1, r);
        pull(p);

        return p;
    }

    int modify(int pre, int l, int r, int pos, const Info &v)
    {
        int cur = clone(pre);

        if (l == r)
        {
            tr[cur].info = v;
            return cur;
        }

        int mid = (l + r) >> 1;

        if (pos <= mid)
        {
            tr[cur].lc = modify(tr[pre].lc, l, mid, pos, v);
        }
        else
        {
            tr[cur].rc = modify(tr[pre].rc, mid + 1, r, pos, v);
        }

        pull(cur);

        return cur;
    }

    Info query(int p, int l, int r, int L, int R) const
    {
        if (L <= l && r <= R)
        {
            return tr[p].info;
        }

        int mid = (l + r) >> 1;
        Info ans;

        if (L <= mid)
        {
            ans = ans + query(tr[p].lc, l, mid, L, R);
        }

        if (R > mid)
        {
            ans = ans + query(tr[p].rc, mid + 1, r, L, R);
        }

        return ans;
    }
};
```

### 现场记忆

```text
位置域主席树。

modify：
clone 当前节点，只复制根到叶子的路径。

叶子直接赋值，回溯 pull。

root[new] = modify(root[old], ...)
旧版本完全不变。
```

## 主席树：InfoTag 区间修改

> **用途**：维护序列的多个历史版本，支持区间修改、区间查询和历史版本访问。

> **复杂度**：建树 `O(n)`；单次区间修改、查询 `O(log n)`；每次修改新增 `O(log n)` 个节点。

> **下标**：序列位置默认 `1-indexed`。

> **限制**：
> - `Info()` 必须是信息合并单位元。
> - `Tag()` 必须是空标记。
> - 修改 `Tag` 时需要同步修改 `Info::apply()`、`Tag::apply()` 和 `Tag::empty()`。

> **注意**：
> - `push()` 时不能直接修改旧儿子，必须先 `clone()`。
> - `query()` 不下传、不创建节点，而是携带祖先懒标记。
> - 如果 Tag 不满足交换律，`tag.apply(t)` 的语义固定为：在当前 Tag 之后再执行 `t`。

### 调用

有初始数组：

```cpp
PST pst;

vector<Info> a(n + 1);
for (int i = 1; i <= n; i++)
{
    a[i] = Info(x);
}

vector<int> root(q + 1);

root[0] = pst.build(a, 1, n);
root[1] = pst.modify(root[0], 1, n, l, r, Tag(x));

Info ans = pst.query(root[1], 1, n, l, r);
```

初始全部为单位元：

```cpp
root[0] = 0;
root[i] = pst.modify(root[ver], 1, n, l, r, Tag(x));
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `build(a,l,r)` | 建立初始版本 |
| `modify(pre,l,r,L,R,tag)` | 基于旧版本区间修改 |
| `query(root,l,r,L,R)` | 查询某个版本区间信息 |
| `Info::apply()` | 标记作用到区间信息 |
| `Tag::apply()` | 合并两个懒标记 |
| `Tag::empty()` | 判断是否为空标记 |

### 模板

```cpp
struct Tag
{
    ll add = 0;

    Tag() {}

    Tag(ll x)
    {
        add = x;
    }

    bool empty() const
    {
        return add == 0;
    }

    void apply(const Tag &tag)
    {
        add += tag.add;
    }
};

struct Info
{
    ll sum = 0;

    Info() {}

    Info(ll x)
    {
        sum = x;
    }

    void apply(const Tag &tag, int l, int r)
    {
        sum += tag.add * (r - l + 1);
    }

    friend Info operator + (const Info &a, const Info &b)
    {
        return Info(a.sum + b.sum);
    }
};

struct PST
{
    struct Node
    {
        int lc = 0;
        int rc = 0;
        Info info;
        Tag tag;
    };

    vector<Node> tr;

    PST()
    {
        init();
    }

    void init()
    {
        tr.assign(1, {});
    }

    int clone(int p)
    {
        tr.push_back(tr[p]);
        return (int)tr.size() - 1;
    }

    void pull(int p)
    {
        tr[p].info = tr[tr[p].lc].info + tr[tr[p].rc].info;
    }

    void apply(int p, int l, int r, const Tag &tag)
    {
        tr[p].info.apply(tag, l, r);
        tr[p].tag.apply(tag);
    }

    void push(int p, int l, int r)
    {
        if (l == r || tr[p].tag.empty())
        {
            return;
        }

        int mid = (l + r) >> 1;

        tr[p].lc = clone(tr[p].lc);
        tr[p].rc = clone(tr[p].rc);

        apply(tr[p].lc, l, mid, tr[p].tag);
        apply(tr[p].rc, mid + 1, r, tr[p].tag);

        tr[p].tag = {};
    }

    int build(const vector<Info> &a, int l, int r)
    {
        int p = clone(0);

        if (l == r)
        {
            tr[p].info = a[l];
            return p;
        }

        int mid = (l + r) >> 1;
        tr[p].lc = build(a, l, mid);
        tr[p].rc = build(a, mid + 1, r);
        pull(p);

        return p;
    }

    int modify(int pre, int l, int r, int L, int R, const Tag &tag)
    {
        int cur = clone(pre);

        if (L <= l && r <= R)
        {
            apply(cur, l, r, tag);
            return cur;
        }

        push(cur, l, r);

        int mid = (l + r) >> 1;

        if (L <= mid)
        {
            tr[cur].lc = modify(tr[cur].lc, l, mid, L, R, tag);
        }

        if (R > mid)
        {
            tr[cur].rc = modify(tr[cur].rc, mid + 1, r, L, R, tag);
        }

        pull(cur);

        return cur;
    }

    Info query(int p, int l, int r, int L, int R, Tag tag = {}) const
    {
        if (L <= l && r <= R)
        {
            Info res = tr[p].info;
            res.apply(tag, l, r);
            return res;
        }

        Tag nxt = tr[p].tag;
        nxt.apply(tag);

        int mid = (l + r) >> 1;
        Info ans;

        if (L <= mid)
        {
            ans = ans + query(tr[p].lc, l, mid, L, R, nxt);
        }

        if (R > mid)
        {
            ans = ans + query(tr[p].rc, mid + 1, r, L, R, nxt);
        }

        return ans;
    }
};
```

### 现场记忆

```text
InfoTag 主席树 = Lazy Segment Tree + 路径复制。

modify 先 clone 当前节点。

push 时：
两个儿子都先 clone，再 apply。

query 不 push，不产生新节点。

非交换 Tag：
当前节点 tag 先执行，祖先携带 tag 后执行。
```

## 主席树：值域计数

> **用途**：维护前缀值域频率，支持区间第 k 小、排名、频率、前驱后继等查询。

> **复杂度**：建树 `O(n log V)`；单次修改、查询、第 k 小 `O(log V)`；空间 `O(n log V)`。

> **下标**：原数组默认 `1-indexed`；线段树维护离散化值域 `[1,m]`。

> **限制**：使用前需要把所有可能出现的值加入离散化。

> **注意**：
> - `root[r] - root[l - 1]` 表示数组区间 `[l,r]` 的值域多重集。
> - `kth()` 要保证 `1 <= k <= r-l+1`。

### 调用

```cpp
vector<int> b(a.begin() + 1, a.end());

sort(b.begin(), b.end());
b.erase(unique(b.begin(), b.end()), b.end());

int m = b.size();

PST pst;
vector<int> root(n + 1);

for (int i = 1; i <= n; i++)
{
    int pos = lower_bound(b.begin(), b.end(), a[i]) - b.begin() + 1;
    root[i] = pst.modify(root[i - 1], 1, m, pos, 1);
}

int id = pst.kth(root[l - 1], root[r], 1, m, k);
int ans = b[id - 1];
```

查询 `<= x` 的数量：

```cpp
int p = upper_bound(b.begin(), b.end(), x) - b.begin();
int ans = p ? pst.query(root[l - 1], root[r], 1, m, 1, p) : 0;
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `modify(pre,l,r,pos,v)` | 某个值的出现次数加 `v` |
| `query(u,v,l,r,L,R)` | 查询版本差中值域 `[L,R]` 的数量 |
| `kth(u,v,l,r,k)` | 查询版本差中的第 `k` 小 |
| `count(u,v)` | 两版本计数总差 |

### 模板

```cpp
struct PST
{
    struct Node
    {
        int lc = 0;
        int rc = 0;
        int cnt = 0;
    };

    vector<Node> tr;

    PST()
    {
        init();
    }

    void init()
    {
        tr.assign(1, {});
    }

    int clone(int p)
    {
        tr.push_back(tr[p]);
        return (int)tr.size() - 1;
    }

    int modify(int pre, int l, int r, int pos, int v)
    {
        int cur = clone(pre);
        tr[cur].cnt += v;

        if (l == r)
        {
            return cur;
        }

        int mid = (l + r) >> 1;

        if (pos <= mid)
        {
            tr[cur].lc = modify(tr[pre].lc, l, mid, pos, v);
        }
        else
        {
            tr[cur].rc = modify(tr[pre].rc, mid + 1, r, pos, v);
        }

        return cur;
    }

    int query(int u, int v, int l, int r, int L, int R) const
    {
        if (L <= l && r <= R)
        {
            return tr[v].cnt - tr[u].cnt;
        }

        int mid = (l + r) >> 1;
        int ans = 0;

        if (L <= mid)
        {
            ans += query(tr[u].lc, tr[v].lc, l, mid, L, R);
        }

        if (R > mid)
        {
            ans += query(tr[u].rc, tr[v].rc, mid + 1, r, L, R);
        }

        return ans;
    }

    int kth(int u, int v, int l, int r, int k) const
    {
        if (l == r)
        {
            return l;
        }

        int mid = (l + r) >> 1;
        int cnt = tr[tr[v].lc].cnt - tr[tr[u].lc].cnt;

        if (k <= cnt)
        {
            return kth(tr[u].lc, tr[v].lc, l, mid, k);
        }

        return kth(tr[u].rc, tr[v].rc, mid + 1, r, k - cnt);
    }

    int count(int u, int v) const
    {
        return tr[v].cnt - tr[u].cnt;
    }
};
```

### 常见查询

```text
区间 [l,r]：
u = root[l-1]
v = root[r]

第 k 小：
kth(u,v,...,k)

<= x：
upper_bound 得到最后一个 <= x 的离散位置，再 query 前缀。

[x,y] 出现次数：
query 对应离散值域。

严格前驱 < x：
c = 区间中 < x 的数量
答案 = kth(c)

严格后继 > x：
c = 区间中 <= x 的数量
答案 = kth(c+1)
```

### 现场记忆

```text
值域主席树的核心是版本差。

root[r] - root[l-1] = [l,r] 的值域频率。

kth：
看左儿子的计数差。

左边够 k -> 左走。
否则右走，k -= 左边数量。
```

## 主席树：区间不同颜色数量

> **用途**：在线查询静态数组区间 `[l,r]` 中不同值的数量。

> **复杂度**：建树 `O(n log n)`，单次查询 `O(log n)`，空间 `O(n log n)`。

> **下标**：数组位置和线段树位置均为 `1-indexed`。

> **注意**：
> - 这是位置域主席树，不是值域计数主席树。
> - 对于版本 `root[r]`，每种颜色只在最后出现位置保留一个 `1`。

### 调用

```cpp
PSTColor pst;

pst.build(a);

int ans = pst.query(l, r);
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `build(a)` | 建立所有前缀版本 |
| `query(l,r)` | 查询 `[l,r]` 不同值数量 |
| `root[r]` | 处理完前缀 `[1,r]` 的版本 |

### 模板

```cpp
struct PSTColor
{
    struct Node
    {
        int lc = 0;
        int rc = 0;
        int sum = 0;
    };

    int n = 0;
    vector<Node> tr;
    vector<int> root;

    PSTColor()
    {
        init();
    }

    void init()
    {
        n = 0;
        tr.assign(1, {});
        root.clear();
    }

    int clone(int p)
    {
        tr.push_back(tr[p]);
        return (int)tr.size() - 1;
    }

    int modify(int pre, int l, int r, int pos, int v)
    {
        int cur = clone(pre);
        tr[cur].sum += v;

        if (l == r)
        {
            return cur;
        }

        int mid = (l + r) >> 1;

        if (pos <= mid)
        {
            tr[cur].lc = modify(tr[pre].lc, l, mid, pos, v);
        }
        else
        {
            tr[cur].rc = modify(tr[pre].rc, mid + 1, r, pos, v);
        }

        return cur;
    }

    int query(int p, int l, int r, int L, int R) const
    {
        if (L <= l && r <= R)
        {
            return tr[p].sum;
        }

        int mid = (l + r) >> 1;
        int ans = 0;

        if (L <= mid)
        {
            ans += query(tr[p].lc, l, mid, L, R);
        }

        if (R > mid)
        {
            ans += query(tr[p].rc, mid + 1, r, L, R);
        }

        return ans;
    }

    void build(const vector<int> &a)
    {
        init();

        n = (int)a.size() - 1;
        root.assign(n + 1, 0);

        vector<int> b(a.begin() + 1, a.end());
        sort(b.begin(), b.end());
        b.erase(unique(b.begin(), b.end()), b.end());

        vector<int> last(b.size() + 1);

        for (int i = 1; i <= n; i++)
        {
            int c = lower_bound(b.begin(), b.end(), a[i]) - b.begin() + 1;

            root[i] = root[i - 1];

            if (last[c])
            {
                root[i] = modify(root[i], 1, n, last[c], -1);
            }

            root[i] = modify(root[i], 1, n, i, 1);
            last[c] = i;
        }
    }

    int query(int l, int r) const
    {
        return query(root[r], 1, n, l, r);
    }
};
```

### 现场记忆

```text
位置 i 是否为某个颜色当前最后出现位置。

加入 a[i]：
旧 last 位置 -1
当前位置 +1

root[r] 中每种颜色只留下最后一次出现。

查询 [l,r] 内的 1 的数量即可。
```

## 主席树：区间 mex

> **用途**：查询静态数组区间 `[l,r]` 的 mex。

> **复杂度**：建树 `O(n log n)`，单次查询 `O(log n)`，空间 `O(n log n)`。

> **下标**：数组位置 `1-indexed`；值域自然维护 `[0,n]`。

> **限制**：值 `< 0` 或 `> n` 不影响长度不超过 `n` 的区间 mex，可以直接忽略。

> **注意**：
> - 叶子 `x` 保存 `last[x]`，即值 `x` 最近出现的位置。
> - 节点保存区间内最小的 `last`。
> - 空节点的 `mn = 0`，因此不需要预建完整值域树。

### 调用

```cpp
PSTMex pst;

pst.build(a);

int ans = pst.query(l, r);
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `build(a)` | 建立所有前缀版本 |
| `modify()` | 修改某个值的最后出现位置 |
| `mex(root,l,r,L)` | 找最小满足 `last[x] < L` 的值 |
| `query(l,r)` | 查询数组 `[l,r]` 的 mex |

### 模板

```cpp
struct PSTMex
{
    struct Node
    {
        int lc = 0;
        int rc = 0;
        int mn = 0;
    };

    int n = 0;
    vector<Node> tr;
    vector<int> root;

    PSTMex()
    {
        init();
    }

    void init()
    {
        n = 0;
        tr.assign(1, {});
        root.clear();
    }

    int clone(int p)
    {
        tr.push_back(tr[p]);
        return (int)tr.size() - 1;
    }

    int modify(int pre, int l, int r, int pos, int v)
    {
        int cur = clone(pre);

        if (l == r)
        {
            tr[cur].mn = v;
            return cur;
        }

        int mid = (l + r) >> 1;

        if (pos <= mid)
        {
            tr[cur].lc = modify(tr[pre].lc, l, mid, pos, v);
        }
        else
        {
            tr[cur].rc = modify(tr[pre].rc, mid + 1, r, pos, v);
        }

        tr[cur].mn = min(tr[tr[cur].lc].mn, tr[tr[cur].rc].mn);

        return cur;
    }

    int mex(int p, int l, int r, int L) const
    {
        if (l == r)
        {
            return l;
        }

        int mid = (l + r) >> 1;

        if (tr[tr[p].lc].mn < L)
        {
            return mex(tr[p].lc, l, mid, L);
        }

        return mex(tr[p].rc, mid + 1, r, L);
    }

    void build(const vector<int> &a)
    {
        init();

        n = (int)a.size() - 1;
        root.assign(n + 1, 0);

        for (int i = 1; i <= n; i++)
        {
            root[i] = root[i - 1];

            if (0 <= a[i] && a[i] <= n)
            {
                root[i] = modify(root[i - 1], 0, n, a[i], i);
            }
        }
    }

    int query(int l, int r) const
    {
        return mex(root[r], 0, n, l);
    }
};
```

### 现场记忆

```text
值域叶子 x 保存 last[x]。

查询 [L,R]：
直接使用 root[R]。

last[x] < L
说明 x 没在 [L,R] 中出现。

左儿子 mn < L -> 左走。
否则右走。
```

## 主席树：树上第 k 小

> **用途**：查询树上路径 `u -> v` 中第 `k` 小的点权。

> **复杂度**：建主席树 `O(n log V)`；单次查询 `O(log V)`，另需 LCA 查询复杂度。

> **下标**：树节点默认 `1-indexed`；值域使用离散化后的 `[1,m]`。

> **限制**：需要配合 LCA；点权需要预先离散化。

> **注意**：
> - `root[u]` 表示根节点到 `u` 路径上的值域计数。
> - 根节点父亲设为 `0`，并令 `root[0] = 0`。

### 调用

DFS 建版本时：

```cpp
int pos = lower_bound(b.begin(), b.end(), a[u]) - b.begin() + 1;
root[u] = pst.modify(root[fa[u][0]], 1, m, pos);
```

查询路径：

```cpp
int w = lca(u, v);
int p = fa[w][0];

int id = pst.kth(root[u], root[v], root[w], root[p], 1, m, k);
int ans = b[id - 1];
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `modify(pre,l,r,pos)` | 根到当前点的版本加入一个点权 |
| `kth(a,b,c,d,l,r,k)` | 查询 `a+b-c-d` 多重集中的第 `k` 小 |

### 模板

```cpp
struct PSTTreeKth
{
    struct Node
    {
        int lc = 0;
        int rc = 0;
        int cnt = 0;
    };

    vector<Node> tr;

    PSTTreeKth()
    {
        init();
    }

    void init()
    {
        tr.assign(1, {});
    }

    int clone(int p)
    {
        tr.push_back(tr[p]);
        return (int)tr.size() - 1;
    }

    int modify(int pre, int l, int r, int pos)
    {
        int cur = clone(pre);
        tr[cur].cnt++;

        if (l == r)
        {
            return cur;
        }

        int mid = (l + r) >> 1;

        if (pos <= mid)
        {
            tr[cur].lc = modify(tr[pre].lc, l, mid, pos);
        }
        else
        {
            tr[cur].rc = modify(tr[pre].rc, mid + 1, r, pos);
        }

        return cur;
    }

    int kth(int a, int b, int c, int d, int l, int r, int k) const
    {
        if (l == r)
        {
            return l;
        }

        int mid = (l + r) >> 1;
        int cnt = tr[tr[a].lc].cnt + tr[tr[b].lc].cnt - tr[tr[c].lc].cnt - tr[tr[d].lc].cnt;

        if (k <= cnt)
        {
            return kth(tr[a].lc, tr[b].lc, tr[c].lc, tr[d].lc, l, mid, k);
        }

        return kth(tr[a].rc, tr[b].rc, tr[c].rc, tr[d].rc, mid + 1, r, k - cnt);
    }
};
```

### 现场记忆

```text
root[u] = 根 -> u 的值域计数。

u -> v 路径：

root[u] + root[v]
- root[lca]
- root[fa[lca]]

之后和普通值域 kth 完全一样。
```

## 4.23 可持久化 01 Trie

> **用途**：维护多个二进制 Trie 版本，查询两个版本计数差中与 `x` 异或的最大值或最小值；前缀建版本后可直接查询数组区间。
> **复杂度**：单次修改 / 查询 `O(B)`；空间每次修改新增 `O(B)` 个节点
> **位数**：维护 `[B,0]` 共 `B+1` 位，默认 `B=30`
> **限制**：查询的两个版本计数差必须构成非空合法多重集；删除时必须保证对应元素存在。
> **注意**：`modify(from,from,...)` 会覆盖版本槽位，但其他历史版本不受影响；旧节点不会回收。

### 调用

```cpp
PTrie01 trie(n, 30);
trie.build(a);

// 区间 [l,r] 与 x 的最大 / 最小异或值
auto mx = trie.range_max_xor(l, r, x);
auto mn = trie.range_min_xor(l, r, x);

// 基于版本 3 插入 x，生成版本 7
trie.insert(3, 7, x);

// 基于版本 3 删除 x，并覆盖版本 3
trie.modify(3, 3, x, -1);
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `init(versions,B)` | 初始化版本槽位与最高位 |
| `build(a)` | 建立数组前缀版本 |
| `modify(from,to,x,delta)` | 修改 `x` 出现次数 |
| `insert(from,to,x)` | 插入一个 `x` |
| `max_xor(vl,vr,x)` | 查询版本差中的最大异或值 |
| `min_xor(vl,vr,x)` | 查询版本差中的最小异或值 |
| `range_max_xor(l,r,x)` | 查询数组区间最大异或值 |
| `range_min_xor(l,r,x)` | 查询数组区间最小异或值 |
| `root[v]` | 第 `v` 个版本根 |

### 模板

```cpp
struct PTrie01 {
    using U = unsigned long long;

    struct Node {
        int ch[2] = {};
        int cnt = 0;
    };

    int B = 30;
    std::vector<Node> tr;
    std::vector<int> root;

    PTrie01(int versions = 0, int B = 30) {
        init(versions, B);
    }

    void init(int versions = 0, int B_ = 30) {
        B = B_;
        tr.assign(1, Node{});
        root.assign(versions + 1, 0);
    }

    void build(const std::vector<U> &a) {
        int n = (int)a.size() - 1;
        init(n, B);

        for (int i = 1; i <= n; ++i)
            insert(i - 1, i, a[i]);
    }

    void modify(int from, int to, U x, int delta) {
        int pre_root = root[from];
        int new_root = clone(pre_root);

        int pre = pre_root;
        int cur = new_root;
        tr[cur].cnt += delta;

        for (int i = B; i >= 0; --i) {
            int bit = (x >> i) & 1ULL;

            int child = clone(tr[pre].ch[bit]);
            tr[cur].ch[bit] = child;

            pre = tr[pre].ch[bit];
            cur = child;

            tr[cur].cnt += delta;
        }

        if (to >= (int)root.size())
            root.resize(to + 1);

        root[to] = new_root;
    }

    void insert(int from, int to, U x) {
        modify(from, to, x, 1);
    }

    U max_xor(int left_ver, int right_ver, U x) const {
        int l = root[left_ver];
        int r = root[right_ver];
        U ans = 0;

        for (int i = B; i >= 0; --i) {
            int bit = (x >> i) & 1ULL;
            int want = bit ^ 1;

            if (count_diff(l, r, want) > 0) {
                ans |= 1ULL << i;
                l = tr[l].ch[want];
                r = tr[r].ch[want];
            } else {
                l = tr[l].ch[bit];
                r = tr[r].ch[bit];
            }
        }

        return ans;
    }

    U min_xor(int left_ver, int right_ver, U x) const {
        int l = root[left_ver];
        int r = root[right_ver];
        U ans = 0;

        for (int i = B; i >= 0; --i) {
            int bit = (x >> i) & 1ULL;

            if (count_diff(l, r, bit) > 0) {
                l = tr[l].ch[bit];
                r = tr[r].ch[bit];
            } else {
                ans |= 1ULL << i;
                int want = bit ^ 1;
                l = tr[l].ch[want];
                r = tr[r].ch[want];
            }
        }

        return ans;
    }

    U range_max_xor(int l, int r, U x) const {
        return max_xor(l - 1, r, x);
    }

    U range_min_xor(int l, int r, U x) const {
        return min_xor(l - 1, r, x);
    }

private:
    int clone(int p) {
        tr.push_back(tr[p]);
        return (int)tr.size() - 1;
    }

    int count_diff(int l, int r, int bit) const {
        int lc = tr[l].ch[bit];
        int rc = tr[r].ch[bit];
        return tr[rc].cnt - tr[lc].cnt;
    }
};
```

### 现场记忆

```text
可持久化 Trie = 每次只复制根到叶子的二进制路径。

版本区间仍然是：
root[R] - root[L-1]。

最大异或：
优先走 x 当前位的反位，前提是版本差中该分支 cnt > 0。

最小异或：
优先走同位，不存在才走反位。

modify(from, to, ...)，from == to 可直接覆盖版本槽位。
```

## 4.24 前缀线性基：区间异或最大值

> **用途**：维护数组每个前缀的线性基，查询区间 `[l,r]` 子集异或最大值。
> **复杂度**：插入 / 查询 `O(B)`，建立全部前缀 `O(nB)`，空间 `O(nB)`
> **限制**：默认维护非负 `int`，最高位为 `LOG=30`。

### 调用

```cpp
vector<LinearBasis> pre(n + 1);

for (int i = 1; i <= n; ++i) {
    pre[i] = pre[i - 1];
    pre[i].insert(a[i], i);
}

int ans = pre[r].query(l);
```

### 模板

```cpp
struct LinearBasis {
    static constexpr int LOG = 30;

    array<int, LOG + 1> p{};
    array<int, LOG + 1> pos{};

    void insert(int x, int id) {
        for (int i = LOG; i >= 0; --i) {
            if (!(x >> i & 1))
                continue;

            if (!p[i]) {
                p[i] = x;
                pos[i] = id;
                return;
            }

            if (id > pos[i]) {
                swap(x, p[i]);
                swap(id, pos[i]);
            }

            x ^= p[i];
        }
    }

    int query(int l) const {
        int res = 0;

        for (int i = LOG; i >= 0; --i) {
            if (pos[i] >= l)
                res = max(res, res ^ p[i]);
        }

        return res;
    }
};
```

### 现场记忆

```text
pre[i] = pre[i-1]，再插入 (a[i], i)。

每个最高位优先保留 pos 更大的基向量：
若新向量 id 更大，先 swap，再继续消元。

查询 [l,r] 只看 pre[r]：
仅允许 pos[i] >= l 的基向量参与贪心。
```

## 4.25 FHQ Treap：按值平衡树

> **用途**：维护可重集，支持插入、删除一个值、排名、第 `k` 小、严格前驱 / 后继。
> **复杂度**：单次操作期望 `O(log n)`，最坏 `O(n)`；空间 `O(n)`
> **下标**：`kth(k)` 中 `k` 从 `1` 开始。
> **注意**：允许重复值；`pre / nxt` 不存在时的返回值按题意修改。

### 调用

```cpp
FHQSet fhq;

fhq.insert(x);
fhq.erase(x);

int rk = fhq.rank_of(x);
int x = fhq.kth(k);
int p = fhq.pre(x);
int q = fhq.nxt(x);
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `clear()` | 清空 |
| `insert(x)` | 插入一个 `x` |
| `erase(x)` | 删除一个 `x` |
| `rank_of(x)` | `x` 的排名，即 `< x` 的数量 `+1` |
| `kth(k)` | 第 `k` 小 |
| `pre(x)` | 严格前驱 |
| `nxt(x)` | 严格后继 |
| `contains(x)` | 是否存在 |

### 模板

```cpp
struct FHQSet {
    struct Node {
        int lc = 0, rc = 0;
        int val = 0, siz = 0;
        uint32_t pri = 0;

        // 【按题意修改】sum / mx / mn ...
    };

    vector<Node> tr{Node{}};
    int root = 0;

    mt19937 rng{
        (uint32_t)chrono::steady_clock::now()
            .time_since_epoch().count()
    };

    int size(int p) const {
        return p ? tr[p].siz : 0;
    }

    void push_up(int p) {
        tr[p].siz = size(tr[p].lc) + size(tr[p].rc) + 1;

        // 【按题意修改】维护额外信息
    }

    int new_node(int val) {
        Node node;
        node.val = val;
        node.siz = 1;
        node.pri = rng();

        // 【按题意修改】初始化额外信息

        tr.push_back(node);
        return (int)tr.size() - 1;
    }

    // x: <= val, y: > val
    pair<int, int> split(int p, long long val) {
        if (!p)
            return {0, 0};

        if (tr[p].val <= val) {
            auto [x, y] = split(tr[p].rc, val);
            tr[p].rc = x;
            push_up(p);
            return {p, y};
        } else {
            auto [x, y] = split(tr[p].lc, val);
            tr[p].lc = y;
            push_up(p);
            return {x, p};
        }
    }

    int merge(int x, int y) {
        if (!x || !y)
            return x | y;

        if (tr[x].pri < tr[y].pri) {
            tr[x].rc = merge(tr[x].rc, y);
            push_up(x);
            return x;
        } else {
            tr[y].lc = merge(x, tr[y].lc);
            push_up(y);
            return y;
        }
    }

    void clear() {
        tr.assign(1, Node{});
        root = 0;
    }

    bool contains(int val) const {
        int p = root;

        while (p) {
            if (tr[p].val == val)
                return true;

            p = val < tr[p].val ? tr[p].lc : tr[p].rc;
        }

        return false;
    }

    void insert(int val) {
        auto [x, y] = split(root, val);
        root = merge(merge(x, new_node(val)), y);
    }

    void erase(int val) {
        if (!contains(val))
            return;

        auto [x, z] = split(root, val);
        auto [a, y] = split(x, (long long)val - 1);

        y = merge(tr[y].lc, tr[y].rc);
        root = merge(merge(a, y), z);
    }

    int rank_of(int val) const {
        int p = root;
        int res = 1;

        while (p) {
            if (tr[p].val >= val) {
                p = tr[p].lc;
            } else {
                res += size(tr[p].lc) + 1;
                p = tr[p].rc;
            }
        }

        return res;
    }

    int kth(int k) const {
        int p = root;

        while (p) {
            int ls = size(tr[p].lc);

            if (k <= ls) {
                p = tr[p].lc;
            } else if (k == ls + 1) {
                return tr[p].val;
            } else {
                k -= ls + 1;
                p = tr[p].rc;
            }
        }

        return 0;
    }

    int pre(int val) const {
        int p = root;
        int res = INT_MIN;  // 【按题意修改】

        while (p) {
            if (tr[p].val < val) {
                res = tr[p].val;
                p = tr[p].rc;
            } else {
                p = tr[p].lc;
            }
        }

        return res;
    }

    int nxt(int val) const {
        int p = root;
        int res = INT_MAX;  // 【按题意修改】

        while (p) {
            if (tr[p].val > val) {
                res = tr[p].val;
                p = tr[p].lc;
            } else {
                p = tr[p].rc;
            }
        }

        return res;
    }
};
```

### 现场记忆

```text
按值 split：
左树 <= val，右树 > val。

insert：
split -> 中间塞新节点 -> merge。

erase 一个 val：
先拆 <=val / >val，
再拆 <val / ==val，
把 ==val 那棵树的根删掉一个。

排名 = 小于 val 的数量 + 1。
```

## 4.26 FHQ Treap：隐式序列

> **用途**：按序列位置维护动态序列，支持插入、删除区间、区间翻转，并方便扩展区间信息和 lazy。
> **复杂度**：单次 `split / merge / 区间操作` 期望 `O(log n)`，最坏 `O(n)`；空间 `O(n)`
> **下标**：序列位置 `1-indexed`
> **注意**：`split(root,k)` 的左树恰好包含前 `k` 个元素。

### 调用

```cpp
FHQSeq fhq;
fhq.build(a);

fhq.insert(3, x);    // 插入后成为第 3 个
fhq.erase(2, 5);
fhq.reverse(2, 5);

vector<int> a = fhq.to_vector();
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `build(a)` | 按 `a` 的顺序建树 |
| `insert(pos,x)` | 插入后成为第 `pos` 个 |
| `erase(l,r)` | 删除 `[l,r]` |
| `reverse(l,r)` | 翻转 `[l,r]` |
| `to_vector()` | 导出当前序列 |
| `split(root,k)` | 拆成前 `k` 个 / 剩余部分 |
| `merge(x,y)` | 拼接两段序列 |

### 模板

```cpp
struct FHQSeq {
    struct Node {
        int lc = 0, rc = 0;
        int val = 0, siz = 0;
        uint32_t pri = 0;
        bool rev = false;

        // 【按题意修改】sum / mx / add / set ...
    };

    vector<Node> tr{Node{}};
    int root = 0;

    mt19937 rng{
        (uint32_t)chrono::steady_clock::now()
            .time_since_epoch().count()
    };

    int size(int p) const {
        return p ? tr[p].siz : 0;
    }

    void push_up(int p) {
        tr[p].siz = size(tr[p].lc) + size(tr[p].rc) + 1;

        // 【按题意修改】维护额外信息
    }

    int new_node(int val) {
        Node node;
        node.val = val;
        node.siz = 1;
        node.pri = rng();

        // 【按题意修改】初始化额外信息

        tr.push_back(node);
        return (int)tr.size() - 1;
    }

    void apply_rev(int p) {
        if (!p)
            return;

        swap(tr[p].lc, tr[p].rc);
        tr[p].rev ^= 1;
    }

    // 【按题意修改】apply_add / apply_set ...

    void push_down(int p) {
        if (!p)
            return;

        if (tr[p].rev) {
            apply_rev(tr[p].lc);
            apply_rev(tr[p].rc);
            tr[p].rev = false;
        }

        // 【按题意修改】下传其他 lazy
    }

    pair<int, int> split(int p, int k) {
        if (!p)
            return {0, 0};

        push_down(p);

        int ls = size(tr[p].lc);

        if (k <= ls) {
            auto [x, y] = split(tr[p].lc, k);
            tr[p].lc = y;
            push_up(p);
            return {x, p};
        } else {
            auto [x, y] = split(tr[p].rc, k - ls - 1);
            tr[p].rc = x;
            push_up(p);
            return {p, y};
        }
    }

    int merge(int x, int y) {
        if (!x || !y)
            return x | y;

        if (tr[x].pri < tr[y].pri) {
            push_down(x);
            tr[x].rc = merge(tr[x].rc, y);
            push_up(x);
            return x;
        } else {
            push_down(y);
            tr[y].lc = merge(x, tr[y].lc);
            push_up(y);
            return y;
        }
    }

    void clear() {
        tr.assign(1, Node{});
        root = 0;
    }

    void build(const vector<int> &a) {
        clear();

        for (int x : a)
            root = merge(root, new_node(x));
    }

    void insert(int pos, int val) {
        auto [x, y] = split(root, pos - 1);
        root = merge(merge(x, new_node(val)), y);
    }

    void erase(int l, int r) {
        auto [x, yz] = split(root, l - 1);
        auto [y, z] = split(yz, r - l + 1);

        root = merge(x, z);
    }

    void reverse(int l, int r) {
        auto [x, yz] = split(root, l - 1);
        auto [y, z] = split(yz, r - l + 1);

        apply_rev(y);
        root = merge(x, merge(y, z));
    }

    void inorder(int p, vector<int> &a) {
        if (!p)
            return;

        push_down(p);
        inorder(tr[p].lc, a);
        a.push_back(tr[p].val);
        inorder(tr[p].rc, a);
    }

    vector<int> to_vector() {
        vector<int> a;
        a.reserve(size(root));
        inorder(root, a);
        return a;
    }
};
```

### 现场记忆

```text
隐式 FHQ 不按 val split，而按 siz split。

split(root,k)：
左边前 k 个，右边剩余。

区间 [l,r]：
split(root,l-1)
split(mid,r-l+1)
操作中间树
再 merge 回去。

加 lazy 时只改 Node / apply_xxx / push_down / push_up。
```

## 4.27 可持久化 FHQ Treap：按值平衡树

> **用途**：维护多个历史版本的可重集，支持插入、删除、排名、第 `k` 小、前驱 / 后继。
> **复杂度**：单次修改期望 `O(log n)`，新增期望 `O(log n)` 个节点；查询期望 `O(log n)`
> **版本**：`modify(from,to,...)` 风格，`from == to` 可直接覆盖版本槽位。
> **注意**：所有修改路径都必须复制；覆盖版本槽位不会影响其他历史版本，但旧节点不会回收。

### 调用

```cpp
PersistentFHQSet fhq;

// version 0 为空

fhq.insert(0, 1, x);   // 0 -> 1
fhq.insert(1, 5, y);   // 1 -> 5
fhq.erase(5, 5, x);    // 覆盖 version 5

int ans = fhq.kth(5, k);
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `init(versions)` | 初始化版本槽位 |
| `insert(from,to,x)` | 基于 `from` 插入，写入 `to` |
| `erase(from,to,x)` | 基于 `from` 删除一个 `x` |
| `rank_of(ver,x)` | 版本 `ver` 中 `x` 的排名 |
| `kth(ver,k)` | 版本 `ver` 中第 `k` 小 |
| `pre(ver,x)` | 严格前驱 |
| `nxt(ver,x)` | 严格后继 |
| `root[v]` | 第 `v` 个版本根 |

### 模板

```cpp
struct PersistentFHQSet {
    struct Node {
        int lc = 0, rc = 0;
        int val = 0, siz = 0;
        uint32_t pri = 0;

        // 【按题意修改】sum / mx / mn ...
    };

    vector<Node> tr{Node{}};
    vector<int> root{0};

    mt19937 rng{
        (uint32_t)chrono::steady_clock::now()
            .time_since_epoch().count()
    };

    void init(int versions = 0) {
        tr.assign(1, Node{});
        root.assign(versions + 1, 0);
    }

    int size(int p) const {
        return p ? tr[p].siz : 0;
    }

    void push_up(int p) {
        tr[p].siz = size(tr[p].lc) + size(tr[p].rc) + 1;

        // 【按题意修改】维护额外信息
    }

    int new_node(int val) {
        Node node;
        node.val = val;
        node.siz = 1;
        node.pri = rng();

        // 【按题意修改】初始化额外信息

        tr.push_back(node);
        return (int)tr.size() - 1;
    }

    int clone(int p) {
        tr.push_back(tr[p]);
        return (int)tr.size() - 1;
    }

    pair<int, int> split(int p, long long val) {
        if (!p)
            return {0, 0};

        int cur = clone(p);

        if (tr[cur].val <= val) {
            auto [x, y] = split(tr[cur].rc, val);
            tr[cur].rc = x;
            push_up(cur);
            return {cur, y};
        } else {
            auto [x, y] = split(tr[cur].lc, val);
            tr[cur].lc = y;
            push_up(cur);
            return {x, cur};
        }
    }

    int merge(int x, int y) {
        if (!x || !y)
            return x | y;

        if (tr[x].pri < tr[y].pri) {
            int cur = clone(x);
            tr[cur].rc = merge(tr[cur].rc, y);
            push_up(cur);
            return cur;
        } else {
            int cur = clone(y);
            tr[cur].lc = merge(x, tr[cur].lc);
            push_up(cur);
            return cur;
        }
    }

    void insert(int from, int to, int val) {
        int base = root[from];
        auto [x, y] = split(base, val);

        int new_root = merge(merge(x, new_node(val)), y);
        set_root(to, new_root);
    }

    void erase(int from, int to, int val) {
        int base = root[from];

        if (!contains_root(base, val)) {
            set_root(to, base);
            return;
        }

        auto [x, z] = split(base, val);
        auto [a, y] = split(x, (long long)val - 1);

        y = merge(tr[y].lc, tr[y].rc);

        int new_root = merge(merge(a, y), z);
        set_root(to, new_root);
    }

    bool contains(int ver, int val) const {
        return contains_root(root[ver], val);
    }

    int rank_of(int ver, int val) const {
        int p = root[ver];
        int res = 1;

        while (p) {
            if (tr[p].val >= val) {
                p = tr[p].lc;
            } else {
                res += size(tr[p].lc) + 1;
                p = tr[p].rc;
            }
        }

        return res;
    }

    int kth(int ver, int k) const {
        int p = root[ver];

        while (p) {
            int ls = size(tr[p].lc);

            if (k <= ls) {
                p = tr[p].lc;
            } else if (k == ls + 1) {
                return tr[p].val;
            } else {
                k -= ls + 1;
                p = tr[p].rc;
            }
        }

        return 0;
    }

    int pre(int ver, int val) const {
        int p = root[ver];
        int res = INT_MIN;  // 【按题意修改】

        while (p) {
            if (tr[p].val < val) {
                res = tr[p].val;
                p = tr[p].rc;
            } else {
                p = tr[p].lc;
            }
        }

        return res;
    }

    int nxt(int ver, int val) const {
        int p = root[ver];
        int res = INT_MAX;  // 【按题意修改】

        while (p) {
            if (tr[p].val > val) {
                res = tr[p].val;
                p = tr[p].lc;
            } else {
                p = tr[p].rc;
            }
        }

        return res;
    }

private:
    void set_root(int ver, int p) {
        if (ver >= (int)root.size())
            root.resize(ver + 1);

        root[ver] = p;
    }

    bool contains_root(int p, int val) const {
        while (p) {
            if (tr[p].val == val)
                return true;

            p = val < tr[p].val ? tr[p].lc : tr[p].rc;
        }

        return false;
    }
};
```

### 现场记忆

```text
可持久化 FHQ：
split / merge 只要准备修改节点，就先 clone。

insert(from,to,x)：
对 root[from] split，最后结果写 root[to]。

from == to 也安全：
先算完整新根，再覆盖版本槽位。

erase 一个重复值仍然是：
<val / ==val / >val，删除 ==val 树根一个。
```

## 4.28 可持久化 FHQ Treap：隐式序列

> **用途**：维护多个历史版本的动态序列，支持任意版本插入、删除区间、区间翻转和只读查询。
> **复杂度**：单次修改期望 `O(log n)`，新增期望 `O(log n)` 个节点；`kth` 查询期望 `O(log n)`
> **下标**：序列位置 `1-indexed`
> **版本**：`modify(from,to,...)` 风格，`from == to` 可覆盖当前版本槽位。
> **注意**：`push_down` 若要修改儿子，必须先复制儿子；纯查询不要 `push_down`。

### 调用

```cpp
PersistentFHQSeq fhq;

fhq.build(0, a);

fhq.insert(0, 1, 3, x);
fhq.reverse(1, 2, l, r);
fhq.erase(2, 2, l, r);

int x = fhq.kth(2, k);
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `build(ver,a)` | 在版本 `ver` 建初始序列 |
| `insert(from,to,pos,x)` | 插入后成为第 `pos` 个 |
| `erase(from,to,l,r)` | 删除 `[l,r]` |
| `reverse(from,to,l,r)` | 翻转 `[l,r]` |
| `kth(ver,k)` | 查询第 `k` 个元素 |
| `to_vector(ver)` | 导出指定版本 |
| `root[v]` | 第 `v` 个版本根 |

### 模板

```cpp
struct PersistentFHQSeq {
    struct Node {
        int lc = 0, rc = 0;
        int val = 0, siz = 0;
        uint32_t pri = 0;
        bool rev = false;

        // 【按题意修改】sum / mx / add / set ...
    };

    vector<Node> tr{Node{}};
    vector<int> root{0};

    mt19937 rng{
        (uint32_t)chrono::steady_clock::now()
            .time_since_epoch().count()
    };

    void init(int versions = 0) {
        tr.assign(1, Node{});
        root.assign(versions + 1, 0);
    }

    int size(int p) const {
        return p ? tr[p].siz : 0;
    }

    void push_up(int p) {
        tr[p].siz = size(tr[p].lc) + size(tr[p].rc) + 1;

        // 【按题意修改】维护额外信息
    }

    int new_node(int val) {
        Node node;
        node.val = val;
        node.siz = 1;
        node.pri = rng();

        // 【按题意修改】初始化额外信息

        tr.push_back(node);
        return (int)tr.size() - 1;
    }

    int clone(int p) {
        if (!p)
            return 0;

        tr.push_back(tr[p]);
        return (int)tr.size() - 1;
    }

    void apply_rev(int p) {
        if (!p)
            return;

        swap(tr[p].lc, tr[p].rc);
        tr[p].rev ^= 1;
    }

    // p 已经是当前版本的副本
    void push_down(int p) {
        if (!p || !tr[p].rev)
            return;

        tr[p].lc = clone(tr[p].lc);
        tr[p].rc = clone(tr[p].rc);

        apply_rev(tr[p].lc);
        apply_rev(tr[p].rc);

        tr[p].rev = false;

        // 【按题意修改】
        // 其他 lazy 同样：修改儿子前先 clone
    }

    pair<int, int> split(int p, int k) {
        if (!p)
            return {0, 0};

        int cur = clone(p);
        push_down(cur);

        int ls = size(tr[cur].lc);

        if (k <= ls) {
            auto [x, y] = split(tr[cur].lc, k);
            tr[cur].lc = y;
            push_up(cur);
            return {x, cur};
        } else {
            auto [x, y] = split(tr[cur].rc, k - ls - 1);
            tr[cur].rc = x;
            push_up(cur);
            return {cur, y};
        }
    }

    int merge(int x, int y) {
        if (!x || !y)
            return x | y;

        if (tr[x].pri < tr[y].pri) {
            int cur = clone(x);
            push_down(cur);

            tr[cur].rc = merge(tr[cur].rc, y);
            push_up(cur);
            return cur;
        } else {
            int cur = clone(y);
            push_down(cur);

            tr[cur].lc = merge(x, tr[cur].lc);
            push_up(cur);
            return cur;
        }
    }

    // 仅用于初始建树，新节点之间可直接修改
    int merge_build(int x, int y) {
        if (!x || !y)
            return x | y;

        if (tr[x].pri < tr[y].pri) {
            tr[x].rc = merge_build(tr[x].rc, y);
            push_up(x);
            return x;
        } else {
            tr[y].lc = merge_build(x, tr[y].lc);
            push_up(y);
            return y;
        }
    }

    void build(int ver, const vector<int> &a) {
        int p = 0;

        for (int x : a)
            p = merge_build(p, new_node(x));

        set_root(ver, p);
    }

    void insert(int from, int to, int pos, int val) {
        auto [x, y] = split(root[from], pos - 1);

        int new_root = merge(merge(x, new_node(val)), y);
        set_root(to, new_root);
    }

    void erase(int from, int to, int l, int r) {
        auto [x, yz] = split(root[from], l - 1);
        auto [y, z] = split(yz, r - l + 1);

        int new_root = merge(x, z);
        set_root(to, new_root);
    }

    void reverse(int from, int to, int l, int r) {
        auto [x, yz] = split(root[from], l - 1);
        auto [y, z] = split(yz, r - l + 1);

        apply_rev(y);

        int new_root = merge(x, merge(y, z));
        set_root(to, new_root);
    }

    int kth(int ver, int k) const {
        int p = root[ver];
        bool flip = false;

        while (p) {
            int left = flip ? tr[p].rc : tr[p].lc;
            int right = flip ? tr[p].lc : tr[p].rc;
            int ls = size(left);

            bool next_flip = flip ^ tr[p].rev;

            if (k <= ls) {
                p = left;
                flip = next_flip;
            } else if (k == ls + 1) {
                return tr[p].val;
            } else {
                k -= ls + 1;
                p = right;
                flip = next_flip;
            }
        }

        return 0;
    }

    vector<int> to_vector(int ver) const {
        vector<int> a;
        a.reserve(size(root[ver]));

        inorder(root[ver], false, a);
        return a;
    }

private:
    void set_root(int ver, int p) {
        if (ver >= (int)root.size())
            root.resize(ver + 1);

        root[ver] = p;
    }

    void inorder(int p, bool flip, vector<int> &a) const {
        if (!p)
            return;

        int left = flip ? tr[p].rc : tr[p].lc;
        int right = flip ? tr[p].lc : tr[p].rc;
        bool next_flip = flip ^ tr[p].rev;

        inorder(left, next_flip, a);
        a.push_back(tr[p].val);
        inorder(right, next_flip, a);
    }
};
```

### 现场记忆

```text
持久化隐式 FHQ 有两层要求：

split / merge：
当前要修改的节点先 clone。

push_down：
若 lazy 会修改儿子，
两个儿子也必须先 clone。

区间操作仍是两次 split。

纯查询不能 push_down；
用 flip 记录祖先尚未实际下传的 reverse。
```

## 4.29 可持久化隐式 FHQ：区间和扩展

> **用途**：在 `PersistentFHQSeq` 上增加区间和，并保持查询纯只读、不复制节点。
> **复杂度**：维护和不改变修改复杂度；区间和查询期望 `O(log n)`
> **限制**：下面的只读查询默认 lazy 只有 `rev`；若增加 `add / set`，部分覆盖查询也要考虑未下传标记。

### Node 增加

```cpp
long long val = 0;
long long sum = 0;
```

原来的：

```cpp
int val = 0;
```

删除。

### 增加辅助函数

```cpp
long long sum(int p) const {
    return p ? tr[p].sum : 0;
}
```

### 修改 `push_up`

```cpp
void push_up(int p) {
    tr[p].siz = size(tr[p].lc) + size(tr[p].rc) + 1;
    tr[p].sum = sum(tr[p].lc) + tr[p].val + sum(tr[p].rc);
}
```

### 修改 `new_node`

```cpp
int new_node(long long val) {
    Node node;
    node.val = node.sum = val;
    node.siz = 1;
    node.pri = rng();

    tr.push_back(node);
    return (int)tr.size() - 1;
}
```

对应的 `build / insert` 参数也将 `int val` 改为 `long long val`。

### 增加查询

```cpp
long long range_sum(int ver, int l, int r) const {
    return range_sum_impl(root[ver], l, r, false);
}
```

并在 `private` 中加入：

```cpp
long long range_sum_impl(int p, int ql, int qr,
                         bool flip) const {
    if (!p || ql > qr)
        return 0;

    if (ql == 1 && qr == size(p))
        return tr[p].sum;

    int left = flip ? tr[p].rc : tr[p].lc;
    int right = flip ? tr[p].lc : tr[p].rc;

    int ls = size(left);
    int mid = ls + 1;

    bool next_flip = flip ^ tr[p].rev;
    long long res = 0;

    if (ql <= ls) {
        res += range_sum_impl(
            left,
            ql,
            min(qr, ls),
            next_flip
        );
    }

    if (ql <= mid && mid <= qr)
        res += tr[p].val;

    if (qr > mid) {
        res += range_sum_impl(
            right,
            max(1, ql - mid),
            qr - mid,
            next_flip
        );
    }

    return res;
}
```

### 现场记忆

```text
sum 对 reverse 不敏感，所以 apply_rev 不用修改 sum。

纯查询不要：
split -> query -> merge
否则一次查询也会产生新节点。

只读查询遇到 rev：
不 push_down，
继续用 flip ^ tr[p].rev 解释儿子方向。

若再加 add / set lazy，这份只读查询要同步处理 lazy。
```

## 4.30 树状数组：动态第 k 小 / 前 k 小之和

> **用途**：值域可离散化时，动态维护多重集合，支持插入 / 删除、第 `k` 小、前 `k` 小之和，以及 `<=x` 的数量 / 总和。
> **复杂度**：预处理离散化 `O(V log V)`；修改 / 查询均 `O(log V)`；空间 `O(V)`
> **重复值**：支持。
> **限制**：所有可能修改的值必须提前加入离散化值域；删除时保证该值当前存在；总和保证不超过 `long long`。
> **优势**：`k` 不需要固定，每次查询可以不同。

### 调用

```cpp
vector<long long> values = a;   // 加入所有可能出现的值

FenwickKSum fw(values);

fw.insert(x);
fw.erase(x);

long long x = fw.kth(k);
long long ans = fw.sum_k(k);

long long cnt = fw.count_le(x);
long long sum = fw.sum_le(x);
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `init(values)` | 离散化所有可能出现的值并清空 |
| `insert(x)` | 插入一个 `x` |
| `erase(x)` | 删除一个 `x` |
| `modify(x,delta)` | `x` 的出现次数增加 `delta` |
| `size()` | 当前元素总数 |
| `sum()` | 当前所有元素之和 |
| `kth(k)` | 第 `k` 小，`1 <= k <= size()` |
| `sum_k(k)` | 前 `k` 小之和，`0 <= k <= size()` |
| `count_le(x)` | `<=x` 的元素个数 |
| `sum_le(x)` | `<=x` 的元素之和 |

### 模板

```cpp
struct FenwickKSum {
    using ll = long long;

    int n = 0;
    vector<ll> val;
    vector<ll> bit_cnt;
    vector<ll> bit_sum;

    FenwickKSum() = default;

    FenwickKSum(vector<ll> values) {
        init(std::move(values));
    }

    void init(vector<ll> values) {
        sort(values.begin(), values.end());
        values.erase(unique(values.begin(), values.end()),
                     values.end());

        val = std::move(values);
        n = (int)val.size();

        bit_cnt.assign(n + 1, 0);
        bit_sum.assign(n + 1, 0);
    }

    void modify(ll x, ll delta) {
        int p = get_id(x);

        for (int i = p; i <= n; i += i & -i) {
            bit_cnt[i] += delta;
            bit_sum[i] += delta * x;
        }
    }

    void insert(ll x) {
        modify(x, 1);
    }

    void erase(ll x) {
        modify(x, -1);
    }

    ll size() const {
        return prefix_cnt(n);
    }

    ll sum() const {
        return prefix_sum(n);
    }

    ll count_le(ll x) const {
        int p = upper_bound(val.begin(), val.end(), x)
              - val.begin();

        return prefix_cnt(p);
    }

    ll sum_le(ll x) const {
        int p = upper_bound(val.begin(), val.end(), x)
              - val.begin();

        return prefix_sum(p);
    }

    ll kth(ll k) const {
        int p = kth_id(k);
        return val[p - 1];
    }

    ll sum_k(ll k) const {
        if (k == 0)
            return 0;

        int p = 0;
        ll cnt = 0;
        ll res = 0;

        for (int d = highest_pow2(); d; d >>= 1) {
            int q = p + d;

            if (q <= n && cnt + bit_cnt[q] < k) {
                p = q;
                cnt += bit_cnt[q];
                res += bit_sum[q];
            }
        }

        // p 是满足 prefix_cnt(p) < k 的最大位置，
        // 第 k 小位于 p + 1。
        return res + (k - cnt) * val[p];
    }

private:
    int get_id(ll x) const {
        return (int)(lower_bound(val.begin(), val.end(), x)
                     - val.begin()) + 1;
    }

    ll prefix_cnt(int p) const {
        ll res = 0;

        for (; p; p -= p & -p)
            res += bit_cnt[p];

        return res;
    }

    ll prefix_sum(int p) const {
        ll res = 0;

        for (; p; p -= p & -p)
            res += bit_sum[p];

        return res;
    }

    int highest_pow2() const {
        int d = 1;

        while (d <= n / 2)
            d <<= 1;

        return d;
    }

    int kth_id(ll k) const {
        int p = 0;

        for (int d = highest_pow2(); d; d >>= 1) {
            int q = p + d;

            if (q <= n && bit_cnt[q] < k) {
                p = q;
                k -= bit_cnt[q];
            }
        }

        return p + 1;
    }
};
```

### 现场记忆

```text
离散化后开两棵 BIT：
cnt 维护出现次数，sum 维护实际值之和。

第 k 小：
在 cnt 上 Fenwick 倍增，O(log V)。

前 k 小和：
倍增时同时累加完整跳过部分的 cnt / sum，
最后补 (k-cnt) * 第 k 小值。

删除不能让某个值的出现次数变成负数。
```

## 4.31 双 multiset 维护前 k 小之和

> **用途**：动态维护多重集合中最小的 `k` 个元素及其和，同时可得到第 `k` 小；常用于滑动窗口。
> **复杂度**：固定 `k` 时插入 / 删除 `O(log n)`，查询和 `O(1)`
> **重复值**：支持。
> **注意**：元素不足 `k` 时，`sum()` 为当前所有元素之和，`full()` 为 `false`；`kth()` 仅在 `k>0 && full()` 时调用。
> **限制**：假设前 `k` 小元素之和可以放入 `long long`。

### 调用

```cpp
KSum s(k);

s.insert(x);
s.erase(x);

if (s.full()) {
    long long sum = s.sum();
    long long kth = s.kth();
}
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `init(k)` | 清空并重新设置 `k` |
| `insert(x)` | 插入一个 `x` |
| `erase(x)` | 删除一个 `x`，不存在则忽略 |
| `sum()` | 当前最小 `min(k,size)` 个元素之和 |
| `full()` | 当前元素数是否至少为 `k` |
| `kth()` | 第 `k` 小，仅在 `k>0 && full()` 时调用 |

### 模板

```cpp
struct KSum {
    using ll = long long;

    int k = 0;
    ll sum_small = 0;

    multiset<ll> small;
    multiset<ll> large;

    KSum() = default;

    KSum(int k) {
        init(k);
    }

    void init(int k_) {
        k = k_;
        sum_small = 0;
        small.clear();
        large.clear();
    }

    void insert(ll x) {
        if (!small.empty() && x <= *small.rbegin()) {
            small.insert(x);
            sum_small += x;
        } else {
            large.insert(x);
        }

        balance();
    }

    void erase(ll x) {
        auto it = small.find(x);

        if (it != small.end()) {
            sum_small -= x;
            small.erase(it);
        } else {
            it = large.find(x);

            if (it == large.end())
                return;

            large.erase(it);
        }

        balance();
    }

    ll sum() const {
        return sum_small;
    }

    bool full() const {
        return (int)small.size() == k;
    }

    ll kth() const {
        return *small.rbegin();
    }

private:
    void balance() {
        while ((int)small.size() > k) {
            auto it = prev(small.end());

            sum_small -= *it;
            large.insert(*it);
            small.erase(it);
        }

        while ((int)small.size() < k && !large.empty()) {
            auto it = large.begin();

            sum_small += *it;
            small.insert(*it);
            large.erase(it);
        }

        while (!small.empty() && !large.empty() &&
               *small.rbegin() > *large.begin()) {
            auto a = prev(small.end());
            auto b = large.begin();

            ll x = *a;
            ll y = *b;

            sum_small += y - x;

            small.erase(a);
            large.erase(b);

            small.insert(y);
            large.insert(x);
        }
    }
};
```

### 现场记忆

```text
small 保存最小的 min(k,n) 个数，
large 保存其余数。

始终维护：
small.size() <= k
max(small) <= min(large)

sum_small 只统计 small。

第 k 小 = max(small)，
调用前保证 k > 0 且 full()。
```

## 4.32 set 维护不相交区间集合

> **用途**：动态维护若干区间的并，插入新区间时自动合并所有重叠或相邻区间；也支持删除一段区间。
> **复杂度**：插入 / 删除 `O((k+1)\log s)`，`k` 为被合并或切割影响的区间数
> **区间**：闭区间 `[l,r]`
> **性质**：集合中任意两个区间始终不相交且不相邻。

### 调用

```cpp
IntSet st;

st.add(l, r);       // 加入 [l,r]，自动合并
st.erase(l, r);     // 从区间并中删除 [l,r]

bool ok = st.contains(x);
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `clear()` | 清空 |
| `add(l,r)` | 加入区间并自动合并重叠 / 相邻区间 |
| `erase(l,r)` | 删除区间 `[l,r]`，必要时切割 |
| `contains(x)` | 查询点 `x` 是否被覆盖 |
| `seg` | 当前所有极大不相交区间 |

### 模板

```cpp
struct IntSet {
    using ll = long long;
    using i128 = __int128_t;

    set<pair<ll, ll>> seg;

    void clear() {
        seg.clear();
    }

    void add(ll l, ll r) {
        if (l > r)
            swap(l, r);

        auto it = seg.lower_bound({l, numeric_limits<ll>::lowest()});

        if (it != seg.begin()) {
            auto pre = prev(it);

            if (touch(pre->second, l))
                it = pre;
        }

        while (it != seg.end() && touch(r, it->first)) {
            l = min(l, it->first);
            r = max(r, it->second);
            it = seg.erase(it);
        }

        seg.insert({l, r});
    }

    void erase(ll l, ll r) {
        if (l > r)
            swap(l, r);

        auto it = seg.lower_bound({l, numeric_limits<ll>::lowest()});

        if (it != seg.begin()) {
            auto pre = prev(it);

            if (pre->second >= l)
                it = pre;
        }

        vector<pair<ll, ll>> add_back;

        while (it != seg.end() && it->first <= r) {
            ll L = it->first;
            ll R = it->second;

            if (R < l) {
                ++it;
                continue;
            }

            it = seg.erase(it);

            if (L < l)
                add_back.push_back({L, l - 1});

            if (R > r)
                add_back.push_back({r + 1, R});
        }

        for (auto [L, R] : add_back)
            seg.insert({L, R});
    }

    bool contains(ll x) const {
        auto it = seg.upper_bound(
            {x, numeric_limits<ll>::max()}
        );

        if (it == seg.begin())
            return false;

        --it;
        return it->second >= x;
    }

private:
    static bool touch(ll r1, ll l2) {
        return (i128)r1 + 1 >= l2;
    }
};
```

### 现场记忆

```text
set 按左端点排序，始终保存极大区间。

add [l,r]：
lower_bound(l)，先检查前驱；
然后一路删除所有 first <= r+1 的区间并扩展 [l,r]。

erase [l,r]：
删除所有相交区间，
若原区间伸出删除范围，则把左右残段重新插回。

相邻区间也合并，所以判断是 r1 + 1 >= l2。
```

## 4.33 set 维护不相交区间集合

> **用途**：维护一组两两不相交的区间 ID，支持判断新区间是否冲突、插入 / 删除，以及找出或删除所有与 `[l,r]` 相交的区间。
> **复杂度**：插入 / 删除 `O(log s)`；找出 `k` 个相交区间 `O(log s+k)`；删除 `k` 个相交区间 `O(log s+k)`
> **区间**：闭区间 `[l,r]`；相邻区间如 `[1,2]`、`[3,4]` 不算相交。
> **限制**：`active` 中的区间必须始终两两不相交；区间加入 `active` 后，不要修改其左右端点。

### 调用

```cpp
vector<IntSet::Seg> seg(m + 1);

seg[id] = {l, r};

IntSet st(seg);

if (st.insert(id)) {
    // 插入成功
}

vector<int> ids = st.ids(l, r);
vector<int> removed = st.erase_cross(l, r);

st.erase(id);
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `init(seg)` | 绑定区间数组并清空集合 |
| `insert(id)` | 无相交区间时插入，成功返回 `true` |
| `erase(id)` | 删除指定 ID |
| `overlap(l,r)` | 是否存在与 `[l,r]` 相交的活动区间 |
| `ids(l,r)` | 返回所有与 `[l,r]` 相交的区间 ID |
| `erase_cross(l,r)` | 删除并返回所有相交区间 ID |

### 模板

```cpp
struct IntSet {
    struct Seg {
        int l = 0;
        int r = 0;
    };

    vector<Seg> seg;
    set<pair<int, int>> active;  // {l, id}

    IntSet() = default;

    IntSet(const vector<Seg> &seg) {
        init(seg);
    }

    void init(const vector<Seg> &seg_) {
        seg = seg_;
        active.clear();
    }

    bool overlap(int l, int r) {
        auto it = first_cross(l);

        return it != active.end() &&
               seg[it->second].l <= r;
    }

    bool insert(int id) {
        int l = seg[id].l;
        int r = seg[id].r;

        if (overlap(l, r))
            return false;

        active.insert({l, id});
        return true;
    }

    bool erase(int id) {
        auto it = active.find({seg[id].l, id});

        if (it == active.end())
            return false;

        active.erase(it);
        return true;
    }

    vector<int> ids(int l, int r) {
        vector<int> res;

        auto it = first_cross(l);

        while (it != active.end() &&
               seg[it->second].l <= r) {
            res.push_back(it->second);
            ++it;
        }

        return res;
    }

    vector<int> erase_cross(int l, int r) {
        vector<int> res;

        auto it = first_cross(l);

        while (it != active.end() &&
               seg[it->second].l <= r) {
            res.push_back(it->second);
            it = active.erase(it);
        }

        return res;
    }

private:
    set<pair<int, int>>::iterator first_cross(int l) {
        auto it = active.lower_bound({l, INT_MIN});

        if (it != active.begin()) {
            auto pre = prev(it);

            if (seg[pre->second].r >= l)
                it = pre;
        }

        return it;
    }
};
```

### 现场记忆

```text
active 按左端点排序，并保证区间两两不相交。

找与 [l,r] 相交的第一个区间：
lower_bound(l)，再检查前驱是否延伸到了 l。

找到起点后一直向右扫，
直到当前区间左端 > r。

不要直接强插重叠区间，否则 first_cross 的性质失效。
```

## 4.34 高精度整数 `cpp_int`

> **用途**：处理超出内置整数范围的整数加减乘除、取模和比较。
> **复杂度**：设整数长度为 `L` 个机器字块；加减 / 比较 `O(L)`，乘除复杂度依位数和 Boost 内部实现而定。
> **支持**：正数、负数、`0`，以及 `+ - * / %`、比较、位运算、输入输出。

```cpp
#include <boost/multiprecision/cpp_int.hpp>
using BigInt = boost::multiprecision::cpp_int;

BigInt a, b;
cin >> a >> b;

BigInt c = a + b;
BigInt d = a - b;
BigInt e = a * b;
BigInt f = a / b;
BigInt r = a % b;

cout << c << '\n';

string s = a.str();
long long x = a.convert_to<long long>(); // 确保 a 在 long long 范围内
```

### 现场记忆

```text
GNU++17/20 直接用 boost::multiprecision::cpp_int。

支持负数、除法、取模、比较和 cin / cout，
普通整数写法基本可以直接套用。

构造大位移要写：
BigInt(1) << k
不要先用 1LL << k 再赋值，否则可能已经溢出。
```
