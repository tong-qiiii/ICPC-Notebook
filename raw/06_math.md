## 6.1 ModInt / 快速幂 / 逆元 / 扩展欧几里得

> **用途**：固定素数模数优先使用 `Mint<MOD>`；动态模数使用 `qpow`，一般模数逆元使用扩展欧几里得。
> **复杂度**：快速幂 / 费马逆元 `O(log b)`；扩展欧几里得 `O(log min(a,b))`。
> **限制**：`Mint::inv()` 要求 `MOD` 为素数且当前值非 `0`；`inv_mod(a, mod)` 要求 `gcd(a, mod) = 1`。

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

    Mint& operator/=(const Mint& o) {
        return *this *= o.inv();
    }

    Mint operator-() const {
        return Mint(-v);
    }

    Mint pow(long long b) const {
        Mint a = *this, res = 1;

        while (b) {
            if (b & 1) res *= a;
            a *= a;
            b >>= 1;
        }

        return res;
    }

    Mint inv() const {
        return pow(MOD - 2);
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

    friend bool operator==(const Mint& a, const Mint& b) {
        return a.v == b.v;
    }

    friend bool operator!=(const Mint& a, const Mint& b) {
        return a.v != b.v;
    }

    friend ostream& operator<<(ostream& os, const Mint& x) {
        return os << x.v;
    }

    friend istream& operator>>(istream& is, Mint& x) {
        long long v;
        is >> v;
        x = Mint(v);
        return is;
    }
};

long long qpow(long long a, long long b, long long mod) {
    a %= mod;
    if (a < 0) a += mod;

    long long res = 1 % mod;

    while (b) {
        if (b & 1)
            res = (__int128)res * a % mod;

        a = (__int128)a * a % mod;
        b >>= 1;
    }

    return res;
}

long long exgcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    long long x1, y1;
    long long g = exgcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - a / b * y1;
    return g;
}

long long inv_mod(long long a, long long mod) {
    a %= mod;
    if (a < 0) a += mod;

    long long x, y;
    long long g = exgcd(a, mod, x, y);

    if (g != 1)
        return -1;

    return (x % mod + mod) % mod;
}
```

### 现场记忆

固定素数模数：
`Mint<MOD>` 直接做 `+ - * /`，`a.inv()` 即 `a^(MOD-2)`。

动态模数快速幂：
`res *= a, a *= a`，乘法用 `__int128` 防溢出。

一般模数逆元：
`ax + mod*y = 1`，所以 `x` 就是逆元；`gcd(a, mod) != 1` 时不存在。

## 6.2 组合数预处理：阶乘 / 逆阶乘

> **用途**：预处理阶乘和逆阶乘，`O(1)` 查询模素数意义下的组合数和排列数。
> **复杂度**：预处理时间 `O(n)`，单次查询 `O(1)`，空间 `O(n)`。
> **限制**：`MOD` 必须为素数，且预处理范围要求 `n < MOD`。
> **依赖**：`6.5` 中的 `Mint<MOD>`。

### 调用

```cpp
constexpr int MOD = 1'000'000'007;

Comb<MOD> comb(n);

cout << comb.C(N, K) << '\n';
cout << comb.A(N, K) << '\n';
```

### 模板

```cpp
template <int MOD>
struct Comb {
    using Z = Mint<MOD>;

    int n = 0;
    vector<Z> fac, ifac;

    Comb() = default;

    Comb(int n_) {
        init(n_);
    }

    void init(int n_) {
        n = n_;

        fac.assign(n + 1, 1);
        ifac.assign(n + 1, 1);

        for (int i = 1; i <= n; ++i)
            fac[i] = fac[i - 1] * i;

        ifac[n] = fac[n].inv();

        for (int i = n; i >= 1; --i)
            ifac[i - 1] = ifac[i] * i;
    }

    Z C(int N, int K) const {
        if (N < 0 || K < 0 || K > N || N > n)
            return 0;

        return fac[N] * ifac[K] * ifac[N - K];
    }

    Z A(int N, int K) const {
        if (N < 0 || K < 0 || K > N || N > n)
            return 0;

        return fac[N] * ifac[N - K];
    }
};
```

### 现场记忆

`C(n,k) = n! / k! / (n-k)!`

`A(n,k) = n! / (n-k)!`

只求一次逆元：
`ifac[n] = fac[n]^{-1}`，再倒推 `ifac[i-1] = ifac[i] * i`。

必须 `n < MOD`；若 `n >= MOD`，普通阶乘逆阶乘公式不能直接这样预处理。

## 6.3 Lucas 定理

> **用途**：当 `n,k` 很大、模数为较小素数 `MOD` 时计算 `C(n,k) mod MOD`。
> **复杂度**：预处理时间 / 空间 `O(MOD)`；单次查询 `O(log_MOD n)`。
> **限制**：`MOD` 必须为素数，并且必须小到能够预处理 `0..MOD-1` 的阶乘。
> **依赖**：`6.5` 中的 `Mint<MOD>`。
> **注意**：`MOD = 1e9+7` 这类大素数不能使用这个 `O(MOD)` 版本。

### 调用

```cpp
Lucas<10007> lucas;

