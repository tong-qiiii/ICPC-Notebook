## 9.1 KMP / Prefix Function

> **用途**：计算前缀函数；在线性时间内查找模式串在文本中的所有出现位置。
> **复杂度**：前缀函数 `O(m)`；匹配 `O(n + m)`；空间 `O(m)`
> **下标**：`0-indexed`

```cpp
vector<int> prefix_function(const string &s) {
    int n = (int)s.size();
    vector<int> pi(n);

    for (int i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j])
            j = pi[j - 1];
        if (s[i] == s[j])
            ++j;
        pi[i] = j;
    }

    return pi;
}

vector<int> kmp_match(const string &text, const string &pat) {
    vector<int> pos;
    if (pat.empty())
        return pos;

    vector<int> pi = prefix_function(pat);
    int m = (int)pat.size();

    for (int i = 0, j = 0; i < (int)text.size(); ++i) {
        while (j > 0 && text[i] != pat[j])
            j = pi[j - 1];

        if (text[i] == pat[j])
            ++j;

        if (j == m) {
            pos.push_back(i - m + 1);
            j = pi[j - 1];
        }
    }

    return pos;
}
```

### 现场记忆

`pi[i]` = `s[0..i]` 的最长相等真前后缀长度。

失配：
`j = pi[j - 1]`

匹配完整模式串后仍要：
`j = pi[j - 1]`

这样才能继续寻找重叠匹配。

## 9.2 Z 函数

> **用途**：求每个后缀与整串前缀的 LCP；可用于字符串匹配、周期等问题。
> **复杂度**：`O(n)`，空间 `O(n)`
> **下标**：`0-indexed`
> **注意**：约定 `z[0] = n`。

```cpp
template <class Seq>
vector<int> z_function(const Seq &s) {
    int n = (int)s.size();
    vector<int> z(n);

    if (n == 0)
        return z;

    z[0] = n;

    for (int i = 1, l = 0, r = -1; i < n; ++i) {
        if (i <= r)
            z[i] = min(r - i + 1, z[i - l]);

        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            ++z[i];

        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }

    return z;
}

vector<int> z_match(const string &text, const string &pat) {
    vector<int> pos;
    if (pat.empty())
        return pos;

    int m = (int)pat.size();
    vector<int> s;
    s.reserve(m + 1 + text.size());

    for (unsigned char c : pat)
        s.push_back(c);

    s.push_back(256);

    for (unsigned char c : text)
        s.push_back(c);

    vector<int> z = z_function(s);

    for (int i = m + 1; i < (int)s.size(); ++i)
        if (z[i] >= m)
            pos.push_back(i - m - 1);

    return pos;
}
```

### 现场记忆

维护最右 Z-box `[l, r]`。

若 `i <= r`：
`z[i] = min(r - i + 1, z[i - l])`

然后继续暴力扩展。

新区间更靠右时更新 `[l, r]`。

## 9.3 Manacher：回文半径

> **用途**：线性求所有奇偶回文半径，可查询最长回文和指定区间是否回文。
> **复杂度**：预处理 `O(n)`；最长回文 `O(n)`；单次区间判断 `O(1)`
> **下标**：`0-indexed`，区间为闭区间 `[l, r]`

### 调用

```cpp
Manacher man(s);

cout << man.longest() << '\n';

if (man.is_pal(l, r)) {
    // s[l..r] is palindrome
}
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `build(s)` | 预处理回文半径 |
| `longest()` | 最长回文子串长度 |
| `is_pal(l,r)` | 判断闭区间 `[l,r]` 是否回文 |
| `d1[i]` | 以 `i` 为中心的奇回文半径 |
| `d2[i]` | 以 `i-1,i` 中间为中心的偶回文半径 |

### 模板

```cpp
struct Manacher {
    vector<int> d1, d2;

    Manacher() = default;

    Manacher(const string &s) {
        build(s);
    }

    void build(const string &s) {
        int n = (int)s.size();
        d1.assign(n, 0);
        d2.assign(n, 0);

        for (int i = 0, l = 0, r = -1; i < n; ++i) {
            int k = (i > r) ? 1 : min(d1[l + r - i], r - i + 1);

            while (i - k >= 0 && i + k < n &&
                   s[i - k] == s[i + k])
                ++k;

            d1[i] = k;

            if (i + k - 1 > r) {
                l = i - k + 1;
                r = i + k - 1;
            }
        }

        for (int i = 0, l = 0, r = -1; i < n; ++i) {
            int k = (i > r) ? 0
                            : min(d2[l + r - i + 1], r - i + 1);

            while (i - k - 1 >= 0 && i + k < n &&
                   s[i - k - 1] == s[i + k])
                ++k;

            d2[i] = k;

            if (i + k - 1 > r) {
                l = i - k;
                r = i + k - 1;
            }
        }
    }

