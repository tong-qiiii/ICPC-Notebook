## 8.1 多项式核心 - NTT / Poly

> **用途**：模 `998244353` 下的多项式基础运算与 NTT 卷积；其余 FPS / 求值 / 复合接口统一声明在 `Poly` 中。
> **复杂度**：乘法 `O(n log n)`，加减 / 截断 / 翻转 `O(n)`。
> **下标**：`0-indexed`，`a[i] = [x^i]f(x)`。
> **限制**：单次 NTT 长度 `<= 2^23`。
> **注意**：后续多项式板块直接补充本块中声明的成员函数，完整库按顺序全部粘贴。

### 调用

```cpp
using namespace poly998;

Poly f({1, 2, 3});
Poly g({4, 5});

Poly h = f * g;
cout << h[2] << '\n';
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `Poly(v)` | 系数数组构造 |
| `f.a` | 系数数组 |
| `f[i]` | `[x^i]f`，越界返回 `0` |
| `f.raw(i)` | 直接引用 `a[i]` |
| `f.size()` / `f.deg()` | 系数长度 / 次数 |
| `f.trim()` / `normalize()` | 删除高位 `0` |
| `f.mod_xk(k)` | `f mod x^k` |
| `f.mul_xk(k)` | `f * x^k` |
| `f.div_xk(k)` | 丢掉低 `k` 项 |
| `f.rev(n)` | 按长度 `n` 翻转 |
| `convolution(a,b)` | NTT 卷积 |
| `qpow(a,k)` | 模幂 |
| `invmod(a)` / `inv_int(a)` | 模逆 |
| `mod_sqrt(a)` | 模平方根，可能无解 |

### 模板

```cpp
#pragma once
#include <bits/stdc++.h>
using namespace std;

namespace poly998 {

constexpr int MOD = 998244353;
constexpr int G = 3;
constexpr int MAX_LOG = 23; // MOD - 1 = 119 * 2^23

int norm(long long x) {
    x %= MOD;
    if (x < 0) x += MOD;
    return (int)x;
}

int qpow(long long a, long long e) {
    a = norm(a);
    long long r = 1;
    while (e) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return (int)r;
}

int invmod(long long x) {
    int a = norm(x);
    assert(a != 0);
    return qpow(a, MOD - 2);
}

int inv_int(int x) {
    return invmod(x);
}

inline vector<int> fac{1}, ifac{1};

void ensure_fact(int n) {
    assert(0 <= n && n < MOD);
    int old = (int)fac.size() - 1;
    if (old >= n) return;

    fac.resize(n + 1);
    for (int i = old + 1; i <= n; ++i) {
        fac[i] = (int)(1LL * fac[i - 1] * i % MOD);
    }

    ifac.resize(n + 1);
    ifac[n] = invmod(fac[n]);
    for (int i = n; i > old; --i) {
        ifac[i - 1] = (int)(1LL * ifac[i] * i % MOD);
    }
}

vector<int> batch_inv(vector<int> a) {
    int n = (int)a.size();
    vector<int> pre(n + 1, 1), res(n);

    for (int i = 0; i < n; ++i) {
        a[i] = norm(a[i]);
        pre[i + 1] = pre[i];
        if (a[i]) pre[i + 1] = (int)(1LL * pre[i + 1] * a[i] % MOD);
    }

    int suf = invmod(pre[n]);
    for (int i = n - 1; i >= 0; --i) {
        if (!a[i]) continue;
        res[i] = (int)(1LL * pre[i] * suf % MOD);
        suf = (int)(1LL * suf * a[i] % MOD);
    }

    return res;
}

optional<int> mod_sqrt(int a) {
    a = norm(a);
    if (a == 0) return 0;
    if (qpow(a, (MOD - 1) / 2) != 1) return nullopt;

    int s = 0, q = MOD - 1;
    while ((q & 1) == 0) {
        ++s;
        q >>= 1;
    }

    int z = 2;
    while (qpow(z, (MOD - 1) / 2) != MOD - 1) ++z;

    long long c = qpow(z, q);
    long long x = qpow(a, (q + 1) / 2);
    long long t = qpow(a, q);
    int m = s;

    while (t != 1) {
        int i = 1;
        long long tt = t * t % MOD;

        while (i < m && tt != 1) {
            tt = tt * tt % MOD;
            ++i;
        }

        assert(i < m);

        long long b = qpow(c, 1LL << (m - i - 1));
        x = x * b % MOD;
        t = t * b % MOD * b % MOD;
        c = b * b % MOD;
        m = i;
    }

    return (int)x;
}

void ntt(vector<int> &a, bool invert) {
    int n = (int)a.size();
    if (n <= 1) return;

    assert((n & (n - 1)) == 0);
    assert(n <= (1 << MAX_LOG));

    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        int wlen = qpow(G, (MOD - 1) / len);
        if (invert) wlen = invmod(wlen);

        for (int i = 0; i < n; i += len) {
            long long w = 1;
            int half = len >> 1;

            for (int j = 0; j < half; ++j) {
                int u = a[i + j];
                int v = (int)(w * a[i + j + half] % MOD);

                int x = u + v;
                if (x >= MOD) x -= MOD;
                a[i + j] = x;

                x = u - v;
                if (x < 0) x += MOD;
                a[i + j + half] = x;

                w = w * wlen % MOD;
            }
        }
    }

    if (invert) {
        int inv_n = invmod(n);
        for (int &x : a) x = (int)(1LL * x * inv_n % MOD);
    }
}

vector<int> convolution(const vector<int> &a, const vector<int> &b) {
    if (a.empty() || b.empty()) return {};

    if (1LL * a.size() * b.size() <= 4096) {
        vector<int> c(a.size() + b.size() - 1);

        for (int i = 0; i < (int)a.size(); ++i) {
            int x = norm(a[i]);
            for (int j = 0; j < (int)b.size(); ++j) {
                c[i + j] = (c[i + j] + 1LL * x * norm(b[j])) % MOD;
            }
        }

        return c;
    }

    int need = (int)a.size() + (int)b.size() - 1;
    int n = 1;
    while (n < need) n <<= 1;

    assert(n <= (1 << MAX_LOG));

    vector<int> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    for (int &x : fa) x = norm(x);
    for (int &x : fb) x = norm(x);

    fa.resize(n);
    fb.resize(n);

    ntt(fa, false);
    ntt(fb, false);

    for (int i = 0; i < n; ++i) {
        fa[i] = (int)(1LL * fa[i] * fb[i] % MOD);
    }

    ntt(fa, true);
    fa.resize(need);
    return fa;
}

struct Poly {
    vector<int> a;

    Poly() = default;
    Poly(long long c);
    Poly(initializer_list<int> v);
    explicit Poly(vector<int> v);

    void trim();
    void normalize();

    int size() const;
    bool empty() const;
    int deg() const;

    int operator[](int i) const;
    int &raw(int i);

    Poly mod_xk(int k) const;
    Poly mul_xk(int k) const;
    Poly div_xk(int k) const;
    Poly rev(int n = -1) const;

    Poly deriv() const;
    Poly integr() const;

    Poly inv(int n) const;
    Poly log(int n) const;
    Poly exp(int n) const;
    Poly pow(long long k, int n) const;
    optional<Poly> sqrt(int n) const;

    Poly sin(int n) const;
    Poly cos(int n) const;
    Poly tan(int n) const;
    Poly asin(int n) const;
    Poly atan(int n) const;