long long n, k;
cin >> n >> k;

cout << lucas.C(n, k) << '\n';
```

### 模板

```cpp
template <int MOD>
struct Lucas {
    using Z = Mint<MOD>;

    vector<Z> fac, ifac;

    Lucas() {
        init();
    }

    void init() {
        fac.assign(MOD, 1);
        ifac.assign(MOD, 1);

        for (int i = 1; i < MOD; ++i)
            fac[i] = fac[i - 1] * i;

        ifac[MOD - 1] = fac[MOD - 1].inv();

        for (int i = MOD - 1; i >= 1; --i)
            ifac[i - 1] = ifac[i] * i;
    }

    Z smallC(int n, int k) const {
        if (k < 0 || k > n)
            return 0;

        return fac[n] * ifac[k] * ifac[n - k];
    }

    Z C(long long n, long long k) const {
        if (n < 0 || k < 0 || k > n)
            return 0;

        Z ans = 1;

        while (n || k) {
            ans *= smallC(n % MOD, k % MOD);
            n /= MOD;
            k /= MOD;
        }

        return ans;
    }
};
```

### 现场记忆

把 `n,k` 按 `MOD` 进制拆位：

`C(n,k) ≡ ∏ C(n_i,k_i) (mod MOD)`

某一位 `k_i > n_i`，这一位组合数为 `0`，最终答案直接为 `0`。

Lucas 解决的是“`n` 很大”，不是“`MOD` 很大”；这个版本仍需 `O(MOD)` 预处理。

## 6.4 整除分块

> **用途**：将 `floor(n / i)` 相同的连续下标合并处理，常用于数论求和。
> **复杂度**：分块数量 `O(√n)`，空间 `O(1)`。

```cpp
long long floor_sum(long long n) {
    long long ans = 0;

    for (long long l = 1, r; l <= n; l = r + 1) {
        long long q = n / l;
        r = n / q;
        ans += (r - l + 1) * q;
    }

    return ans;
}
```

### 现场记忆

当前左端点 `l`：

`q = n / l`

商为 `q` 的最右位置：

`r = n / q`

然后整段 `[l,r]` 一次处理，下一段从 `r+1` 开始。

## 6.5 线性筛

> **用途**：`O(n)` 预处理素数和每个数的最小质因子。
> **复杂度**：时间 `O(n)`，空间 `O(n)`。
> **下标**：`1-indexed`。

### 调用

```cpp
LinearSieve sieve(n);

for (int p : sieve.primes)
    cout << p << ' ';

cout << sieve.minp[x] << '\n';
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `init(n)` | 筛 `1..n` |
| `primes` | 所有素数 |
| `minp[x]` | `x` 的最小质因子 |

### 模板

```cpp
struct LinearSieve {
    int n = 0;
    vector<int> primes, minp;

    LinearSieve() = default;

    LinearSieve(int n_) {
        init(n_);
    }

    void init(int n_) {
        n = n_;
        primes.clear();
        minp.assign(n + 1, 0);

        for (int i = 2; i <= n; ++i) {
            if (!minp[i]) {
                minp[i] = i;
                primes.push_back(i);
            }

            for (int p : primes) {
                if (1LL * i * p > n)
                    break;

                minp[i * p] = p;

                if (p == minp[i])
                    break;
            }
        }
    }
};
```

### 现场记忆

`minp[i] == 0` 时，`i` 是新素数。

枚举素数 `p` 筛掉 `i*p`：

`minp[i*p] = p`

当 `p == minp[i]` 时停止，保证每个合数只被最小质因子筛一次。

## 6.6 欧拉函数 / 莫比乌斯函数：线性筛

> **用途**：线性预处理 `phi`、`mu`、最小质因子和素数表。
> **复杂度**：时间 `O(n)`，空间 `O(n)`。
> **下标**：`1-indexed`。

### 调用

```cpp
MultiSieve sieve(n);

cout << sieve.phi[x] << '\n';
cout << sieve.mu[x] << '\n';
cout << sieve.minp[x] << '\n';
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `init(n)` | 预处理 `1..n` |
| `primes` | 素数表 |
| `phi[x]` | 欧拉函数 `φ(x)` |
| `mu[x]` | 莫比乌斯函数 `μ(x)` |
| `minp[x]` | 最小质因子 |

### 模板

```cpp
struct MultiSieve {
    int n = 0;
    vector<int> primes;
    vector<int> phi, mu, minp;