    int longest() const {
        int ans = 0;

        for (int x : d1)
            ans = max(ans, 2 * x - 1);

        for (int x : d2)
            ans = max(ans, 2 * x);

        return ans;
    }

    bool is_pal(int l, int r) const {
        int len = r - l + 1;
        int mid = (l + r) >> 1;

        if (len & 1)
            return d1[mid] >= len / 2 + 1;

        return d2[mid + 1] >= len / 2;
    }
};
```

### 现场记忆

`d1[i]`：中心是 `i`，半径包含中心，长度 `2*d1[i]-1`。

`d2[i]`：中心在 `i-1` 和 `i` 之间，长度 `2*d2[i]`。

偶回文 `[l,r]` 的中心右侧位置：
`(l+r)/2 + 1`

Manacher 维护的 `[l,r]` 是当前最右回文闭区间。

## 9.4 字符串哈希：双模数

> **用途**：预处理字符串后，`O(1)` 获取子串哈希、比较两个子串或拼接哈希。
> **复杂度**：预处理 `O(n)`；`get / equal / merge` 均为 `O(1)`；空间 `O(n)`
> **下标**：`0-indexed`，`get(l,r)` 使用闭区间 `[l,r]`
> **注意**：哈希存在极低概率冲突；`merge()` 的 `right_len` 不能超过已预处理的幂次数。

### 调用

```cpp
Hash2 hs(s);

if (hs.equal(l1, r1, l2, r2)) {
    // s[l1..r1] == s[l2..r2]
}

auto h = hs.get(l, r);
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `build(s)` | 预处理字符串 |
| `get(l,r)` | 闭区间 `[l,r]` 的双哈希 |
| `equal(...)` | 比较两个子串 |
| `merge(a,b,len)` | 将哈希 `a` 与长度为 `len` 的 `b` 拼接 |

### 模板

```cpp
struct Hash2 {
    using Hash = pair<int, int>;

    static constexpr int MOD1 = 1000000007;
    static constexpr int MOD2 = 1000000009;
    static constexpr int BASE = 911382323;

    int n = 0;
    vector<int> p1, p2;
    vector<int> h1, h2;

    Hash2() = default;

    Hash2(const string &s) {
        build(s);
    }

    void build(const string &s) {
        n = (int)s.size();

        p1.assign(n + 1, 1);
        p2.assign(n + 1, 1);
        h1.assign(n + 1, 0);
        h2.assign(n + 1, 0);

        for (int i = 1; i <= n; ++i) {
            p1[i] = 1LL * p1[i - 1] * BASE % MOD1;
            p2[i] = 1LL * p2[i - 1] * BASE % MOD2;

            int c = (unsigned char)s[i - 1] + 1;

            h1[i] = (1LL * h1[i - 1] * BASE + c) % MOD1;
            h2[i] = (1LL * h2[i - 1] * BASE + c) % MOD2;
        }
    }

    Hash get(int l, int r) const {
        int len = r - l + 1;

        int x1 = (
            h1[r + 1] - 1LL * h1[l] * p1[len] % MOD1 + MOD1
        ) % MOD1;

        int x2 = (
            h2[r + 1] - 1LL * h2[l] * p2[len] % MOD2 + MOD2
        ) % MOD2;

        return {x1, x2};
    }

    bool equal(int l1, int r1, int l2, int r2) const {
        if (r1 - l1 != r2 - l2)
            return false;

        return get(l1, r1) == get(l2, r2);
    }

    Hash merge(Hash left, Hash right, int right_len) const {
        int x1 = (
            1LL * left.first * p1[right_len] + right.first
        ) % MOD1;

        int x2 = (
            1LL * left.second * p2[right_len] + right.second
        ) % MOD2;

        return {x1, x2};
    }
};
```

### 现场记忆

前缀哈希：
`h[i+1] = h[i] * BASE + s[i]`

子串 `[l,r]`：
`h[r+1] - h[l] * BASE^(r-l+1)`