    pair<Poly, Poly> divmod(const Poly &b) const;

    vector<int> eval(const vector<int> &xs) const;
    static optional<Poly> interpolate(const vector<int> &xs, const vector<int> &ys);

    Poly shift(long long c) const;

    Poly compose_slow(const Poly &g, int n) const;
    Poly compositional_inverse_slow(int n) const;

    friend bool operator==(const Poly &x, const Poly &y);
    friend bool operator!=(const Poly &x, const Poly &y);

    friend Poly operator+(const Poly &x, const Poly &y);
    friend Poly operator-(const Poly &x, const Poly &y);
    friend Poly operator-(const Poly &x);
    friend Poly operator*(const Poly &x, const Poly &y);
    friend Poly operator*(const Poly &x, long long k);
    friend Poly operator*(long long k, const Poly &x);
    friend Poly operator/(const Poly &x, long long k);
    friend Poly operator/(const Poly &x, const Poly &y);
    friend Poly operator%(const Poly &x, const Poly &y);

    Poly &operator+=(const Poly &o);
    Poly &operator-=(const Poly &o);
    Poly &operator*=(const Poly &o);
    Poly &operator*=(long long k);
    Poly &operator/=(long long k);

private:
    optional<Poly> sqrt_nonzero_constant(int n) const;
};

Poly::Poly(long long c) {
    c = norm(c);
    if (c) a = {(int)c};
}

Poly::Poly(initializer_list<int> v) : a(v) {
    for (int &x : a) x = norm(x);
    trim();
}

Poly::Poly(vector<int> v) : a(move(v)) {
    for (int &x : a) x = norm(x);
    trim();
}

void Poly::trim() {
    while (!a.empty() && a.back() == 0) a.pop_back();
}

void Poly::normalize() {
    trim();
}

int Poly::size() const {
    return (int)a.size();
}

bool Poly::empty() const {
    return a.empty();
}

int Poly::deg() const {
    return size() - 1;
}

int Poly::operator[](int i) const {
    return 0 <= i && i < size() ? a[i] : 0;
}

int &Poly::raw(int i) {
    return a[i];
}

Poly Poly::mod_xk(int k) const {
    if (k <= 0 || empty()) return Poly();
    if (k >= size()) return *this;
    return Poly(vector<int>(a.begin(), a.begin() + k));
}

Poly Poly::mul_xk(int k) const {
    assert(k >= 0);
    if (empty()) return Poly();

    vector<int> r(k);
    r.insert(r.end(), a.begin(), a.end());
    return Poly(move(r));
}

Poly Poly::div_xk(int k) const {
    if (k <= 0) return *this;
    if (k >= size()) return Poly();
    return Poly(vector<int>(a.begin() + k, a.end()));
}

Poly Poly::rev(int n) const {
    if (n < 0) n = size();

    vector<int> r = a;
    r.resize(n);
    reverse(r.begin(), r.end());

    return Poly(move(r));
}

bool operator==(const Poly &x, const Poly &y) {
    return x.a == y.a;
}

bool operator!=(const Poly &x, const Poly &y) {
    return !(x == y);
}

Poly operator+(const Poly &x, const Poly &y) {
    vector<int> r(max(x.size(), y.size()));

    for (int i = 0; i < (int)r.size(); ++i) {
        int v = x[i] + y[i];
        if (v >= MOD) v -= MOD;
        r[i] = v;
    }

    return Poly(move(r));
}

Poly operator-(const Poly &x, const Poly &y) {
    vector<int> r(max(x.size(), y.size()));

    for (int i = 0; i < (int)r.size(); ++i) {
        int v = x[i] - y[i];
        if (v < 0) v += MOD;
        r[i] = v;
    }

    return Poly(move(r));
}

Poly operator-(const Poly &x) {
    vector<int> r = x.a;

    for (int &v : r) {
        if (v) v = MOD - v;
    }

    return Poly(move(r));
}

Poly operator*(const Poly &x, const Poly &y) {
    return Poly(convolution(x.a, y.a));
}

Poly operator*(const Poly &x, long long k) {
    int c = norm(k);
    if (c == 0 || x.empty()) return Poly();

    vector<int> r = x.a;
    for (int &v : r) v = (int)(1LL * v * c % MOD);

    return Poly(move(r));
}

Poly operator*(long long k, const Poly &x) {
    return x * k;
}

Poly operator/(const Poly &x, long long k) {
    return x * invmod(k);
}

Poly &Poly::operator+=(const Poly &o) {
    return *this = *this + o;
}

Poly &Poly::operator-=(const Poly &o) {
    return *this = *this - o;
}

Poly &Poly::operator*=(const Poly &o) {
    return *this = *this * o;
}

Poly &Poly::operator*=(long long k) {
    return *this = *this * k;
}

Poly &Poly::operator/=(long long k) {
    return *this = *this / k;
}

} // namespace poly998
```

### 现场记忆

```text
998244353 = 119 * 2^23 + 1，原根 G = 3。
NTT 长度必须是 2 的幂，且 <= 2^23。
Poly 自动去掉末尾 0；零多项式 empty()，deg() = -1。
mod_xk(k) = 只保留 0 ... k-1 次项。
```


## 8.2 形式幂级数 FPS - 逆 / ln / exp / 幂 / sqrt / 三角函数

> **用途**：形式幂级数的求导、积分、逆、对数、指数、整数幂、平方根及三角 / 反三角函数。
> **复杂度**：主要操作 `O(M(n))`，其中 `M(n)=O(n log n)`。
> **下标**：`0-indexed`。
> **依赖**：`多项式核心 - NTT / Poly`。
> **注意**：`inv`: `f[0]!=0`；`log`: `f[0]==1`；`exp/sin/cos/tan/asin/atan`: `f[0]==0`。

### 调用

```cpp
Poly f({1, 2, 3});

Poly fi = f.inv(10);
Poly ln = f.log(10);
Poly pw = f.pow(100, 10);

auto rt = f.sqrt(10);
if (rt) {
    Poly g = *rt;
}
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `f.deriv()` | 求导 |
| `f.integr()` | 积分，常数项取 `0` |
| `f.inv(n)` | `1/f mod x^n` |
| `f.log(n)` | `ln(f) mod x^n` |
| `f.exp(n)` | `exp(f) mod x^n` |
| `f.pow(k,n)` | `f^k mod x^n` |
| `f.sqrt(n)` | 平方根，可能无解 |
| `f.sin(n)` | `sin(f) mod x^n` |
| `f.cos(n)` | `cos(f) mod x^n` |
| `f.tan(n)` | `tan(f) mod x^n` |
| `f.asin(n)` | `asin(f) mod x^n` |
| `f.atan(n)` | `atan(f) mod x^n` |

### 模板

