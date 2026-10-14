## 1.1 GCD

> **用途**：求两个整数的最大公约数，用于约分、互质判断等。
> **复杂度**：时间 `O(log min(|a|, |b|))`，额外空间 `O(1)`。

```cpp
long long g = std::gcd(a, b);
```

### 现场记忆

`std::gcd` 在 `<numeric>`，`bits/stdc++.h` 已包含。
支持负数，结果非负；`gcd(0, 0) = 0`。


## 1.2 位运算枚举

> **用途**：枚举状态、置位位置、子集以及所有 `(mask, sub)`。
> **复杂度**：所有状态 `O(2^n)`；单集合子集 `O(2^{popcount(m)})`；所有集合子集对 `O(3^n)`。
> **下标**：二进制位 `0-indexed`
> **注意**：`1 << n` 使用 `int`，需要更高位时改成 `1LL << n`。

```cpp
// 所有状态
for (int mask = 0; mask < (1 << n); ++mask) {
}

// mask 中所有置位位置
for (int s = mask; s; s &= s - 1) {
    int i = __builtin_ctz(s);
}

// m 的所有非空子集，降序
for (int s = m; s; s = (s - 1) & m) {
}

// m 的所有子集，包括空集
for (int s = m;; s = (s - 1) & m) {
    // use s
    if (s == 0) break;
}

// 所有 mask 及其非空子集
for (int m = 0; m < (1 << n); ++m) {
    for (int s = m; s; s = (s - 1) & m) {
        // use (m, s)
    }
}
```

### 现场记忆

删最低位 `1`：`s &= s - 1`。

子集下一状态：
`(s - 1) & m`

包含空集时不能直接写 `for (...; s; ...)`，
处理完 `s == 0` 后手动 `break`。


## 1.3 快读

> **用途**：使用 `fread` 快速读取整数和无空白字符串。
> **复杂度**：总时间 `O(输入长度)`，缓冲区空间 `O(2^20)`。

### 调用

```cpp
int n;
long long x;
string s;

io.read(n);
io.read(x);
io.read(s);
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `read(x)` | 读取整数，支持负数 |
| `read(s)` | 读取一个无空白字符串 |
| 返回值 | 成功为 `true`，EOF 为 `false` |

### 模板

```cpp
struct FastIO {
    static constexpr int S = 1 << 20;

    unsigned char buf[S];
    int p = 0, len = 0;

    int gc() {
        if (p == len) {
            len = fread(buf, 1, S, stdin);
            p = 0;
            if (!len) return EOF;
        }
        return buf[p++];
    }

    template <class T>
    bool read(T& x) {
        int c = gc();

        while (c != EOF && c != '-' && (c < '0' || c > '9'))
            c = gc();

        if (c == EOF) return false;

        bool neg = false;
        if (c == '-') {
            neg = true;
            c = gc();
        }

        x = 0;
        while (c >= '0' && c <= '9') {
            x = x * 10 + (c - '0');
            c = gc();
        }

        if (neg) x = -x;
        return true;
    }

    bool read(string& s) {
        int c = gc();

        while (c != EOF && c <= ' ')
            c = gc();

        if (c == EOF) return false;

        s.clear();
        while (c > ' ') {
            s += char(c);
            c = gc();
        }
        return true;
    }
} io;
```

### 现场记忆

`gc()` 必须返回 `int`，否则 `EOF` 处理容易出问题。

整数读取流程：
跳无关字符 → 判负号 → 连续读数字。

需要读整行时不要直接套 `read(string)`，它只读一个 token。

## 1.4 取模类 Mint

> **用途**：封装模意义下的加、减、乘、快速幂、逆元和除法。
> **复杂度**：加减乘 `O(1)`；`pow / inv / 除法` 为 `O(log MOD)`。
> **限制**：当前 `inv()` 使用费马小定理，要求 `MOD` 为质数；除数必须非 `0`。

### 调用

```cpp
using mint = Mint<998244353>;

mint a = 10, b = 3;

mint x = a + b;
mint y = a * b;
mint z = a / b;
mint p = a.pow(100);

cout << x.v << '\n';
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `Mint<MOD>` | 指定模数 |
| `v` | `[0, MOD)` 内的值 |
| `+ - * /` | 模意义四则运算 |
| `pow(k)` | `a^k mod MOD` |
| `inv()` | 模逆元 |

### 模板