拼接 `A + B`：
`hash(A) * BASE^|B| + hash(B)`

比较子串时先保证长度相同。

## 9.5 Trie：字符串插入 / 前缀计数

> **用途**：维护小写字符串集合，支持插入、统计完整字符串出现次数和指定前缀数量。
> **复杂度**：单次插入 / 查询 `O(|s|)`，空间 `O(字符总数 × 字符集)` 的节点存储。
> **字符集**：`a ~ z`

### 调用

```cpp
Trie tr;

tr.insert("apple");
tr.insert("app");

tr.count_word("app");      // 1
tr.count_prefix("app");    // 2
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `init()` | 清空 Trie |
| `insert(s)` | 插入字符串 |
| `count_word(s)` | `s` 完整出现次数 |
| `count_prefix(s)` | 以 `s` 为前缀的字符串数量 |

### 模板

```cpp
struct Trie {
    struct Node {
        array<int, 26> nxt{};
        int pass = 0;
        int end = 0;
    };

    vector<Node> tr;

    Trie() { init(); }

    void init() {
        tr.assign(1, Node());
    }

    int new_node() {
        tr.push_back(Node());
        return (int)tr.size() - 1;
    }

    void insert(const string &s) {
        int u = 0;
        ++tr[u].pass;

        for (char ch : s) {
            int c = ch - 'a';
            if (!tr[u].nxt[c])
                tr[u].nxt[c] = new_node();

            u = tr[u].nxt[c];
            ++tr[u].pass;
        }

        ++tr[u].end;
    }

    int count_word(const string &s) const {
        int u = 0;

        for (char ch : s) {
            int c = ch - 'a';
            if (!tr[u].nxt[c]) return 0;
            u = tr[u].nxt[c];
        }

        return tr[u].end;
    }

    int count_prefix(const string &s) const {
        int u = 0;

        for (char ch : s) {
            int c = ch - 'a';
            if (!tr[u].nxt[c]) return 0;
            u = tr[u].nxt[c];
        }

        return tr[u].pass;
    }
};
```

### 现场记忆

- `pass[u]`：经过节点 `u` 的字符串数量。
- `end[u]`：恰好在节点 `u` 结束的字符串数量。
- 本模板默认字符均为 `a ~ z`。

## 9.6 01 Trie：最大异或 / 最小异或

> **用途**：维护非负整数集合，支持插入、删除，以及查询与 `x` 的最大 / 最小异或值。
> **复杂度**：单次插入 / 删除 / 查询 `O(LOG)`，默认 `LOG = 30`。
> **注意**：查询时集合必须非空；`erase(x)` 前必须保证 `x` 已存在。

### 调用

```cpp
Trie01 tr;

tr.insert(5);
tr.insert(10);

int mx = tr.max_xor(7);
int mn = tr.min_xor(7);

tr.erase(5);
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `init(log)` | 清空并设置最高二进制位 |
| `insert(x)` | 插入 `x` |
| `erase(x)` | 删除一个 `x` |
| `max_xor(x)` | 返回集合中 `y ^ x` 的最大值 |
| `min_xor(x)` | 返回集合中 `y ^ x` 的最小值 |
| `size()` | 当前元素数量 |

### 模板

```cpp
struct Trie01 {
    struct Node {
        int son[2] = {};
        int cnt = 0;
    };

    int LOG;
    vector<Node> tr;

    Trie01(int log = 30) { init(log); }

    void init(int log = 30) {
        LOG = log;
        tr.assign(1, Node());
    }

    int new_node() {
        tr.push_back(Node());
        return (int)tr.size() - 1;
    }

    int size() const {
        return tr[0].cnt;
    }

    void insert(int x) {
        int u = 0;
        ++tr[u].cnt;

        for (int i = LOG; i >= 0; --i) {
            int b = (x >> i) & 1;

            if (!tr[u].son[b])
                tr[u].son[b] = new_node();

            u = tr[u].son[b];
            ++tr[u].cnt;
        }
    }

    void erase(int x) {
        assert(size() > 0);

        int u = 0;
        --tr[u].cnt;

        for (int i = LOG; i >= 0; --i) {
            int b = (x >> i) & 1;
            assert(tr[u].son[b] && tr[tr[u].son[b]].cnt > 0);

            u = tr[u].son[b];
            --tr[u].cnt;
        }
    }

    int max_xor(int x) const {
        assert(size() > 0);

        int u = 0, ans = 0;

        for (int i = LOG; i >= 0; --i) {
            int b = (x >> i) & 1;
            int v = b ^ 1;

            if (tr[u].son[v] && tr[tr[u].son[v]].cnt > 0) {
                ans |= 1 << i;
                u = tr[u].son[v];
            } else {
                u = tr[u].son[b];
            }
        }

        return ans;
    }

    int min_xor(int x) const {
        assert(size() > 0);

        int u = 0, ans = 0;

        for (int i = LOG; i >= 0; --i) {
            int b = (x >> i) & 1;

            if (tr[u].son[b] && tr[tr[u].son[b]].cnt > 0) {
                u = tr[u].son[b];
            } else {
                ans |= 1 << i;
                u = tr[u].son[b ^ 1];
            }
        }

        return ans;
    }
};
```