```cpp
namespace poly998 {

Poly Poly::deriv() const {
    if (size() <= 1) return Poly();

    vector<int> r(size() - 1);
    for (int i = 1; i < size(); ++i) {
        r[i - 1] = (int)(1LL * a[i] * i % MOD);
    }

    return Poly(move(r));
}

Poly Poly::integr() const {
    if (empty()) return Poly();

    assert(size() < MOD);

    static vector<int> invs{0, 1};
    int n = size();

    if ((int)invs.size() <= n) {
        int old = (int)invs.size();
        invs.resize(n + 1);

        for (int i = max(2, old); i <= n; ++i) {
            invs[i] = MOD - (int)(1LL * (MOD / i) * invs[MOD % i] % MOD);
        }
    }

    vector<int> r(n + 1);
    for (int i = 0; i < n; ++i) {
        r[i + 1] = (int)(1LL * a[i] * invs[i + 1] % MOD);
    }

    return Poly(move(r));
}

Poly Poly::inv(int n) const {
    if (n <= 0) return Poly();

    assert(n < MOD);
    assert((*this)[0] != 0);

    Poly x(invmod((*this)[0]));

    for (int m = 2; m < 2 * n; m <<= 1) {
        int k = min(m, n);
        x = (x * (Poly(2) - mod_xk(k) * x)).mod_xk(k);
    }

    return x.mod_xk(n);
}

Poly Poly::log(int n) const {
    if (n <= 0) return Poly();

    assert(n < MOD);
    assert((*this)[0] == 1);

    Poly f = mod_xk(n);
    return (f.deriv() * f.inv(n)).integr().mod_xk(n);
}

Poly Poly::exp(int n) const {
    if (n <= 0) return Poly();

    assert(n < MOD);
    assert((*this)[0] == 0);

    Poly x(1);

    for (int m = 2; m < 2 * n; m <<= 1) {
        int k = min(m, n);
        x = (x * (mod_xk(k) - x.log(k) + Poly(1))).mod_xk(k);
    }

    return x.mod_xk(n);
}

Poly Poly::pow(long long k, int n) const {
    assert(k >= 0);

    if (n <= 0) return Poly();

    assert(n < MOD);

    if (k == 0) return Poly(1);
    if (empty()) return Poly();

    int t = 0;
    while (t < size() && a[t] == 0) ++t;

    if ((__int128)t * k >= n) return Poly();

    int shift = (int)((__int128)t * k);
    int lead = a[t];
    int need = n - shift;
    int r = min(size(), t + need);

    Poly g(vector<int>(a.begin() + t, a.begin() + r));
    g *= invmod(lead);

    Poly h = (g.log(need) * (k % MOD)).exp(need);
    h *= qpow(lead, k);

    return h.mul_xk(shift).mod_xk(n);
}

optional<Poly> Poly::sqrt_nonzero_constant(int n) const {
    auto s0 = mod_sqrt((*this)[0]);
    if (!s0) return nullopt;

    Poly x(*s0);
    constexpr int INV2 = (MOD + 1) / 2;

    for (int m = 2; m < 2 * n; m <<= 1) {
        int k = min(m, n);
        x = ((x + mod_xk(k) * x.inv(k)) * INV2).mod_xk(k);
    }

    return x.mod_xk(n);
}

optional<Poly> Poly::sqrt(int n) const {
    if (n <= 0 || empty()) return Poly();

    assert(n < MOD);

    int t = 0;
    while (t < size() && a[t] == 0) ++t;

    // f == 0 (mod x^n)
    if (t >= n) return Poly();

    if (t & 1) return nullopt;

    int shift = t / 2;
    int need = n - t;

    Poly g(vector<int>(a.begin() + t, a.begin() + min(size(), n)));

    auto h = g.sqrt_nonzero_constant(need);
    if (!h) return nullopt;

    return h->mul_xk(shift).mod_xk(n);
}

int imag_unit() {
    static int I = qpow(G, (MOD - 1) / 4);
    return I;
}

Poly Poly::sin(int n) const {
    if (n <= 0) return Poly();

    assert((*this)[0] == 0);

    int I = imag_unit();
    Poly fi = mod_xk(n) * I;

    Poly e1 = fi.exp(n);
    Poly e2 = (-fi).exp(n);

    return (e1 - e2) * invmod(2LL * I);
}

Poly Poly::cos(int n) const {
    if (n <= 0) return Poly();

    assert((*this)[0] == 0);

    int I = imag_unit();
    Poly fi = mod_xk(n) * I;

    Poly e1 = fi.exp(n);
    Poly e2 = (-fi).exp(n);

    return (e1 + e2) / 2;
}

Poly Poly::tan(int n) const {
    if (n <= 0) return Poly();

    assert((*this)[0] == 0);

    Poly s = sin(n);
    Poly c = cos(n);

    return (s * c.inv(n)).mod_xk(n);
}

Poly Poly::asin(int n) const {
    if (n <= 0) return Poly();

    assert((*this)[0] == 0);

    Poly f = mod_xk(n);
    auto rt = (Poly(1) - f * f).mod_xk(n).sqrt(n);

    assert(rt.has_value());

    Poly s = *rt;

    // asin 的形式幂级数分支要求 sqrt(1-f^2)[0] = +1
    if (s[0] != 1) s = -s;

    return (f.deriv() * s.inv(n)).integr().mod_xk(n);
}

Poly Poly::atan(int n) const {
    if (n <= 0) return Poly();

    assert((*this)[0] == 0);

    Poly f = mod_xk(n);
    Poly den = (Poly(1) + f * f).mod_xk(n);

    return (f.deriv() * den.inv(n)).integr().mod_xk(n);
}

} // namespace poly998
```

### 现场记忆

```text
inv：g <- g(2 - fg)

log f = ∫ f' / f

exp：g <- g(1 + f - log g)

pow：
f = x^t * c * g, g[0]=1
f^k = x^(tk) * c^k * exp(k log g)

sqrt：
最低非零次数 t >= n -> 直接为 0
否则 t 必须为偶数
g <- (g + f/g) / 2

sin/cos：用 exp(i f)，i^2 = -1
asin' = f' / sqrt(1-f^2)
atan' = f' / (1+f^2)
```

## 8.3 多项式除法 / 多点求值 / 插值

> **用途**：快速求多项式商余、多点求值，以及由任意互异点值恢复多项式。
> **复杂度**：除法 `O(M(n))`；多点求值 / 插值 `O(M(n) log n)`。
> **下标**：`0-indexed`。
> **依赖**：多项式核心、FPS `inv()`。
> **注意**：插值要求 `xs[i]` 在模 `998244353` 意义下两两不同。

### 调用