    MultiSieve() = default;

    MultiSieve(int n_) {
        init(n_);
    }

    void init(int n_) {
        n = n_;
        primes.clear();

        phi.assign(n + 1, 0);
        mu.assign(n + 1, 0);
        minp.assign(n + 1, 0);

        if (n >= 1) {
            phi[1] = 1;
            mu[1] = 1;
        }

        for (int i = 2; i <= n; ++i) {
            if (!minp[i]) {
                minp[i] = i;
                primes.push_back(i);

                phi[i] = i - 1;
                mu[i] = -1;
            }

            for (int p : primes) {
                if (1LL * i * p > n)
                    break;

                int x = i * p;
                minp[x] = p;

                if (p == minp[i]) {
                    phi[x] = phi[i] * p;
                    mu[x] = 0;
                    break;
                }

                phi[x] = phi[i] * (p - 1);
                mu[x] = -mu[i];
            }
        }
    }
};
```

### 现场记忆

若 `p | i`：

`phi[i*p] = phi[i] * p`

`mu[i*p] = 0`

否则：

`phi[i*p] = phi[i] * (p-1)`

`mu[i*p] = -mu[i]`

## 6.7 莫比乌斯反演：互质对数

> **用途**：统计 `1 <= i <= a, 1 <= j <= b` 中 `gcd(i,j)=1` 或 `gcd(i,j)=k` 的有序对数。
> **复杂度**：预处理时间 `O(n)`、空间 `O(n)`；单次查询约 `O(√a + √b)`。
> **下标**：`1-indexed`。
> **限制**：初始化必须满足 `n >= min(a,b)`；`gcd_eq(a,b,k)` 要求 `k >= 1`。

### 调用

```cpp
Mobius mobius(n);

cout << mobius.coprime_pairs(a, b) << '\n';
cout << mobius.gcd_eq(a, b, k) << '\n';
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `init(n)` | 预处理 `mu` 及其前缀和 |
| `mu[x]` | 莫比乌斯函数 |
| `pre[x]` | `Σ_{i=1}^x mu[i]` |
| `coprime_pairs(a,b)` | `gcd(i,j)=1` 的有序对数 |
| `gcd_eq(a,b,k)` | `gcd(i,j)=k` 的有序对数 |

### 模板

```cpp
struct Mobius {
    int n = 0;
    vector<int> primes, mu;
    vector<long long> pre;
    vector<char> vis;

    Mobius() = default;

    Mobius(int n_) {
        init(n_);
    }

    void init(int n_) {
        n = n_;
        primes.clear();

        mu.assign(n + 1, 0);
        pre.assign(n + 1, 0);
        vis.assign(n + 1, false);

        if (n >= 1)
            mu[1] = 1;

        for (int i = 2; i <= n; ++i) {
            if (!vis[i]) {
                primes.push_back(i);
                mu[i] = -1;
            }

            for (int p : primes) {
                if (1LL * i * p > n)
                    break;

                vis[i * p] = true;

                if (i % p == 0) {
                    mu[i * p] = 0;
                    break;
                }

                mu[i * p] = -mu[i];
            }
        }

        for (int i = 1; i <= n; ++i)
            pre[i] = pre[i - 1] + mu[i];
    }

    long long coprime_pairs(int a, int b) const {
        int limit = min(a, b);
        long long ans = 0;

        for (int l = 1, r; l <= limit; l = r + 1) {
            r = min(a / (a / l), b / (b / l));

            ans += (pre[r] - pre[l - 1])
                 * 1LL * (a / l) * (b / l);
        }

        return ans;
    }

    long long gcd_eq(int a, int b, int k) const {
        if (k <= 0)
            return 0;

        return coprime_pairs(a / k, b / k);
    }
};
```

### 现场记忆

互质对数：

`Σ mu[d] * floor(a/d) * floor(b/d)`

对两个整除项同时分块：

`r = min(a/(a/l), b/(b/l))`

区间 `[l,r]` 的 `mu` 用前缀和一次取出。

`gcd(i,j)=k`：先同时除以 `k`，转成互质对数。


## 6.8 矩阵快速幂

> **用途**：构造转移矩阵后快速计算线性递推、路径计数等问题。
> **复杂度**：`k × k` 矩阵快速幂时间 `O(k^3 log p)`，空间 `O(k^2)`。
> **下标**：`0-indexed`。
> **限制**：指数必须非负。
> **依赖**：`6.5` 中的 `Mint<MOD>`。

### 调用

```cpp
constexpr int MOD = 1'000'000'007;

Matrix<MOD> A(2);

A[0][0] = 1;
A[0][1] = 1;
A[1][0] = 1;

auto B = A.pow(k);

cout << B[0][0] << '\n';
```

### 模板