### 现场记忆

- 求最大异或：每一位优先走 `b ^ 1`。
- 求最小异或：每一位优先走 `b`。
- `cnt > 0` 才表示该分支当前仍有元素。
- 默认 `LOG = 30`，适用于非负 `int`。

## 9.7 AC 自动机：多模式匹配

> **用途**：同时匹配多个小写模式串，并统计每个模式串在文本中的出现次数。
> **复杂度**：插入 `O(模式串总长)`，建机 `O(节点数 × 26)`，匹配 `O(|text| + 节点数 + 模式数)`。
> **字符集**：`a ~ z`
> **注意**：所有模式串必须在 `build()` 前插入；允许重复模式串和重叠匹配。

### 调用

```cpp
AC ac;

int id1 = ac.insert("he");
int id2 = ac.insert("she");

ac.build();

auto cnt = ac.count("sheshe");

cout << cnt[id1] << '\n';
cout << cnt[id2] << '\n';
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `init()` | 清空自动机 |
| `insert(s)` | 插入模式串，返回模式编号 |
| `build()` | 建立 fail 指针并补全转移 |
| `count(text)` | 返回每个模式串的出现次数 |

### 模板

```cpp
struct AC {
    struct Node {
        array<int, 26> nxt{};
        int fail = 0;
    };

    vector<Node> tr;
    vector<int> pos, order;
    bool built = false;

    AC() { init(); }

    void init() {
        tr.assign(1, Node());
        pos.clear();
        order.clear();
        built = false;
    }

    int new_node() {
        tr.push_back(Node());
        return (int)tr.size() - 1;
    }

    int insert(const string &s) {
        assert(!built);

        int u = 0;
        for (char ch : s) {
            int c = ch - 'a';

            if (!tr[u].nxt[c])
                tr[u].nxt[c] = new_node();

            u = tr[u].nxt[c];
        }

        pos.push_back(u);
        return (int)pos.size() - 1;
    }

    void build() {
        if (built) return;
        built = true;

        queue<int> q;
        order.clear();

        for (int c = 0; c < 26; ++c) {
            int v = tr[0].nxt[c];
            if (v) q.push(v);
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            order.push_back(u);

            for (int c = 0; c < 26; ++c) {
                int v = tr[u].nxt[c];

                if (v) {
                    tr[v].fail = tr[tr[u].fail].nxt[c];
                    q.push(v);
                } else {
                    tr[u].nxt[c] = tr[tr[u].fail].nxt[c];
                }
            }
        }
    }

    vector<long long> count(const string &text) const {
        assert(built);

        vector<long long> cnt(tr.size());
        int u = 0;

        for (char ch : text) {
            if (ch < 'a' || ch > 'z') {
                u = 0;
                continue;
            }

            u = tr[u].nxt[ch - 'a'];
            ++cnt[u];
        }

        for (int i = (int)order.size() - 1; i >= 0; --i) {
            int v = order[i];
            cnt[tr[v].fail] += cnt[v];
        }

        vector<long long> ans(pos.size());
        for (int i = 0; i < (int)pos.size(); ++i)
            ans[i] = cnt[pos[i]];

        return ans;
    }
};
```

### 现场记忆

- `fail[u]` 指向 `u` 的最长真后缀状态。
- 建机时不存在的转移直接继承 `fail` 的转移。
- 扫文本只统计“到达每个状态多少次”。
- 最后按 BFS 逆序：`cnt[fail[u]] += cnt[u]`。
- 模式串答案就是其终止节点的 `cnt`。

## 9.8 回文自动机 PAM

> **用途**：维护所有本质不同回文子串；可查询当前最长回文后缀、回文后缀数量及每个回文串出现次数。
> **复杂度**：建树 `O(n)`；`calc_occ()` `O(n)`；空间 `O(n)`
> **下标**：节点编号 `0-indexed`，`0` 为偶根，`1` 为奇根
> **限制**：字符集为 `'a' ~ 'z'`，节点数最多 `n + 2`
> **注意**：`tr[u].occ` 需在 `calc_occ()` 后读取；继续插入字符后应重新调用。

### 调用

```cpp
PAM pam;
pam.build(s);
pam.calc_occ();