```cpp
auto [q, r] = f.divmod(g);

vector<int> xs = {1, 2, 3, 4};
vector<int> ys = f.eval(xs);

auto p = Poly::interpolate(xs, ys);
if (p) {
    Poly f2 = *p;
}
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `f.divmod(g)` | `{商, 余数}` |
| `f / g` | 商 |
| `f % g` | 余数 |
| `f.eval(xs)` | 返回所有 `f(xs[i])` |
| `Poly::interpolate(xs,ys)` | 插值，重复横坐标返回 `nullopt` |

### 模板

```cpp
namespace poly998 {

pair<Poly, Poly> Poly::divmod(const Poly &b) const {
    assert(!b.empty());

    if (size() < b.size()) return {Poly(), *this};

    int qn = size() - b.size() + 1;

    Poly q = (rev(size()).mod_xk(qn) * b.rev(b.size()).inv(qn))
                 .mod_xk(qn)
                 .rev(qn);

    Poly r = (*this - b * q).mod_xk(b.size() - 1);

    return {q, r};
}

Poly operator/(const Poly &x, const Poly &y) {
    return x.divmod(y).first;
}

Poly operator%(const Poly &x, const Poly &y) {
    return x.divmod(y).second;
}

vector<int> Poly::eval(const vector<int> &xs) const {
    int n = (int)xs.size();
    if (n == 0) return {};

    int base = 1;
    while (base < n) base <<= 1;

    vector<Poly> prod(base << 1, Poly(1));

    for (int i = 0; i < n; ++i) {
        prod[base + i] = Poly({norm(-1LL * xs[i]), 1});
    }

    for (int i = base - 1; i >= 1; --i) {
        prod[i] = prod[i << 1] * prod[i << 1 | 1];
    }

    vector<Poly> rem(base << 1);
    rem[1] = *this % prod[1];

    for (int i = 1; i < base; ++i) {
        rem[i << 1] = rem[i] % prod[i << 1];
        rem[i << 1 | 1] = rem[i] % prod[i << 1 | 1];
    }

    vector<int> ans(n);

    for (int i = 0; i < n; ++i) {
        ans[i] = rem[base + i][0];
    }

    return ans;
}

optional<Poly> Poly::interpolate(const vector<int> &xs, const vector<int> &ys) {
    int n = (int)xs.size();

    assert((int)ys.size() == n);

    if (n == 0) return Poly();

    int base = 1;
    while (base < n) base <<= 1;

    vector<Poly> prod(base << 1, Poly(1));

    for (int i = 0; i < n; ++i) {
        prod[base + i] = Poly({norm(-1LL * xs[i]), 1});
    }

    for (int i = base - 1; i >= 1; --i) {
        prod[i] = prod[i << 1] * prod[i << 1 | 1];
    }

    vector<int> den = prod[1].deriv().eval(xs);
    vector<Poly> val(base << 1);

    for (int i = 0; i < n; ++i) {
        if (den[i] == 0) return nullopt;
        val[base + i] = Poly(1LL * norm(ys[i]) * invmod(den[i]) % MOD);
    }

    for (int i = base - 1; i >= 1; --i) {
        val[i] = val[i << 1] * prod[i << 1 | 1]
               + val[i << 1 | 1] * prod[i << 1];
    }

    return val[1].mod_xk(n);
}

} // namespace poly998
```

### 现场记忆

```text
除法：
翻转 A、B，把最高次项变成常数项；
Q^R = A^R / B^R，再翻回来。

多点求值：
乘积树存 ∏(x-x_i)
根处 f % prod[root]
向儿子继续取模

插值：
P(x)=∏(x-x_i)
叶子权值 = y_i / P'(x_i)
向上合并：
left * prod[right] + right * prod[left]
```

## 8.4 多项式平移 / Chirp-Z / 连续点值平移

> **用途**：快速求 `f(x+c)`、等比点 `f(1),f(q),...`，以及已知 `f(0..n-1)` 后求另一段连续点值。
> **复杂度**：均为 `O(M(n+m))` 量级。
> **下标**：`0-indexed`。
> **依赖**：多项式核心。
> **注意**：`consecutive_value_shift(y,c,m)` 中 `y[i]=f(i)`，返回 `f(c),...,f(c+m-1)`。

### 调用

```cpp
Poly g = f.shift(c);

vector<int> a = chirp_z(f, q, m);

vector<int> z = consecutive_value_shift(y, c, m);
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `f.shift(c)` | `f(x+c)` |
| `chirp_z(f,q,m)` | `f(1), f(q), ..., f(q^(m-1))` |
| `consecutive_value_shift(y,c,m)` | 已知 `f(0..n-1)`，求 `f(c..c+m-1)` |

### 模板

```cpp
namespace poly998 {

Poly Poly::shift(long long c) const {
    int n = size();

    if (n <= 1) return *this;

    assert(n < MOD);

    ensure_fact(n - 1);
    c = norm(c);

    vector<int> p(n), q(n);

    long long pw = 1;

    for (int i = 0; i < n; ++i) {
        p[n - 1 - i] = (int)(1LL * a[i] * fac[i] % MOD);
        q[i] = (int)(pw * ifac[i] % MOD);
        pw = pw * c % MOD;
    }

    vector<int> r = convolution(p, q);
    vector<int> ans(n);

    for (int i = 0; i < n; ++i) {
        ans[i] = (int)(1LL * r[n - 1 - i] * ifac[i] % MOD);
    }

    return Poly(move(ans));
}

vector<int> chirp_z(const Poly &f, int q, int m) {
    if (m <= 0) return {};

    vector<int> ans(m);

    if (f.empty()) return ans;

    q = norm(q);

    if (q == 0) {
        int sum = 0;

        for (int x : f.a) {
            sum += x;
            if (sum >= MOD) sum -= MOD;
        }

        ans[0] = sum;

        for (int i = 1; i < m; ++i) {
            ans[i] = f[0];
        }

        return ans;
    }

    int n = f.size();
    int lim = n + m - 1;
    int iq = invmod(q);

    vector<int> pos(lim);
    vector<int> neg(max(n, m));

    long long cur = 1, pw = 1;

    // pos[i] = q^(i(i-1)/2)
    for (int i = 0; i < lim; ++i) {
        pos[i] = (int)cur;
        cur = cur * pw % MOD;
        pw = pw * q % MOD;
    }

    cur = 1;
    pw = 1;

    // neg[i] = q^(-i(i-1)/2)
    for (int i = 0; i < (int)neg.size(); ++i) {
        neg[i] = (int)cur;
        cur = cur * pw % MOD;
        pw = pw * iq % MOD;
    }

    vector<int> a(n);

    for (int i = 0; i < n; ++i) {
        a[n - 1 - i] = (int)(1LL * f.a[i] * neg[i] % MOD);
    }

    vector<int> c = convolution(a, pos);

    for (int k = 0; k < m; ++k) {
        ans[k] = (int)(1LL * c[n - 1 + k] * neg[k] % MOD);
    }

    return ans;
}

vector<int> consecutive_value_shift(const vector<int> &y, long long c, int m) {
    int n = (int)y.size();

    if (m <= 0) return {};
    if (n == 0) return vector<int>(m);

    assert(n < MOD);

    ensure_fact(n - 1);

    int len = n + m - 1;
    vector<int> vals(len);

    for (int p = 0; p < len; ++p) {
        vals[p] = norm(c + p - (n - 1LL));
    }

    vector<int> invs = batch_inv(vals);
    vector<int> a(n);

    for (int i = 0; i < n; ++i) {
        long long v = 1LL * norm(y[i]) * ifac[i] % MOD * ifac[n - 1 - i] % MOD;

        if ((n - 1 - i) & 1) {
            v = v ? MOD - v : 0;
        }

        a[i] = (int)v;
    }

    vector<int> conv = convolution(a, invs);

    int zeros = 0;
    long long prod = 1;

    for (int p = 0; p < n; ++p) {
        if (vals[p] == 0) {
            ++zeros;
        } else {
            prod = prod * vals[p] % MOD;
        }
    }

    vector<int> ans(m);

    for (int j = 0; j < m; ++j) {
        int x = norm(c + j);

        if (x < n) {
            ans[j] = norm(y[x]);
        } else {
            assert(zeros == 0);
            ans[j] = (int)(prod * conv[j + n - 1] % MOD);
        }

        if (j + 1 == m) break;

        int out = j;
        int in = j + n;

        if (vals[out] == 0) {
            --zeros;
        } else {
            prod = prod * invs[out] % MOD;
        }

        if (vals[in] == 0) {
            ++zeros;
        } else {
            prod = prod * vals[in] % MOD;
        }
    }

    return ans;
}

} // namespace poly998
```

### 现场记忆