```cpp
template <int MOD>
struct Matrix {
    using Z = Mint<MOD>;

    int n = 0;
    vector<vector<Z>> a;

    Matrix() = default;

    Matrix(int n_, bool identity = false) {
        init(n_, identity);
    }

    void init(int n_, bool identity = false) {
        n = n_;
        a.assign(n, vector<Z>(n));

        if (identity) {
            for (int i = 0; i < n; ++i)
                a[i][i] = 1;
        }
    }

    vector<Z>& operator[](int i) {
        return a[i];
    }

    const vector<Z>& operator[](int i) const {
        return a[i];
    }

    Matrix operator*(const Matrix& o) const {
        Matrix res(n);

        for (int i = 0; i < n; ++i) {
            for (int k = 0; k < n; ++k) {
                if (a[i][k] == Z(0))
                    continue;

                for (int j = 0; j < n; ++j)
                    res[i][j] += a[i][k] * o[k][j];
            }
        }

        return res;
    }

    Matrix pow(long long b) const {
        Matrix base = *this;
        Matrix res(n, true);

        while (b) {
            if (b & 1)
                res = res * base;

            base = base * base;
            b >>= 1;
        }

        return res;
    }
};
```

### 现场记忆

矩阵快速幂和普通快速幂完全一样：

`res = I`

指数当前位为 `1`：`res = res * base`

每轮：`base = base * base`

矩阵乘法顺序不能反，通常转移写成 `state' = A * state`。

## 6.9 高斯消元：实数线性方程组

> **用途**：求解实数线性方程组，并判断无解、唯一解或多解。
> **复杂度**：`n` 个方程、`m` 个未知数时约 `O(nm min(n,m))`，空间 `O(nm)`。
> **下标**：增广矩阵为 `n × (m+1)`。
> **注意**：实数消元必须选绝对值最大的主元，避免数值误差。

### 调用

```cpp
GaussD gauss;
vector<double> ans;

int rank = gauss.solve(a, ans);

if (rank == -1) {
    // 无解
} else if (rank < m) {
    // 多解，ans 为其中一组解
} else {
    // 唯一解
}
```

### 模板

```cpp
struct GaussD {
    static constexpr double EPS = 1e-9;

    int solve(vector<vector<double>> a, vector<double>& ans) const {
        int n = a.size();
        int m = a[0].size() - 1;

        vector<int> where(m, -1);
        int row = 0;

        for (int col = 0; col < m && row < n; ++col) {
            int sel = row;

            for (int i = row; i < n; ++i) {
                if (fabs(a[i][col]) > fabs(a[sel][col]))
                    sel = i;
            }

            if (fabs(a[sel][col]) < EPS)
                continue;

            swap(a[sel], a[row]);
            where[col] = row;

            double inv = 1.0 / a[row][col];

            for (int j = col; j <= m; ++j)
                a[row][j] *= inv;

            for (int i = 0; i < n; ++i) {
                if (i == row)
                    continue;

                double factor = a[i][col];

                if (fabs(factor) < EPS)
                    continue;

                for (int j = col; j <= m; ++j)
                    a[i][j] -= factor * a[row][j];
            }

            ++row;
        }

        for (int i = 0; i < n; ++i) {
            bool zero = true;

            for (int j = 0; j < m; ++j) {
                if (fabs(a[i][j]) >= EPS) {
                    zero = false;
                    break;
                }
            }

            if (zero && fabs(a[i][m]) >= EPS) {
                ans.clear();
                return -1;
            }
        }

        ans.assign(m, 0);

        for (int i = 0; i < m; ++i) {
            if (where[i] != -1)
                ans[i] = a[where[i]][m];
        }

        return row;
    }
};
```

### 现场记忆

每列从剩余行中选 `|a[i][col]|` 最大的主元。

没有主元：这一列是自由变量。

出现：

`0x1 + 0x2 + ... + 0xm = c (c != 0)`

则无解。

否则 `rank == m` 唯一解，`rank < m` 多解。

## 6.10 高斯消元：模素数线性方程组

> **用途**：求解素数模意义下的线性方程组；异或方程可直接取 `MOD = 2`。
> **复杂度**：消元 `O(nm min(n,m))`，另有至多 `O(min(n,m) log MOD)` 的求逆；空间 `O(nm)`。
> **下标**：增广矩阵为 `n × (m+1)`。
> **限制**：`MOD` 必须为素数。
> **依赖**：`6.5` 中的 `Mint<MOD>`。

### 调用

```cpp
using Z = Mint<998244353>;

vector<vector<Z>> a(n, vector<Z>(m + 1));

GaussM<998244353> gauss;
vector<Z> ans;

int rank = gauss.solve(a, ans);

if (rank == -1) {
    // 无解
} else if (rank < m) {
    // 多解，ans 为其中一组解
} else {
    // 唯一解
}
```

### 模板