cout << pam.distinct() << '\n';
cout << pam.longest_suffix() << '\n';

for (int u = 2; u < (int)pam.tr.size(); ++u)
    cout << pam.tr[u].len << ' ' << pam.tr[u].occ << '\n';
```

逐字符维护：

```cpp
PAM pam(n);

for (char c : s) {
    int u = pam.insert(c);

    cout << pam.tr[u].len << '\n';
    cout << pam.suffix_count() << '\n';
}
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `init()` | 清空并初始化 |
| `insert(c)` | 插入字符，返回当前最长回文后缀节点 |
| `build(s)` | 清空后插入整个字符串 |
| `calc_occ()` | 汇总所有回文串出现次数 |
| `distinct()` | 本质不同回文串数量 |
| `longest_suffix()` | 当前前缀最长回文后缀长度 |
| `suffix_count()` | 当前前缀的回文后缀数量 |
| `last` | 当前最长回文后缀节点 |
| `tr[u].len` | 节点 `u` 对应回文串长度 |
| `tr[u].fail` | 最长真回文后缀 |
| `tr[u].occ` | 回文串总出现次数 |

### 模板

```cpp
struct PAM {
    static constexpr int A = 26;

    struct Node {
        array<int, A> nxt{};
        int fail = 0;
        int len = 0;
        int suf_cnt = 0;

        // cnt: 作为最长回文后缀的次数；occ: 总出现次数
        long long cnt = 0, occ = 0;

        Node(int len = 0) : len(len) {}
    };

    vector<Node> tr;
    vector<int> s;
    int last = 0;

    PAM(int n = 0) {
        tr.reserve(n + 2);
        s.reserve(n + 1);
        init();
    }

    void init() {
        tr.clear();
        tr.emplace_back(0);
        tr.emplace_back(-1);

        tr[0].fail = 1;
        tr[1].fail = 1;

        s.clear();
        s.push_back(-1);

        last = 0;
    }

private:
    int get_fail(int u) {
        int n = (int)s.size() - 1;

        while (s[n - tr[u].len - 1] != s[n])
            u = tr[u].fail;

        return u;
    }

public:
    int insert(char ch) {
        int c = ch - 'a';
        s.push_back(c);

        int cur = get_fail(last);

        if (!tr[cur].nxt[c]) {
            int now = (int)tr.size();
            tr.emplace_back(tr[cur].len + 2);

            if (tr[now].len == 1) {
                tr[now].fail = 0;
            } else {
                int f = get_fail(tr[cur].fail);
                tr[now].fail = tr[f].nxt[c];
            }

            tr[now].suf_cnt = tr[tr[now].fail].suf_cnt + 1;
            tr[cur].nxt[c] = now;
        }

        last = tr[cur].nxt[c];
        ++tr[last].cnt;

        return last;
    }

    void build(const string &str) {
        init();
        tr.reserve(str.size() + 2);
        s.reserve(str.size() + 1);

        for (char c : str)
            insert(c);
    }

    void calc_occ() {
        for (auto &x : tr)
            x.occ = x.cnt;

        // fail[u] 一定指向更早创建的节点
        for (int u = (int)tr.size() - 1; u >= 2; --u)
            tr[tr[u].fail].occ += tr[u].occ;
    }

    int distinct() const {
        return (int)tr.size() - 2;
    }

    int longest_suffix() const {
        return tr[last].len;
    }

    int suffix_count() const {
        return tr[last].suf_cnt;
    }
};
```

### 现场记忆

两个根：

`0: len = 0`，偶根  
`1: len = -1`，奇根

插入字符 `c`：
沿 `fail` 找满足两端能加 `c` 的最长回文。

新节点：
`len = tr[cur].len + 2`

`fail` = 新回文的最长真回文后缀。

`tr[u].suf_cnt = tr[fail[u]].suf_cnt + 1`