```text
Taylor shift：
A_i = a_i * i! 后翻转
B_i = c^i / i!
卷积后再除 i!

Chirp-Z：
ki = C(k+i,2) - C(k,2) - C(i,2)
转成一次卷积。

连续点值平移：
Lagrange 权值 (-1)^(n-1-i) / i! / (n-1-i)!
1/(c+j-i) 转卷积。
目标正好落在 0..n-1 时直接返回已知值。
```

## 8.5 位运算卷积 - FWT XOR / OR / AND

> **用途**：求下标通过 `xor / or / and` 合并得到的卷积。
> **复杂度**：时间 `O(N log N)`，空间 `O(N)`，`N` 为补齐后的 2 的幂。
> **下标**：mask `0-indexed`。
> **依赖**：多项式核心中的 `MOD / norm / invmod`。
> **注意**：返回长度为 `>= max(|a|,|b|)` 的最小 2 的幂。

### 调用

```cpp
auto cx = xor_convolution(a, b);
auto co = or_convolution(a, b);
auto ca = and_convolution(a, b);
```

### 接口速查

| 接口 | 计算 |
|---|---|
| `xor_convolution(a,b)` | `c[k] = sum_{i^j=k} a[i]b[j]` |
| `or_convolution(a,b)` | `c[k] = sum_{i\|j=k} a[i]b[j]` |
| `and_convolution(a,b)` | `c[k] = sum_{i&j=k} a[i]b[j]` |

### 模板

```cpp
namespace poly998 {

vector<int> xor_convolution(vector<int> a, vector<int> b) {
    if (a.empty() || b.empty()) return {};

    int n = 1;
    while (n < (int)max(a.size(), b.size())) n <<= 1;

    a.resize(n);
    b.resize(n);

    for (int &x : a) x = norm(x);
    for (int &x : b) x = norm(x);

    auto fwt = [&](vector<int> &v) {
        for (int len = 1; len < n; len <<= 1) {
            for (int i = 0; i < n; i += len << 1) {
                for (int j = 0; j < len; ++j) {
                    int x = v[i + j];
                    int y = v[i + j + len];

                    int p = x + y;
                    if (p >= MOD) p -= MOD;

                    int q = x - y;
                    if (q < 0) q += MOD;

                    v[i + j] = p;
                    v[i + j + len] = q;
                }
            }
        }
    };

    fwt(a);
    fwt(b);

    for (int i = 0; i < n; ++i) {
        a[i] = (int)(1LL * a[i] * b[i] % MOD);
    }

    fwt(a);

    int inv_n = invmod(n);

    for (int &x : a) {
        x = (int)(1LL * x * inv_n % MOD);
    }

    return a;
}

vector<int> or_convolution(vector<int> a, vector<int> b) {
    if (a.empty() || b.empty()) return {};

    int n = 1;
    while (n < (int)max(a.size(), b.size())) n <<= 1;

    a.resize(n);
    b.resize(n);

    for (int &x : a) x = norm(x);
    for (int &x : b) x = norm(x);

    auto fwt = [&](vector<int> &v, bool invert) {
        for (int bit = 1; bit < n; bit <<= 1) {
            for (int mask = 0; mask < n; ++mask) {
                if (mask & bit) {
                    int &x = v[mask];
                    int y = v[mask ^ bit];

                    x = invert ? x - y : x + y;

                    if (x < 0) x += MOD;
                    if (x >= MOD) x -= MOD;
                }
            }
        }
    };

    fwt(a, false);
    fwt(b, false);

    for (int i = 0; i < n; ++i) {
        a[i] = (int)(1LL * a[i] * b[i] % MOD);
    }

    fwt(a, true);

    return a;
}

vector<int> and_convolution(vector<int> a, vector<int> b) {
    if (a.empty() || b.empty()) return {};

    int n = 1;
    while (n < (int)max(a.size(), b.size())) n <<= 1;

    a.resize(n);
    b.resize(n);

    for (int &x : a) x = norm(x);
    for (int &x : b) x = norm(x);

    auto fwt = [&](vector<int> &v, bool invert) {
        for (int bit = 1; bit < n; bit <<= 1) {
            for (int mask = 0; mask < n; ++mask) {
                if ((mask & bit) == 0) {
                    int &x = v[mask];
                    int y = v[mask | bit];

                    x = invert ? x - y : x + y;

                    if (x < 0) x += MOD;
                    if (x >= MOD) x -= MOD;
                }
            }
        }
    };

    fwt(a, false);
    fwt(b, false);

    for (int i = 0; i < n; ++i) {
        a[i] = (int)(1LL * a[i] * b[i] % MOD);
    }

    fwt(a, true);

    return a;
}

} // namespace poly998
```

### 现场记忆

```text
XOR：
(x,y) -> (x+y,x-y)
逆变换再做一次，最后整体 / N

OR：
有 bit 的位置 += 没 bit 的位置
逆变换改成 -=

AND：
没 bit 的位置 += 有 bit 的位置
逆变换改成 -=
```

## 8.6 线性递推 - Bostan-Mori / Berlekamp-Massey

> **用途**：求常系数线性递推的超远项；由序列前缀恢复最短线性递推；求有理生成函数 `[x^n]P/Q`。
> **复杂度**：Bostan-Mori `O(M(d) log n)`；BM `O(L^2)`。
> **下标**：序列从 `a[0]` 开始。
> **依赖**：多项式核心、FPS、多项式除法。
> **注意**：递推系数约定为 `a[n] = coef[0]a[n-1] + ... + coef[d-1]a[n-d]`。

### 调用

```cpp
vector<int> init = {0, 1};
vector<int> coef = {1, 1};

int fib = linear_recurrence_nth(init, coef, 1000000000000LL);

vector<int> rec = berlekamp_massey(prefix);
int ans = linear_recurrence_nth(prefix, rec, k);

int c = bostan_mori(P, Q, n);
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `bostan_mori(P,Q,n)` | `[x^n] P(x)/Q(x)` |
| `linear_recurrence_nth(init,coef,k)` | 求递推第 `k` 项 |
| `berlekamp_massey(prefix)` | 猜最短线性递推 |

### 模板

```cpp
namespace poly998 {

int bostan_mori(Poly P, Poly Q, long long n) {
    assert(n >= 0);
    assert(Q[0] != 0);

    int extra = 0;

    if (P.size() >= Q.size()) {
        auto [quo, rem] = P.divmod(Q);

        if (n <= quo.deg()) {
            extra = quo[(int)n];
        }

        P = move(rem);
    }

    while (n > 0 && !P.empty()) {
        vector<int> qneg = Q.a;

        for (int i = 1; i < (int)qneg.size(); i += 2) {
            if (qneg[i]) qneg[i] = MOD - qneg[i];
        }

        Poly R = P * Poly(qneg);
        Poly S = Q * Poly(move(qneg));

        vector<int> np, nq;

        for (int i = (int)(n & 1); i < R.size(); i += 2) {
            np.push_back(R.a[i]);
        }

        for (int i = 0; i < S.size(); i += 2) {
            nq.push_back(S.a[i]);
        }

        P = Poly(move(np));
        Q = Poly(move(nq));

        n >>= 1;
    }

    int ans = P.empty() ? 0 : (int)(1LL * P[0] * invmod(Q[0]) % MOD);

    ans += extra;
    if (ans >= MOD) ans -= MOD;

    return ans;
}

int linear_recurrence_nth(const vector<int> &init, const vector<int> &coef, long long k) {
    assert(k >= 0);

    int d = (int)coef.size();

    if (k < (long long)init.size()) {
        return norm(init[(int)k]);
    }

    if (d == 0) return 0;

    assert((int)init.size() >= d);

    vector<int> q(d + 1);

    q[0] = 1;

    for (int i = 1; i <= d; ++i) {
        q[i] = norm(-1LL * coef[i - 1]);
    }

    vector<int> s(d);

    for (int i = 0; i < d; ++i) {
        s[i] = norm(init[i]);
    }

    Poly Q(move(q));
    Poly P = (Poly(move(s)) * Q).mod_xk(d);

    return bostan_mori(P, Q, k);
}

vector<int> berlekamp_massey(const vector<int> &seq) {
    vector<int> C{1};
    vector<int> B{1};

    int L = 0;
    int m = 1;
    int b = 1;

    for (int n = 0; n < (int)seq.size(); ++n) {
        long long d = norm(seq[n]);

        for (int i = 1; i <= L; ++i) {
            d = (d + 1LL * C[i] * norm(seq[n - i])) % MOD;
        }

        if (d == 0) {
            ++m;
            continue;
        }

        vector<int> T = C;
        int coef = (int)(d * invmod(b) % MOD);

        if ((int)C.size() < (int)B.size() + m) {
            C.resize(B.size() + m);
        }

        for (int i = 0; i < (int)B.size(); ++i) {
            C[i + m] = norm(C[i + m] - 1LL * coef * B[i]);
        }

        if (2 * L <= n) {
            L = n + 1 - L;
            B = move(T);
            b = (int)d;
            m = 1;
        } else {
            ++m;
        }
    }

    vector<int> coef(L);

    for (int i = 1; i <= L; ++i) {
        coef[i - 1] = norm(-C[i]);
    }

    return coef;
}

} // namespace poly998
```

### 现场记忆

```text
递推：
a[n] = c1*a[n-1] + ... + cd*a[n-d]
Q(x) = 1 - c1*x - ... - cd*x^d
P = (A * Q) mod x^d