```cpp
template <int MOD>
struct GaussM {
    using Z = Mint<MOD>;

    int solve(vector<vector<Z>> a, vector<Z>& ans) const {
        int n = a.size();
        int m = a[0].size() - 1;

        vector<int> where(m, -1);
        int row = 0;

        for (int col = 0; col < m && row < n; ++col) {
            int sel = -1;

            for (int i = row; i < n; ++i) {
                if (a[i][col] != Z(0)) {
                    sel = i;
                    break;
                }
            }

            if (sel == -1)
                continue;

            swap(a[sel], a[row]);
            where[col] = row;

            Z inv = a[row][col].inv();

            for (int j = col; j <= m; ++j)
                a[row][j] *= inv;

            for (int i = 0; i < n; ++i) {
                if (i == row || a[i][col] == Z(0))
                    continue;

                Z factor = a[i][col];

                for (int j = col; j <= m; ++j)
                    a[i][j] -= factor * a[row][j];
            }

            ++row;
        }

        for (int i = 0; i < n; ++i) {
            bool zero = true;

            for (int j = 0; j < m; ++j) {
                if (a[i][j] != Z(0)) {
                    zero = false;
                    break;
                }
            }

            if (zero && a[i][m] != Z(0)) {
                ans.clear();
                return -1;
            }
        }

        ans.assign(m, 0);

        for (int i = 0; i < m; ++i) {
            if (where[i] != -1)
                ans[i] = a[where[i]][m];
        }

        return row;
    }
};
```

### 现场记忆

模素数下和普通高斯一样，只是：

`除以 pivot -> 乘 pivot.inv()`

没有主元：该变量可能是自由变量。

出现：

`0x1 + ... + 0xm = c (c != 0)`

则无解。

返回 `-1` 表示无解；否则返回秩，`rank == m` 唯一解。


## 6.11 容斥原理：子集枚举

> **用途**：枚举限制集合的非空子集，用奇加偶减计算若干集合的并集贡献。
> **复杂度**：`k` 个限制通常为 `O(2^k)`。
> **限制**：使用 64 位状态，要求 `k < 64`；实际受 `2^k` 限制通常远小于此。

```cpp
template <class F>
auto subset_ie(int m, F work) {
    using T = decltype(work(1ULL));
    T ans = 0;

    for (uint64_t mask = 1; mask < (1ULL << m); ++mask) {
        T val = work(mask);

        if (__builtin_popcountll(mask) & 1)
            ans += val;
        else
            ans -= val;
    }

    return ans;
}

// 1..n 中至少被 d 中一个正整数整除的数的个数
long long div_any(long long n, const vector<long long>& d) {
    int m = d.size();
    long long ans = 0;

    for (uint64_t mask = 1; mask < (1ULL << m); ++mask) {
        __int128 lcm = 1;
        bool valid = true;

        for (int i = 0; i < m; ++i) {
            if (!(mask >> i & 1))
                continue;

            long long g = gcd((long long)lcm, d[i]);
            lcm = lcm / g * d[i];

            if (lcm > n) {
                valid = false;
                break;
            }
        }

        if (!valid)
            continue;

        long long cnt = n / (long long)lcm;

        if (__builtin_popcountll(mask) & 1)
            ans += cnt;
        else
            ans -= cnt;
    }

    return ans;
}
```

### 现场记忆

并集容斥：

`|A1 ∪ A2 ∪ ...| = 单个 - 两两交 + 三个交 - ...`

子集大小为奇数加，偶数减。

“被若干数至少一个整除”：

子集交集对应 `lcm`，贡献为 `floor(n / lcm)`。

`lcm > n` 后该子集贡献直接为 `0`。


## 6.12 自适应辛普森积分

> **用途**：数值计算定积分，对较平滑函数自动细分误差较大的区间。
> **复杂度**：与函数形状和精度要求有关；空间为递归深度。
> **注意**：被积函数应在积分区间内有定义且数值稳定；`eps` 为绝对误差控制量。

```cpp
template <class F>
double simpson(const F& f, double l, double r) {
    double mid = (l + r) / 2;
    return (f(l) + 4 * f(mid) + f(r)) * (r - l) / 6;
}

template <class F>
double adaptive_simpson_dfs(
    const F& f,
    double l,
    double r,
    double whole,
    double eps,
    int dep
) {
    double mid = (l + r) / 2;
    double left = simpson(f, l, mid);
    double right = simpson(f, mid, r);
    double delta = left + right - whole;

    if (dep == 0 || mid == l || mid == r ||
        fabs(delta) <= 15 * eps) {
        return left + right + delta / 15;
    }

    return adaptive_simpson_dfs(
               f, l, mid, left, eps / 2, dep - 1
           ) +
           adaptive_simpson_dfs(
               f, mid, r, right, eps / 2, dep - 1
           );
}

template <class F>
double adaptive_simpson(
    const F& f,
    double l,
    double r,
    double eps = 1e-8,
    int max_dep = 30
) {
    if (l == r)
        return 0;

    if (l > r)
        return -adaptive_simpson(f, r, l, eps, max_dep);

    return adaptive_simpson_dfs(
        f, l, r, simpson(f, l, r), eps, max_dep
    );
}
```