出现次数最后沿 `fail` 从长回文向短回文累加。

## 9.9 后缀自动机 SAM

> **用途**：维护一个字符串的所有子串；可查询不同子串数量、模式串出现次数、是否为子串及与另一串的最长公共子串。
> **复杂度**：建树 `O(n)`；`calc_occ()` `O(n)`；`contains / count / lcs_substr` 均为 `O(|t|)`
> **空间**：`O(n)`，状态数最多 `2n - 1`
> **限制**：字符集为 `'a' ~ 'z'`
> **注意**：`count(t)` 前需调用 `calc_occ()`；之后若继续 `extend()`，需重新调用。

### 调用

```cpp
SAM sam;
sam.build(s);

cout << sam.distinct() << '\n';
cout << sam.contains(t) << '\n';
cout << sam.lcs_substr(t) << '\n';

sam.calc_occ();
cout << sam.count(t) << '\n';
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `init()` | 清空并初始化 |
| `extend(c)` | 在末尾加入字符 |
| `build(s)` | 清空后建立 SAM |
| `calc_occ()` | 汇总每个状态的出现次数 |
| `distinct()` | 本质不同子串数量 |
| `contains(t)` | `t` 是否为原串子串 |
| `count(t)` | `t` 的出现次数 |
| `lcs_substr(t)` | 与 `t` 的最长公共子串长度 |
| `st[u].len` | 状态最大长度 |
| `st[u].link` | suffix link |
| `st[u].occ` | 状态对应子串出现次数 |

### 模板

```cpp
struct SAM {
    static constexpr int A = 26;

    struct Node {
        array<int, A> nxt;
        int len = 0;
        int link = -1;

        // cnt: endpos 初始贡献；occ: calc_occ() 后的出现次数
        long long cnt = 0, occ = 0;

        Node() {
            nxt.fill(-1);
        }
    };

    vector<Node> st;
    int last = 0;

    SAM(int n = 0) {
        st.reserve(2 * n + 5);
        init();
    }

    void init() {
        st.clear();
        st.emplace_back();
        last = 0;
    }

    void extend(char ch) {
        int c = ch - 'a';

        int cur = (int)st.size();
        st.emplace_back();

        st[cur].len = st[last].len + 1;
        st[cur].cnt = 1;

        int p = last;

        while (p != -1 && st[p].nxt[c] == -1) {
            st[p].nxt[c] = cur;
            p = st[p].link;
        }

        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].nxt[c];

            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = (int)st.size();
                st.push_back(st[q]);

                st[clone].len = st[p].len + 1;
                st[clone].cnt = 0;
                st[clone].occ = 0;

                while (p != -1 && st[p].nxt[c] == q) {
                    st[p].nxt[c] = clone;
                    p = st[p].link;
                }

                st[q].link = st[cur].link = clone;
            }
        }

        last = cur;
    }

    void build(const string &s) {
        init();
        st.reserve(2 * s.size() + 5);

        for (char c : s)
            extend(c);
    }

    void calc_occ() {
        int mx = 0;

        for (auto &x : st) {
            x.occ = x.cnt;
            mx = max(mx, x.len);
        }

        vector<int> cnt(mx + 1);
        vector<int> ord(st.size());

        for (auto &x : st)
            ++cnt[x.len];

        for (int i = 1; i <= mx; ++i)
            cnt[i] += cnt[i - 1];

        for (int u = (int)st.size() - 1; u >= 0; --u)
            ord[--cnt[st[u].len]] = u;

        for (int i = (int)ord.size() - 1; i > 0; --i) {
            int u = ord[i];
            st[st[u].link].occ += st[u].occ;
        }
    }

    long long distinct() const {
        long long ans = 0;

        for (int u = 1; u < (int)st.size(); ++u)
            ans += st[u].len - st[st[u].link].len;

        return ans;
    }

    bool contains(const string &t) const {
        int u = 0;

        for (char ch : t) {
            int c = ch - 'a';

            if (st[u].nxt[c] == -1)
                return false;

            u = st[u].nxt[c];
        }

        return true;
    }

    long long count(const string &t) const {
        int u = 0;

        for (char ch : t) {
            int c = ch - 'a';

            if (st[u].nxt[c] == -1)
                return 0;

            u = st[u].nxt[c];
        }

        return st[u].occ;
    }

    int lcs_substr(const string &t) const {
        int u = 0;
        int len = 0;
        int ans = 0;

        for (char ch : t) {
            int c = ch - 'a';

            while (u && st[u].nxt[c] == -1) {
                u = st[u].link;
                len = st[u].len;
            }

            if (st[u].nxt[c] != -1) {
                u = st[u].nxt[c];
                ++len;
            } else {
                u = 0;
                len = 0;
            }

            ans = max(ans, len);
        }

        return ans;
    }
};
```

### 现场记忆

普通扩展：
一路补 `nxt[c]`，直到遇到已有转移。

若 `len[p] + 1 == len[q]`：
直接 `link[cur] = q`。

否则 clone：
复制 `q`，令 `len[clone] = len[p] + 1`，
再把指向 `q` 的对应转移改到 clone。

不同子串数：
`Σ(len[u] - len[link[u]])`

出现次数按 `len` 从大到小沿 `link` 累加；clone 初始贡献为 `0`。

## 9.10 后缀数组 SA：O(n log n)

> **用途**：求字符串所有后缀的字典序排名。
> **复杂度**：时间 `O(n log n)`，空间 `O(n)`
> **下标**：字符串、`sa` 均 `0-indexed`

### 调用

```cpp
auto sa = buildSA(s);