Bostan-Mori：
Q-(x) = Q(-x)
R = P(x)Q(-x)
S = Q(x)Q(-x)

n 奇 -> P 取 R 奇数项
n 偶 -> P 取 R 偶数项
Q 始终取 S 偶数项
n >>= 1

BM 返回的 coef 与 linear_recurrence_nth 的方向一致。
```

## 8.7 多项式复合 / 拉格朗日反演

> **用途**：小规模形式幂级数复合、复合逆，以及用 Lagrange Inversion 求单个系数。
> **复杂度**：`compose_slow` 约 `O(n M(n))`；复合逆同量级；拉格朗日单项系数主要为 FPS `pow/inv`。
> **下标**：`0-indexed`。
> **依赖**：多项式核心、FPS。
> **限制**：`compose_slow` 要求 `g[0]=0`；复合逆要求 `f[0]=0, f[1]!=0`。

### 调用

```cpp
Poly h = f.compose_slow(g, n);

Poly invf = f.compositional_inverse_slow(n);

int c1 = lagrange_y_power_coeff(phi, n, k);
int c2 = lagrange_inverse_coeff(f, n);
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `f.compose_slow(g,n)` | `f(g(x)) mod x^n` |
| `f.compositional_inverse_slow(n)` | 求 `g` 使 `f(g(x))=x mod x^n` |
| `lagrange_y_power_coeff(phi,n,k)` | `y=x*phi(y)` 时 `[x^n]y^k` |
| `lagrange_inverse_coeff(f,n)` | `[x^n] f^{-1}(x)`，这里是复合逆 |

### 模板

```cpp
namespace poly998 {

int lagrange_y_power_coeff(const Poly &phi, int n, int k) {
    assert(n >= 0);
    assert(k >= 0);

    if (k == 0) return n == 0;
    if (n == 0 || k > n) return 0;

    assert(n < MOD);
    assert(phi[0] != 0);

    int need = n - k + 1;
    Poly p = phi.pow(n, need);

    return (int)(1LL * k * invmod(n) % MOD * p[n - k] % MOD);
}

int lagrange_inverse_coeff(const Poly &f, int n) {
    assert(n >= 0);

    if (n == 0) return 0;

    assert(n < MOD);
    assert(f[0] == 0);
    assert(f[1] != 0);

    Poly h = f.div_xk(1).mod_xk(n);
    Poly phi = h.inv(n);
    Poly p = phi.pow(n, n);

    return (int)(1LL * p[n - 1] * invmod(n) % MOD);
}

Poly Poly::compose_slow(const Poly &g, int n) const {
    if (n <= 0) return Poly();

    assert(g[0] == 0);

    Poly h = g.mod_xk(n);
    Poly r;

    int lim = min(size(), n);

    for (int i = lim - 1; i >= 0; --i) {
        r = (r * h + Poly(a[i])).mod_xk(n);
    }

    return r;
}

Poly Poly::compositional_inverse_slow(int n) const {
    if (n <= 0) return Poly();

    assert((*this)[0] == 0);
    assert((*this)[1] != 0);

    if (n == 1) return Poly();

    Poly g({0, invmod((*this)[1])});
    Poly X({0, 1});

    for (int m = 2; m < n; m <<= 1) {
        int k = min(m << 1, n);

        Poly fg = mod_xk(k).compose_slow(g, k);
        Poly df = deriv().mod_xk(k).compose_slow(g, k);

        g = (g - (fg - X) * df.inv(k)).mod_xk(k);
    }

    return g.mod_xk(n);
}

} // namespace poly998
```

### 现场记忆

```text
y = x * phi(y)：

[x^n] y^k
= k/n * [t^(n-k)] phi(t)^n

若 g = f^{-1}（复合逆）：
f(t) = t * h(t)
phi(t) = 1 / h(t)

[x^n] g
= 1/n * [t^(n-1)] (t/f(t))^n

复合逆 Newton：
g <- g - (f(g)-x) / f'(g)
```

## 8.8 生成函数 OGF / EGF

> **用途**：把序列系数转为普通生成函数或指数生成函数。
> **复杂度**：时间 `O(n)`，空间 `O(n)`。
> **下标**：`0-indexed`。
> **依赖**：多项式核心。

### 模板

```cpp
namespace poly998 {

Poly ogf_from_sequence(const vector<int> &seq) {
    return Poly(seq);
}

Poly egf_from_sequence(const vector<int> &seq) {
    int n = (int)seq.size();

    if (n == 0) return Poly();

    assert(n <= MOD);

    ensure_fact(n - 1);

    vector<int> a(n);

    for (int i = 0; i < n; ++i) {
        a[i] = (int)(1LL * norm(seq[i]) * ifac[i] % MOD);
    }

    return Poly(move(a));
}

} // namespace poly998
```

### 现场记忆

```text
OGF：
F(x) = sum a[n] x^n
Poly 系数直接等于 a[n]

EGF：
F(x) = sum a[n] x^n / n!
Poly 系数等于 a[n] / n!
```

## 8.9 多项式多点求值

> **用途**：给定多项式 `f(x)` 和 `x[0..n-1]`，快速计算全部 `f(x[i])`。
> **复杂度**：`O(n log² n)`，空间 `O(n log n)`
> **下标**：系数、点均 `0-indexed`
> **限制**：模数固定为 `998244353`。

### 调用

```cpp
Poly f = {1, 2, 3};
vector<int> x = {1, 2, 3};

Multipoint tree(x);
auto y = tree.eval(f);
```

### 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int G = 3;

using ll = long long;
using Poly = vector<int>;