```cpp
template <int MOD>
struct Mint {
    int v;

    Mint(long long x = 0) {
        x %= MOD;
        if (x < 0) x += MOD;
        v = int(x);
    }

    Mint& operator+=(const Mint& o) {
        long long x = (long long)v + o.v;
        v = int(x >= MOD ? x - MOD : x);
        return *this;
    }

    Mint& operator-=(const Mint& o) {
        v -= o.v;
        if (v < 0) v += MOD;
        return *this;
    }

    Mint& operator*=(const Mint& o) {
        v = (long long)v * o.v % MOD;
        return *this;
    }

    Mint pow(long long n) const {
        Mint a = *this, res = 1;

        while (n) {
            if (n & 1) res *= a;
            a *= a;
            n >>= 1;
        }
        return res;
    }

    Mint inv() const {
        return pow(MOD - 2);
    }

    Mint& operator/=(const Mint& o) {
        return *this *= o.inv();
    }

    Mint operator-() const {
        return Mint(-v);
    }

    friend Mint operator+(Mint a, const Mint& b) {
        return a += b;
    }

    friend Mint operator-(Mint a, const Mint& b) {
        return a -= b;
    }

    friend Mint operator*(Mint a, const Mint& b) {
        return a *= b;
    }

    friend Mint operator/(Mint a, const Mint& b) {
        return a /= b;
    }
};
```

### 现场记忆

构造时统一压到 `[0, MOD)`。

快速幂：
`n & 1` 时乘答案，每轮 `a *= a, n >>= 1`。

质数模下：
`a^{-1} = a^{MOD-2}`。

`0` 没有逆元，不能作为除数。

## 1.5 STL 二分

> **用途**：在有序序列中寻找第一个 `>= x`、第一个 `> x`，以及最后一个 `< x / <= x`。
> **复杂度**：随机访问迭代器上时间 `O(log n)`。
> **注意**：序列必须按与二分相同的比较规则有序；向前移动迭代器前必须检查 `begin()`。

```cpp
// 第一个 >= x
auto it = lower_bound(a.begin(), a.end(), x);

// 第一个 > x
auto it = upper_bound(a.begin(), a.end(), x);

// 最后一个 < x
auto it = lower_bound(a.begin(), a.end(), x);
if (it != a.begin()) {
    --it;
    // use *it
}

// 最后一个 <= x
auto it = upper_bound(a.begin(), a.end(), x);
if (it != a.begin()) {
    --it;
    // use *it
}

// 第一个 >= x 的下标
int p = lower_bound(a.begin(), a.end(), x) - a.begin();

// < x 的元素个数
int cnt1 = lower_bound(a.begin(), a.end(), x) - a.begin();

// <= x 的元素个数
int cnt2 = upper_bound(a.begin(), a.end(), x) - a.begin();
```

### 现场记忆

`lower_bound(x)`：第一个 `>= x`。

`upper_bound(x)`：第一个 `> x`。

所以：
`< x` 看 `lower_bound` 前一个，
`<= x` 看 `upper_bound` 前一个。

前移前先判 `it != begin()`。

## 1.6 对拍

> **用途**：循环生成随机数据，用暴力程序校验正解并寻找反例。
> **复杂度**：每轮取决于生成器、正解和暴力程序的运行时间。
> **限制**：使用 `system()` 和 Shell 重定向，适合 Linux / macOS 环境。
> **注意**：默认按空白分隔后的 token 比较，忽略空格和换行差异；浮点误差 / Special Judge 需要自行修改比较函数。

### 调用

```cpp
int main() {
    stress();
}
```

### 模板

```cpp
const string generator = "./gen";
const string solution = "./sol";
const string brute = "./brute";

const string input_file = "data.in";
const string solution_output = "sol.out";
const string brute_output = "brute.out";

bool run_cmd(const string& cmd) {
    return system(cmd.c_str()) == 0;
}

bool same_output(const string& a, const string& b) {
    ifstream fa(a), fb(b);
    if (!fa || !fb) return false;

    string x, y;

    while (true) {
        bool pa = bool(fa >> x);
        bool pb = bool(fb >> y);

        if (pa != pb) return false;
        if (!pa) return true;
        if (x != y) return false;
    }
}

void stress() {
    long long tc = 1;

    while (true) {
        if (!run_cmd(generator + " > " + input_file)) {
            cerr << "Generator RE\n";
            return;
        }

        if (!run_cmd(solution + " < " + input_file + " > " + solution_output)) {
            cerr << "Solution RE on test " << tc << '\n';
            return;
        }

        if (!run_cmd(brute + " < " + input_file + " > " + brute_output)) {
            cerr << "Brute RE on test " << tc << '\n';
            return;
        }

        if (!same_output(solution_output, brute_output)) {
            cout << "\nWA on test " << tc << '\n';
            return;
        }

        cout << "AC " << tc++ << '\r' << flush;
    }
}
```

### 现场记忆