### 调用

```cpp
auto f = [&](double x) {
    return sin(x);
};

double ans = adaptive_simpson(f, 0.0, acos(-1.0));
```

### 现场记忆

整段 Simpson 为 `S`，左右两半为 `L,R`。

若：

`|L + R - S| <= 15 * eps`

则接受，并做 Richardson 修正：

`L + R + (L + R - S) / 15`

否则左右递归，误差各分 `eps / 2`。

## 6.13 CRT 中国剩余定理

> **用途**：合并模数两两互质的同余方程 `x ≡ r[i] (mod m[i])`。
> **复杂度**：时间 `O(k log M)`，空间 `O(1)`
> **限制**：模数两两互质，所有模数乘积需要能放进 `long long`。
> **返回**：`{最小非负解, 总模数}`。

```cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using i128 = __int128_t;

ll exgcd(ll a, ll b, ll& x, ll& y) {
    if (!b) {
        x = 1;
        y = 0;
        return a;
    }

    ll x1, y1;
    ll g = exgcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - a / b * y1;

    return g;
}

ll norm(ll x, ll mod) {
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

pair<ll, ll> crt(const vector<ll>& r, const vector<ll>& m) {
    ll ans = 0, mod = 1;

    for (int i = 0; i < (int)r.size(); ++i) {
        ll x, y;
        exgcd(mod, m[i], x, y);

        ll d = norm(r[i] - ans, m[i]);
        ll t = (ll)((i128)d * x % m[i]);
        t = norm(t, m[i]);

        i128 newMod = (i128)mod * m[i];
        i128 newAns = (i128)ans + (i128)mod * t;

        ans = (ll)(newAns % newMod);
        mod = (ll)newMod;
    }

    return {ans, mod};
}
```

### 现场记忆

```text
当前：
x ≡ ans (mod M)

加入：
x ≡ r (mod m)

令 x = ans + M*t：
M*t ≡ r-ans (mod m)

两模数互质 -> M 有逆元。
```

## 6.14 Miller-Rabin 素性测试

> **用途**：快速判断一个 `uint64_t` 整数是否为质数。
> **复杂度**：约 `O(log n)` 次模乘，固定常数轮数
> **限制**：代码中的固定底数集合对 `uint64_t` 范围确定正确。

```cpp
#include <bits/stdc++.h>
using namespace std;

using u64 = uint64_t;
using u128 = __uint128_t;

u64 mulMod(u64 a, u64 b, u64 mod) {
    return (u128)a * b % mod;
}

u64 powMod(u64 a, u64 e, u64 mod) {
    u64 res = 1;

    while (e) {
        if (e & 1) res = mulMod(res, a, mod);
        a = mulMod(a, a, mod);
        e >>= 1;
    }

    return res;
}

bool isPrime(u64 n) {
    if (n < 2) return false;

    for (u64 p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
        if (n % p == 0) return n == p;
    }

    u64 d = n - 1, s = 0;

    while (!(d & 1)) {
        d >>= 1;
        ++s;
    }

    for (u64 a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
        if (a % n == 0) continue;

        u64 x = powMod(a % n, d, n);

        if (x == 1 || x == n - 1) continue;

        bool composite = true;

        for (u64 r = 1; r < s; ++r) {
            x = mulMod(x, x, n);

            if (x == n - 1) {
                composite = false;
                break;
            }
        }

        if (composite) return false;
    }

    return true;
}
```

### 现场记忆

```text
n-1 = d * 2^s，d 为奇数。

检查：
a^d 是否为 1 / -1
否则不断平方，看能否出现 -1。

64 位确定底数：
2, 325, 9375, 28178,
450775, 9780504, 1795265022
```

## 6.15 Pollard-Rho 大整数分解

> **用途**：分解 `uint64_t` 范围内的大整数质因数。
> **复杂度**：随机算法，分解因子 `p` 的期望复杂度约 `O(p^{1/2})`；通常远快于试除到 `√n`
> **限制**：依赖 Miller-Rabin 判断质数。

### 调用

```cpp
u64 n;
cin >> n;

auto fac = factorize(n);

for (u64 p : fac) cout << p << ' ';
```

### 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

using u64 = uint64_t;
using u128 = __uint128_t;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

u64 mulMod(u64 a, u64 b, u64 mod) {
    return (u128)a * b % mod;
}