int modPow(int a, int e) {
    ll res = 1;

    while (e) {
        if (e & 1) res = res * a % MOD;
        a = (ll)a * a % MOD;
        e >>= 1;
    }

    return res;
}

void ntt(Poly& a, bool inv) {
    int n = a.size();

    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;

        for (; j & bit; bit >>= 1) j ^= bit;

        j ^= bit;

        if (i < j) swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        int wlen = modPow(G, (MOD - 1) / len);

        if (inv) wlen = modPow(wlen, MOD - 2);

        for (int i = 0; i < n; i += len) {
            ll w = 1;

            for (int j = 0; j < len / 2; ++j) {
                int u = a[i + j];
                int v = w * a[i + j + len / 2] % MOD;

                a[i + j] = u + v;
                if (a[i + j] >= MOD) a[i + j] -= MOD;

                a[i + j + len / 2] = u - v;
                if (a[i + j + len / 2] < 0) a[i + j + len / 2] += MOD;

                w = w * wlen % MOD;
            }
        }
    }

    if (inv) {
        int invN = modPow(n, MOD - 2);

        for (int& x : a) x = (ll)x * invN % MOD;
    }
}

void norm(Poly& a) {
    while (!a.empty() && a.back() == 0) a.pop_back();
}

Poly mul(Poly a, Poly b) {
    if (a.empty() || b.empty()) return {};

    if ((ll)a.size() * b.size() <= 256) {
        Poly c(a.size() + b.size() - 1);

        for (int i = 0; i < (int)a.size(); ++i)
            for (int j = 0; j < (int)b.size(); ++j)
                c[i + j] = (c[i + j] + (ll)a[i] * b[j]) % MOD;

        return c;
    }

    int need = a.size() + b.size() - 1;
    int n = 1;

    while (n < need) n <<= 1;

    a.resize(n);
    b.resize(n);

    ntt(a, false);
    ntt(b, false);

    for (int i = 0; i < n; ++i) a[i] = (ll)a[i] * b[i] % MOD;

    ntt(a, true);
    a.resize(need);

    return a;
}

Poly polyInv(const Poly& a, int n) {
    Poly res(1, modPow(a[0], MOD - 2));

    for (int len = 2; len < 2 * n; len <<= 1) {
        int m = min(len, n);

        Poly f(a.begin(), a.begin() + min((int)a.size(), m));
        Poly t = mul(f, res);

        t.resize(m);

        for (int& x : t) x = x ? MOD - x : 0;

        t[0] += 2;
        if (t[0] >= MOD) t[0] -= MOD;

        res = mul(res, t);
        res.resize(m);
    }

    res.resize(n);
    return res;
}

Poly polyMod(Poly a, const Poly& b) {
    norm(a);

    if (a.size() < b.size()) return a;

    int k = a.size() - b.size() + 1;

    Poly ra = a, rb = b;

    reverse(ra.begin(), ra.end());
    reverse(rb.begin(), rb.end());

    ra.resize(k);
    rb.resize(k);

    Poly q = mul(ra, polyInv(rb, k));
    q.resize(k);

    reverse(q.begin(), q.end());

    Poly qb = mul(q, b);

    for (int i = 0; i < (int)qb.size(); ++i) {
        a[i] -= qb[i];
        if (a[i] < 0) a[i] += MOD;
    }

    a.resize(b.size() - 1);
    norm(a);

    return a;
}

struct Multipoint {
    int n;
    vector<int> x;
    vector<Poly> prod;

    Multipoint(vector<int> x_) : n(x_.size()), x(move(x_)), prod(max(1, 4 * n)) {
        for (int& v : x) {
            v %= MOD;
            if (v < 0) v += MOD;
        }

        if (n) build(1, 0, n);
    }

    void build(int p, int l, int r) {
        if (r - l == 1) {
            prod[p] = {(MOD - x[l]) % MOD, 1};
            return;
        }

        int mid = (l + r) >> 1;

        build(p << 1, l, mid);
        build(p << 1 | 1, mid, r);

        prod[p] = mul(prod[p << 1], prod[p << 1 | 1]);
    }

    void dfs(int p, int l, int r, const Poly& f, vector<int>& ans) {
        if (r - l == 1) {
            ans[l] = f.empty() ? 0 : f[0];
            return;
        }

        int mid = (l + r) >> 1;

        Poly lf = polyMod(f, prod[p << 1]);
        Poly rf = polyMod(f, prod[p << 1 | 1]);

        dfs(p << 1, l, mid, lf, ans);
        dfs(p << 1 | 1, mid, r, rf, ans);
    }

    vector<int> eval(const Poly& f) {
        if (!n) return {};

        vector<int> ans(n);
        Poly r = polyMod(f, prod[1]);

        dfs(1, 0, n, r, ans);

        return ans;
    }
};
```

### 现场记忆

```text
建乘积树：
P_node(x) = ∏(x-x_i)

根上：
f %= P_root

向下：
左儿子拿 f % P_left
右儿子拿 f % P_right

到叶子时余式就是常数 f(x_i)。

多项式取模：
反转 -> 求逆 -> 做多项式除法。
```

## 8.10 多项式快速插值

> **用途**：给定 `n` 个互异点 `(x_i,y_i)`，在 `998244353` 下快速恢复次数 `< n` 的多项式。
> **复杂度**：`O(n log² n)`，空间 `O(n log n)`
> **下标**：系数 `0-indexed`
> **限制**：所有 `x_i` 在模 `998244353` 意义下两两不同。

### 调用

```cpp
vector<int> x(n), y(n);

Poly f = fastInterpolation(x, y);