for (int p : sa) cout << p << ' ';
```

### 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<int> buildSA(const string& s) {
    int n = s.size();
    if (!n) return {};

    int m = 256;

    vector<int> sa(n), x(n), y(n);
    vector<int> cnt(max(n, 256) + 1);

    for (int i = 0; i < n; ++i) ++cnt[x[i] = (unsigned char)s[i]];

    for (int i = 1; i < m; ++i) cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; --i) sa[--cnt[x[i]]] = i;

    for (int k = 1; k < n; k <<= 1) {
        int p = 0;

        for (int i = n - k; i < n; ++i) y[p++] = i;
        for (int i = 0; i < n; ++i)
            if (sa[i] >= k)
                y[p++] = sa[i] - k;

        fill(cnt.begin(), cnt.begin() + m, 0);

        for (int i = 0; i < n; ++i) ++cnt[x[y[i]]];
        for (int i = 1; i < m; ++i) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; --i) sa[--cnt[x[y[i]]]] = y[i];

        swap(x, y);

        x[sa[0]] = 0;
        p = 1;

        for (int i = 1; i < n; ++i) {
            int a = sa[i - 1], b = sa[i];

            bool same = y[a] == y[b];
            same &= (a + k < n ? y[a + k] : -1) == (b + k < n ? y[b + k] : -1);

            x[b] = same ? p - 1 : p++;
        }

        if (p == n) break;
        m = p;
    }

    return sa;
}
```

### 现场记忆

```text
倍增：
比较后缀前 2k 个字符
= 两个长度 k 的 rank。

先按第二关键字排，再按第一关键字计数排序。

sa[i]：排名 i 的后缀起点。
rank[sa[i]] = i。
```

## 9.11 LCP / Height + RMQ

> **用途**：预处理任意两个后缀的最长公共前缀 `LCP(i,j)`。
> **复杂度**：构造 SA `O(n log n)`，Height `O(n)`，RMQ `O(n log n)`，单次查询 `O(1)`
> **下标**：字符串位置 `0 ~ n-1`

### 调用

```cpp
LCPRMQ lcp(s);

cout << lcp.query(i, j) << '\n';
```

### 接口速查

| 变量 / 接口 | 含义 |
|---|---|
| `sa[k]` | 第 `k` 小后缀起点 |
| `rk[i]` | 后缀 `i` 的排名 |
| `height[k]` | `LCP(sa[k-1], sa[k])` |
| `query(i,j)` | 后缀 `i,j` 的 LCP |

### 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<int> buildSA(const string& s) {
    int n = s.size();
    if (!n) return {};

    int m = 256;

    vector<int> sa(n), x(n), y(n);
    vector<int> cnt(max(n, 256) + 1);

    for (int i = 0; i < n; ++i) ++cnt[x[i] = (unsigned char)s[i]];

    for (int i = 1; i < m; ++i) cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; --i) sa[--cnt[x[i]]] = i;

    for (int k = 1; k < n; k <<= 1) {
        int p = 0;

        for (int i = n - k; i < n; ++i) y[p++] = i;
        for (int i = 0; i < n; ++i)
            if (sa[i] >= k)
                y[p++] = sa[i] - k;

        fill(cnt.begin(), cnt.begin() + m, 0);

        for (int i = 0; i < n; ++i) ++cnt[x[y[i]]];
        for (int i = 1; i < m; ++i) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; --i) sa[--cnt[x[y[i]]]] = y[i];

        swap(x, y);

        x[sa[0]] = 0;
        p = 1;

        for (int i = 1; i < n; ++i) {
            int a = sa[i - 1], b = sa[i];

            bool same = y[a] == y[b];
            same &= (a + k < n ? y[a + k] : -1) == (b + k < n ? y[b + k] : -1);

            x[b] = same ? p - 1 : p++;
        }

        if (p == n) break;
        m = p;
    }

    return sa;
}