u64 powMod(u64 a, u64 e, u64 mod) {
    u64 res = 1;

    while (e) {
        if (e & 1) res = mulMod(res, a, mod);
        a = mulMod(a, a, mod);
        e >>= 1;
    }

    return res;
}

bool isPrime(u64 n) {
    if (n < 2) return false;

    for (u64 p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
        if (n % p == 0) return n == p;
    }

    u64 d = n - 1, s = 0;

    while (!(d & 1)) {
        d >>= 1;
        ++s;
    }

    for (u64 a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
        if (a % n == 0) continue;

        u64 x = powMod(a % n, d, n);

        if (x == 1 || x == n - 1) continue;

        bool composite = true;

        for (u64 r = 1; r < s; ++r) {
            x = mulMod(x, x, n);

            if (x == n - 1) {
                composite = false;
                break;
            }
        }

        if (composite) return false;
    }

    return true;
}

u64 pollardRho(u64 n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;

    while (true) {
        u64 c = uniform_int_distribution<u64>(1, n - 1)(rng);
        u64 x = uniform_int_distribution<u64>(0, n - 1)(rng);
        u64 y = x;
        u64 d = 1;

        auto f = [&](u64 x) {
            return (mulMod(x, x, n) + c) % n;
        };

        while (d == 1) {
            x = f(x);
            y = f(f(y));

            u64 diff = x > y ? x - y : y - x;
            d = gcd(diff, n);
        }

        if (d != n) return d;
    }
}

void factorDfs(u64 n, vector<u64>& fac) {
    if (n == 1) return;

    if (isPrime(n)) {
        fac.push_back(n);
        return;
    }

    u64 d = pollardRho(n);

    factorDfs(d, fac);
    factorDfs(n / d, fac);
}

vector<u64> factorize(u64 n) {
    vector<u64> fac;

    factorDfs(n, fac);
    sort(fac.begin(), fac.end());

    return fac;
}
```

### 现场记忆

```text
Miller-Rabin：判断当前数是不是质数。

Pollard-Rho：
随机 f(x)=x²+c
Floyd 找环，同时 gcd(|x-y|,n) 找因子。

找到非平凡因子 d 后：
递归分解 d 和 n/d。

随机失败 / 得到 n -> 换随机参数重新跑。
```

## 6.16 BSGS / exBSGS 离散对数

> **用途**：求最小非负整数 `x` 满足 `a^x ≡ b (mod m)`；`exBSGS` 支持 `gcd(a,m) != 1`。
> **复杂度**：时间、空间均约 `O(√m)`
> **返回**：无解返回 `-1`。
> **注意**：`m` 很大时 `O(√m)` 的哈希表可能无法承受。

### 调用

```cpp
cout << exBSGS(a, b, mod) << '\n';
```

### 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using i128 = __int128_t;

ll mulMod(ll a, ll b, ll mod) {
    return (i128)a * b % mod;
}

ll powMod(ll a, ll e, ll mod) {
    ll res = 1 % mod;

    while (e) {
        if (e & 1) res = mulMod(res, a, mod);
        a = mulMod(a, a, mod);
        e >>= 1;
    }

    return res;
}

ll exgcd(ll a, ll b, ll& x, ll& y) {
    if (!b) {
        x = 1;
        y = 0;
        return a;
    }

    ll x1, y1;
    ll g = exgcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - a / b * y1;

    return g;
}

ll invMod(ll a, ll mod) {
    ll x, y;
    ll g = exgcd(a, mod, x, y);

    if (g != 1) return -1;

    x %= mod;
    if (x < 0) x += mod;

    return x;
}

ll BSGS(ll a, ll b, ll mod) {
    if (mod == 1) return 0;

    a %= mod;
    b %= mod;

    if (b == 1) return 0;
    if (gcd(a, mod) != 1) return -1;

    ll n = sqrtl((long double)mod);

    while ((i128)n * n < mod) ++n;
    while (n > 0 && (i128)(n - 1) * (n - 1) >= mod) --n;

    unordered_map<ll, ll> mp;

    ll cur = 1;

    for (ll j = 0; j < n; ++j) {
        if (!mp.count(cur)) mp[cur] = j;
        cur = mulMod(cur, a, mod);
    }

    ll inv = invMod(a, mod);
    ll step = powMod(inv, n, mod);

    cur = b;

    for (ll i = 0; i <= n; ++i) {
        auto it = mp.find(cur);

        if (it != mp.end()) return i * n + it->second;

        cur = mulMod(cur, step, mod);
    }

    return -1;
}

ll exBSGS(ll a, ll b, ll mod) {
    if (mod == 1) return 0;

    a %= mod;
    b %= mod;

    if (b == 1) return 0;

    ll cnt = 0;
    ll k = 1 % mod;

    while (true) {
        ll g = gcd(a, mod);

        if (g == 1) break;
        if (b % g) return -1;

        mod /= g;
        b /= g;

        k = mulMod(k, a / g, mod);
        ++cnt;

        if (k == b) return cnt;
    }

    ll inv = invMod(k, mod);
    ll target = mulMod(b, inv, mod);
    ll res = BSGS(a, target, mod);

    return res == -1 ? -1 : res + cnt;
}
```