for (int c : f) cout << c << ' ';
```

### 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int G = 3;

using ll = long long;
using Poly = vector<int>;

int modPow(int a, int e) {
    ll res = 1;

    while (e) {
        if (e & 1) res = res * a % MOD;
        a = (ll)a * a % MOD;
        e >>= 1;
    }

    return res;
}

void ntt(Poly& a, bool inv) {
    int n = a.size();

    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;

        for (; j & bit; bit >>= 1) j ^= bit;

        j ^= bit;

        if (i < j) swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        int wlen = modPow(G, (MOD - 1) / len);

        if (inv) wlen = modPow(wlen, MOD - 2);

        for (int i = 0; i < n; i += len) {
            ll w = 1;

            for (int j = 0; j < len / 2; ++j) {
                int u = a[i + j];
                int v = w * a[i + j + len / 2] % MOD;

                a[i + j] = u + v;
                if (a[i + j] >= MOD) a[i + j] -= MOD;

                a[i + j + len / 2] = u - v;
                if (a[i + j + len / 2] < 0) a[i + j + len / 2] += MOD;

                w = w * wlen % MOD;
            }
        }
    }

    if (inv) {
        int invN = modPow(n, MOD - 2);

        for (int& x : a) x = (ll)x * invN % MOD;
    }
}

void norm(Poly& a) {
    while (!a.empty() && a.back() == 0) a.pop_back();
}

Poly addPoly(Poly a, const Poly& b) {
    if (a.size() < b.size()) a.resize(b.size());

    for (int i = 0; i < (int)b.size(); ++i) {
        a[i] += b[i];
        if (a[i] >= MOD) a[i] -= MOD;
    }

    norm(a);
    return a;
}

Poly mul(Poly a, Poly b) {
    if (a.empty() || b.empty()) return {};

    if ((ll)a.size() * b.size() <= 256) {
        Poly c(a.size() + b.size() - 1);

        for (int i = 0; i < (int)a.size(); ++i)
            for (int j = 0; j < (int)b.size(); ++j)
                c[i + j] = (c[i + j] + (ll)a[i] * b[j]) % MOD;

        return c;
    }

    int need = a.size() + b.size() - 1;
    int n = 1;

    while (n < need) n <<= 1;

    a.resize(n);
    b.resize(n);

    ntt(a, false);
    ntt(b, false);

    for (int i = 0; i < n; ++i) a[i] = (ll)a[i] * b[i] % MOD;

    ntt(a, true);
    a.resize(need);

    return a;
}

Poly polyInv(const Poly& a, int n) {
    Poly res(1, modPow(a[0], MOD - 2));

    for (int len = 2; len < 2 * n; len <<= 1) {
        int m = min(len, n);

        Poly f(a.begin(), a.begin() + min((int)a.size(), m));
        Poly t = mul(f, res);

        t.resize(m);

        for (int& x : t) x = x ? MOD - x : 0;

        t[0] += 2;
        if (t[0] >= MOD) t[0] -= MOD;

        res = mul(res, t);
        res.resize(m);
    }

    res.resize(n);
    return res;
}

Poly polyMod(Poly a, const Poly& b) {
    norm(a);

    if (a.size() < b.size()) return a;

    int k = a.size() - b.size() + 1;

    Poly ra = a, rb = b;

    reverse(ra.begin(), ra.end());
    reverse(rb.begin(), rb.end());

    ra.resize(k);
    rb.resize(k);

    Poly q = mul(ra, polyInv(rb, k));
    q.resize(k);

    reverse(q.begin(), q.end());

    Poly qb = mul(q, b);

    for (int i = 0; i < (int)qb.size(); ++i) {
        a[i] -= qb[i];
        if (a[i] < 0) a[i] += MOD;
    }

    a.resize(b.size() - 1);
    norm(a);

    return a;
}

struct ProductTree {
    int n;
    vector<int> x;
    vector<Poly> prod;

    ProductTree(vector<int> x_) : n(x_.size()), x(move(x_)), prod(max(1, 4 * n)) {
        for (int& v : x) {
            v %= MOD;
            if (v < 0) v += MOD;
        }

        if (n) build(1, 0, n);
    }

    void build(int p, int l, int r) {
        if (r - l == 1) {
            prod[p] = {(MOD - x[l]) % MOD, 1};
            return;
        }

        int mid = (l + r) >> 1;

        build(p << 1, l, mid);
        build(p << 1 | 1, mid, r);

        prod[p] = mul(prod[p << 1], prod[p << 1 | 1]);
    }

    void evalDfs(int p, int l, int r, const Poly& f, vector<int>& ans) {
        if (r - l == 1) {
            ans[l] = f.empty() ? 0 : f[0];
            return;
        }

        int mid = (l + r) >> 1;

        Poly lf = polyMod(f, prod[p << 1]);
        Poly rf = polyMod(f, prod[p << 1 | 1]);

        evalDfs(p << 1, l, mid, lf, ans);
        evalDfs(p << 1 | 1, mid, r, rf, ans);
    }

    vector<int> eval(const Poly& f) {
        vector<int> ans(n);

        if (!n) return ans;

        Poly r = polyMod(f, prod[1]);
        evalDfs(1, 0, n, r, ans);

        return ans;
    }

    Poly interpolateDfs(int p, int l, int r, const vector<int>& w) {
        if (r - l == 1) return {w[l]};

        int mid = (l + r) >> 1;

        Poly L = interpolateDfs(p << 1, l, mid, w);
        Poly R = interpolateDfs(p << 1 | 1, mid, r, w);

        Poly a = mul(L, prod[p << 1 | 1]);
        Poly b = mul(R, prod[p << 1]);

        return addPoly(a, b);
    }
};

Poly fastInterpolation(vector<int> x, const vector<int>& y) {
    int n = x.size();
    if (!n) return {};

    ProductTree tree(x);

    Poly P = tree.prod[1];
    Poly dP(max(0, (int)P.size() - 1));

    for (int i = 1; i < (int)P.size(); ++i) dP[i - 1] = (ll)i * P[i] % MOD;

    vector<int> d = tree.eval(dP);
    vector<int> w(n);

    for (int i = 0; i < n; ++i) w[i] = (ll)y[i] * modPow(d[i], MOD - 2) % MOD;

    Poly ans = tree.interpolateDfs(1, 0, n, w);

    ans.resize(n);
    return ans;
}
```

### 现场记忆

```text
先令：
P(x) = ∏(x-x_i)

Lagrange：
f(x) = Σ y_i / P'(x_i) * P(x)/(x-x_i)

步骤：
1. 乘积树求 P
2. 求 P'
3. 多点求值 P'(x_i)
4. w_i = y_i / P'(x_i)
5. 分治合并 w_i * 其它因子

核心和多点求值共用同一棵乘积树。
```

## 8.11 拉格朗日插值

> **用途**：给定若干互异点 `(x_i,y_i)`，多次查询次数 `< n` 的插值多项式在任意 `x` 处的值。
> **复杂度**：预处理 `O(n²)`，单次查询 `O(n)`，空间 `O(n)`
> **下标**：点 `0-indexed`
> **限制**：模数 `MOD` 为质数，所有 `x_i` 模 `MOD` 两两不同。

### 调用

```cpp
Lagrange lag(x, y);

cout << lag.query(k) << '\n';
```

### 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const int MOD = 998244353;

int modPow(int a, int e) {
    ll res = 1;

    while (e) {
        if (e & 1) res = res * a % MOD;
        a = (ll)a * a % MOD;
        e >>= 1;
    }

    return res;
}

struct Lagrange {
    int n;
    vector<int> x, y, w;

    Lagrange(vector<int> x_, vector<int> y_) : n(x_.size()), x(move(x_)), y(move(y_)), w(n, 1) {
        for (int& v : x) {
            v %= MOD;
            if (v < 0) v += MOD;
        }

        for (int& v : y) {
            v %= MOD;
            if (v < 0) v += MOD;
        }

        for (int i = 0; i < n; ++i) {
            ll den = 1;

            for (int j = 0; j < n; ++j) {
                if (i == j) continue;

                int d = x[i] - x[j];
                if (d < 0) d += MOD;

                den = den * d % MOD;
            }

            w[i] = modPow(den, MOD - 2);
        }
    }

    int query(ll X) const {
        int z = X % MOD;
        if (z < 0) z += MOD;

        for (int i = 0; i < n; ++i)
            if (z == x[i])
                return y[i];

        ll prod = 1;

        for (int i = 0; i < n; ++i) {
            int d = z - x[i];
            if (d < 0) d += MOD;

            prod = prod * d % MOD;
        }

        ll ans = 0;

        for (int i = 0; i < n; ++i) {
            int d = z - x[i];
            if (d < 0) d += MOD;

            ll cur = (ll)y[i] * w[i] % MOD;
            cur = cur * modPow(d, MOD - 2) % MOD;

            ans += cur;
            if (ans >= MOD) ans -= MOD;
        }

        return prod * ans % MOD;
    }
};
```

### 现场记忆

```text
重心形式：

w_i = 1 / ∏_{j!=i}(x_i-x_j)

f(x) =
∏(x-x_j) *
Σ y_i*w_i/(x-x_i)

查询点恰好等于某个 x_i 时，直接返回 y_i，不能除 0。
```