三个程序：
`gen` 造数据，`sol` 正解，`brute` 暴力。

每轮顺序：
生成 `data.in` → 跑两份程序 → 比较输出。

发现 WA 后 `data.in` 不会被覆盖，可直接拿来调试。

程序运行失败要单独判 RE，不能把非零返回值直接当成答案不同。

## 1.7 __int128 输入输出

> **用途**：让 `__int128` 可以直接使用 `cin / cout`。
> **复杂度**：`O(数字位数)`

```cpp
#include <bits/stdc++.h>
using namespace std;

using i128 = __int128_t;
using u128 = __uint128_t;

istream& operator>>(istream& in, i128& x) {
    string s;
    in >> s;

    bool neg = false;
    int p = 0;

    if (s[0] == '-') {
        neg = true;
        p = 1;
    }

    u128 val = 0;

    for (; p < (int)s.size(); ++p) val = val * 10 + s[p] - '0';

    x = neg ? -(i128)val : (i128)val;
    return in;
}

ostream& operator<<(ostream& out, i128 x) {
    if (x == 0) return out << '0';

    u128 y;

    if (x < 0) {
        out << '-';
        y = -(u128)x;
    } else {
        y = x;
    }

    string s;

    while (y) {
        s.push_back('0' + y % 10);
        y /= 10;
    }

    reverse(s.begin(), s.end());

    return out << s;
}
```

## 1.8 随机数 RNG

> **用途**：比赛中生成随机整数、随机打乱数组，以及给随机算法提供种子。
> **复杂度**：单次生成均摊 `O(1)`

```cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count() ^ (ull)(uintptr_t)new char);

ll rnd(ll l, ll r) {
    return uniform_int_distribution<ll>(l, r)(rng);
}
```

### 调用

```cpp
ll x = rnd(l, r);

shuffle(a.begin(), a.end(), rng);
```

## 1.9 Custom Hash / 防 Hack

> **用途**：降低 `unordered_map / unordered_set` 被构造恶意数据卡到 `O(n²)` 的风险。
> **复杂度**：期望单次 `O(1)`

### 调用

```cpp
unordered_map<long long, int, CustomHash> mp;
unordered_set<long long, CustomHash> st;

unordered_map<pair<int, int>, int, CustomHash> pairMap;
```

### 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;

struct CustomHash {
    static ull splitmix64(ull x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    size_t operator()(ull x) const {
        static const ull FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }

    template<class T, class U>
    size_t operator()(const pair<T, U>& p) const {
        ull a = std::hash<T>{}(p.first);
        ull b = std::hash<U>{}(p.second);

        return splitmix64(a + 0x9e3779b97f4a7c15ULL + (b << 6) + (b >> 2));
    }
};
```

### 现场记忆

```text
普通 unordered_map 的 hash 对整数几乎固定，
有可能被构造大量冲突。

splitmix64 + 运行时随机种子，
让对方无法提前预测桶位置。
```

## 1.10 Debug 宏

> **用途**：本地调试时快速输出变量、pair 和常用容器；提交时自动删除。
> **使用**：本地编译加 `-DLOCAL`。

```cpp
#include <bits/stdc++.h>
using namespace std;

#ifdef LOCAL

template<class T>
void _print(const T& x) {
    cerr << x;
}

template<class A, class B>
void _print(const pair<A, B>& p) {
    cerr << '(';
    _print(p.first);
    cerr << ", ";
    _print(p.second);
    cerr << ')';
}

template<class T>
void _print(const vector<T>& a) {
    cerr << '[';

    for (int i = 0; i < (int)a.size(); ++i) {
        if (i) cerr << ", ";
        _print(a[i]);
    }

    cerr << ']';
}

template<class T>
void _print(const set<T>& a) {
    cerr << '{';

    bool first = true;

    for (auto& x : a) {
        if (!first) cerr << ", ";
        first = false;
        _print(x);
    }

    cerr << '}';
}

template<class K, class V>
void _print(const map<K, V>& a) {
    cerr << '{';

    bool first = true;

    for (auto& x : a) {
        if (!first) cerr << ", ";
        first = false;
        _print(x);
    }

    cerr << '}';
}

void _debug() {
    cerr << '\n';
}

template<class T, class... Ts>
void _debug(const T& x, const Ts&... xs) {
    _print(x);

    if constexpr (sizeof...(xs)) cerr << ", ";

    _debug(xs...);
}

#define debug(...) cerr << "[" << #__VA_ARGS__ << "] = ", _debug(__VA_ARGS__)

#else

#define debug(...) ((void)0)

#endif
```

### 调用

```cpp
debug(x, y, dp);
```