struct LCPRMQ {
    int n;
    string s;
    vector<int> sa, rk, height, lg;
    vector<vector<int>> st;

    LCPRMQ(const string& s_) : n(s_.size()), s(s_) {
        sa = buildSA(s);

        if (!n) return;

        rk.resize(n);
        height.resize(n);

        for (int i = 0; i < n; ++i) rk[sa[i]] = i;

        for (int i = 0, h = 0; i < n; ++i) {
            int r = rk[i];

            if (!r) continue;

            int j = sa[r - 1];

            while (i + h < n && j + h < n && s[i + h] == s[j + h]) ++h;

            height[r] = h;

            if (h) --h;
        }

        lg.resize(n + 1);

        for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;

        int K = lg[n] + 1;

        st.assign(K, vector<int>(n));
        st[0] = height;

        for (int k = 1; k < K; ++k)
            for (int i = 0; i + (1 << k) <= n; ++i)
                st[k][i] = min(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
    }

    int query(int i, int j) const {
        if (i == j) return n - i;

        int l = rk[i], r = rk[j];

        if (l > r) swap(l, r);

        ++l;

        int len = r - l + 1;
        int k = lg[len];

        return min(st[k][l], st[k][r - (1 << k) + 1]);
    }
};
```

### 现场记忆

```text
height[i] = LCP(sa[i-1], sa[i])

任意两个后缀排名 l < r：
LCP = min(height[l+1 ... r])

Kasai：
当前后缀 LCP 为 h，
下一个后缀至少从 h-1 开始匹配。
```

## 9.12 Duval：Lyndon 分解

> **用途**：将字符串唯一分解为字典序非增的 Lyndon 串序列。
> **复杂度**：时间 `O(n)`，空间 `O(n)`
> **下标**：返回区间 `[l,r)`，`0-indexed`

### 调用

```cpp
auto seg = duval(s);

for (auto [l, r] : seg)
    cout << s.substr(l, r - l) << '\n';
```

### 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

vector<pair<int, int>> duval(const string& s) {
    int n = s.size();
    vector<pair<int, int>> res;

    int i = 0;

    while (i < n) {
        int j = i + 1, k = i;

        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j]) k = i;
            else ++k;

            ++j;
        }

        int len = j - k;

        while (i <= k) {
            res.push_back({i, i + len});
            i += len;
        }
    }

    return res;
}
```

### 现场记忆

```text
维护 i, j, k。

s[k] < s[j] -> k = i
s[k] == s[j] -> ++k
s[k] > s[j] -> 当前 Lyndon 块确定

块长 = j-k
不断从 i 开始切长度 j-k。
```

## 9.13 字符串最小表示法

> **用途**：求循环字符串所有旋转中字典序最小的旋转起点。
> **复杂度**：时间 `O(n)`，空间 `O(1)`
> **下标**：返回起点 `0-indexed`

### 调用

```cpp
int p = minRepresentation(s);
string t = s.substr(p) + s.substr(0, p);
```

### 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

int minRepresentation(const string& s) {
    int n = s.size();
    if (!n) return 0;

    int i = 0, j = 1, k = 0;

    while (i < n && j < n && k < n) {
        char a = s[(i + k) % n];
        char b = s[(j + k) % n];

        if (a == b) {
            ++k;
            continue;
        }

        if (a > b) {
            i += k + 1;
            if (i == j) ++i;
        } else {
            j += k + 1;
            if (i == j) ++j;
        }

        k = 0;
    }

    return min(i, j);
}
```

### 现场记忆

```text
比较旋转 i 和 j。

前 k 个字符相同，第 k 个首次不同：

i 更大 -> i 到 i+k 全不可能是答案
j 更大 -> j 到 j+k 全不可能是答案

直接跳 k+1。
```