### 现场记忆

```text
BSGS：
x = i*m + j
a^j 表一边，a^{-m*i} * b 表另一边。

exBSGS：
g = gcd(a,m)

若 b % g != 0 -> 无解
否则同时除掉 g，并记录已经强制消掉多少个 a。

直到 gcd(a,m)=1，再进入普通 BSGS。
```

## 6.17 floor_sum

> **用途**：快速计算 `Σ floor((a*i+b)/m)`，`i = 0..n-1`。
> **复杂度**：时间 `O(log m)`，空间 `O(1)`
> **限制**：`n >= 0`，`m > 0`；代码支持 `a,b` 为负数。
> **返回**：使用 `__int128` 避免求和溢出。

```cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using i128 = __int128_t;

ll safeMod(ll x, ll m) {
    x %= m;
    if (x < 0) x += m;
    return x;
}

i128 floorSumUnsigned(ll n, ll m, ll a, ll b) {
    i128 ans = 0;

    while (true) {
        if (a >= m) {
            ans += (i128)(n - 1) * n * (a / m) / 2;
            a %= m;
        }

        if (b >= m) {
            ans += (i128)n * (b / m);
            b %= m;
        }

        i128 y = (i128)a * n + b;

        if (y < m) break;

        n = (ll)(y / m);
        b = (ll)(y % m);
        swap(a, m);
    }

    return ans;
}

i128 floorSum(ll n, ll m, ll a, ll b) {
    i128 ans = 0;

    if (a < 0) {
        ll a2 = safeMod(a, m);
        ans -= (i128)n * (n - 1) / 2 * ((a2 - (i128)a) / m);
        a = a2;
    }

    if (b < 0) {
        ll b2 = safeMod(b, m);
        ans -= (i128)n * ((b2 - (i128)b) / m);
        b = b2;
    }

    return ans + floorSumUnsigned(n, m, a, b);
}
```

### 现场记忆

```text
求：
Σ floor((a*i+b)/m)

先把 a/m、b/m 的整数部分直接贡献到答案。

剩余 a<m,b<m 后：
交换坐标轴，问题规模类似 Euclid 下降。

复杂度和 gcd 一样是 O(log m)。
```

## 6.18 Cipolla / Tonelli-Shanks 模平方根

> **用途**：在奇质数模 `p` 下求 `x² ≡ n (mod p)`；这里采用 Tonelli-Shanks。
> **复杂度**：通常 `O(log² p)`
> **限制**：`p` 必须为质数。
> **返回**：返回一个平方根；无解返回 `-1`。另一个根为 `p-x`。

```cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using i128 = __int128_t;

ll mulMod(ll a, ll b, ll mod) {
    return (i128)a * b % mod;
}

ll powMod(ll a, ll e, ll mod) {
    ll res = 1 % mod;

    while (e) {
        if (e & 1) res = mulMod(res, a, mod);
        a = mulMod(a, a, mod);
        e >>= 1;
    }

    return res;
}

ll modSqrt(ll n, ll p) {
    n %= p;
    if (n < 0) n += p;

    if (n == 0) return 0;
    if (p == 2) return n;
    if (powMod(n, (p - 1) / 2, p) != 1) return -1;
    if (p % 4 == 3) return powMod(n, (p + 1) / 4, p);

    ll q = p - 1;
    int s = 0;

    while (!(q & 1)) {
        q >>= 1;
        ++s;
    }

    ll z = 2;

    while (powMod(z, (p - 1) / 2, p) != p - 1) ++z;

    ll c = powMod(z, q, p);
    ll x = powMod(n, (q + 1) / 2, p);
    ll t = powMod(n, q, p);
    int m = s;

    while (t != 1) {
        ll cur = t;
        int i = 0;

        while (cur != 1) {
            cur = mulMod(cur, cur, p);
            ++i;
        }

        ll b = c;

        for (int j = 0; j < m - i - 1; ++j) b = mulMod(b, b, p);

        x = mulMod(x, b, p);
        ll b2 = mulMod(b, b, p);

        t = mulMod(t, b2, p);
        c = b2;
        m = i;
    }

    return x;
}
```

### 现场记忆

```text
先 Euler 判据：
n^((p-1)/2) != 1 -> 无平方根。

p % 4 == 3：
直接 n^((p+1)/4)。

一般情况：
p-1 = q * 2^s，q 为奇数
找一个二次非剩余 z，再做 Tonelli-Shanks。
```
