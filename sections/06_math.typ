= 数学 <chapter-math>

#text("完整保留 raw/06_math.md 的 18 个原始条目及其重要变体，按原顺序排列。原稿附项按当前位置参与算法自动编号。")

#text("统一约定：输入规模、节点号、版本号和查询区间须合法；空区间仅在接口明确支持时使用。递归实现需评估栈空间；整数中间值及累计值必须在对应类型范围内。多测重新调用 init / build / clear，普通函数的外部维护状态由调用者清空。各模板独立选用，同名 Info / Tag / Seg / Query 不应同时粘贴。")

#import "../styles/source.typ": source_code


=== 公共代码与复制顺序

本章源码块仅展示算法自身实现；公共骨架只打印一次。按依赖链接复制所需的公共代码，再复制本算法与调用示例，比赛时不需要仓库头文件。已有基础算法被复用时，直接复制其原条目，不在每个代码块里重贴。

*依赖索引：* #ref(<algo-math-modint>, supplement: none)、#link(<shared-math-modint>)[Mint 模整数骨架]、#ref(<algo-math-miller-rabin>, supplement: none)。

=== 公共骨架：Mint 模整数骨架 <shared-math-modint>

完整代码只在此处展示一次；用途、接口和调用仍见 #ref(<algo-math-modint>, supplement: none)。

#source_code(("math/modint.hpp",))

== #text("ModInt / 快速幂 / 逆元 / 扩展欧几里得") <algo-math-modint>

#text("原稿来源：6.1 ModInt / 快速幂 / 逆元 / 扩展欧几里得")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：固定素数模数优先使用 ")#raw("Mint<MOD>")#text("；动态模数使用 ")#raw("qpow")#text("，一般模数逆元使用扩展欧几里得。")
#strong[#text("复杂度")]#text("：快速幂 / 费马逆元 ")#raw("O(log b)")#text("；扩展欧几里得 ")#raw("O(log min(a,b))")#text("。")
#strong[#text("限制")]#text("：")#raw("Mint::inv()")#text(" 要求 ")#raw("MOD")#text(" 为素数且当前值非 ")#raw("0")#text("；")#raw("inv_mod(a, mod)")#text(" 要求 ")#raw("gcd(a, mod) = 1")#text("。")


=== #text("现场记忆")

#text("固定素数模数：")
#raw("Mint<MOD>")#text(" 直接做 ")#raw("+ - * /")#text("，")#raw("a.inv()")#text(" 即 ")#raw("a^(MOD-2)")#text("。")

#text("动态模数快速幂：")
#raw("res *= a, a *= a")#text("，乘法用 ")#raw("__int128")#text(" 防溢出。")

#text("一般模数逆元：")
#raw("ax + mod*y = 1")#text("，所以 ")#raw("x")#text(" 就是逆元；")#raw("gcd(a, mod) != 1")#text(" 时不存在。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("标量模运算，无数组下标；1<MOD<=INT_MAX，pow 指数非负，inv 需要素数模且非零。qpow 的动态模数必须正且指数非负；exgcd 按非负 a,b 使用；inv_mod 不互质返回 -1。Mint 提供流输入输出，读入值在 long long 内。")

=== 最小调用示例

```cpp
using Z = Mint<101>;
assert((Z(-1) + 2).v == 1 && (Z(8) / 2).v == 4);
```

=== 模板引用

#source_code(("math/modint.hpp",), shared: <shared-math-modint>)

==== #text("qpow")

```cpp
assert(qpow(-2, 5, 101) == 69 && qpow(0, 0, 1) == 0);
```

#source_code(("math/qpow.hpp",))
==== #text("extended_gcd")

```cpp
ll x, y;
ll g = exgcd(30, 18, x, y);
assert(g == 6 && 30 * x + 18 * y == 6);
```

#source_code(("math/extended_gcd.hpp",))
==== #text("inverse_mod")

```cpp
assert(inv_mod(3, 7) == 5 && inv_mod(2, 6) == -1);
```

*代码依赖：* 先复制 #ref(<algo-math-modint>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("math/inverse_mod.hpp",))

== #text("组合数预处理：阶乘 / 逆阶乘") <algo-math-combination>

#text("原稿来源：6.2 组合数预处理：阶乘 / 逆阶乘")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：预处理阶乘和逆阶乘，")#raw("O(1)")#text(" 查询模素数意义下的组合数和排列数。")
#strong[#text("复杂度")]#text("：预处理时间 ")#raw("O(n)")#text("，单次查询 ")#raw("O(1)")#text("，空间 ")#raw("O(n)")#text("。")
#strong[#text("限制")]#text("：")#raw("MOD")#text(" 必须为素数，且预处理范围要求 ")#raw("n < MOD")#text("。")
#strong[#text("依赖")]#text("：")#ref(<algo-math-modint>, supplement: none)#text(" 中的 ")#raw("Mint<MOD>")#text("。")





=== #text("现场记忆")

#raw("C(n,k) = n! / k! / (n-k)!")

#raw("A(n,k) = n! / (n-k)!")

#text("只求一次逆元：")
#raw("ifac[n] = fac[n]^{-1}")#text("，再倒推 ")#raw("ifac[i-1] = ifac[i] * i")#text("。")

#text("必须 ")#raw("n < MOD")#text("；若 ")#raw("n >= MOD")#text("，普通阶乘逆阶乘公式不能直接这样预处理。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("阶乘索引 0..n，0<=n<素数 MOD。默认构造只预处理到 0，支持 C(0,0)=A(0,0)=1；更大 N 先 init(N)。N<0、K<0 或 K>N 返回 0；合法 N 超出表上界时抛 out_of_range，不再静默返回 0。init 拒绝负上界或 n>=MOD；重复 init 重建全表。")

=== 最小调用示例

```cpp
Comb<101> c(30);
assert(c.C(5, 2).v == 10 && c.A(5, 2).v == 20);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-math-modint>)[Mint 模整数骨架]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("math/combination.hpp",))


== #text("Lucas 定理") <algo-math-lucas>

#text("原稿来源：6.3 Lucas 定理")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：当 ")#raw("n,k")#text(" 很大、模数为较小素数 ")#raw("MOD")#text(" 时计算 ")#raw("C(n,k) mod MOD")#text("。")
#strong[#text("复杂度")]#text("：预处理时间 / 空间 ")#raw("O(MOD)")#text("；单次查询 ")#raw("O(log_MOD n)")#text("。")
#strong[#text("限制")]#text("：")#raw("MOD")#text(" 必须为素数，并且必须小到能够预处理 ")#raw("0..MOD-1")#text(" 的阶乘。")
#strong[#text("依赖")]#text("：")#ref(<algo-math-modint>, supplement: none)#text(" 中的 ")#raw("Mint<MOD>")#text("。")
#strong[#text("注意")]#text("：")#raw("MOD = 1e9+7")#text(" 这类大素数不能使用这个 ")#raw("O(MOD)")#text(" 版本。")





=== #text("现场记忆")

#text("把 ")#raw("n,k")#text(" 按 ")#raw("MOD")#text(" 进制拆位：")

#raw("C(n,k) ≡ ∏ C(n_i,k_i) (mod MOD)")

#text("某一位 ")#raw("k_i > n_i")#text("，这一位组合数为 ")#raw("0")#text("，最终答案直接为 ")#raw("0")#text("。")

#text("Lucas 解决的是“")#raw("n")#text(" 很大”，不是“")#raw("MOD")#text(" 很大”；这个版本仍需 ")#raw("O(MOD)")#text(" 预处理。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("fac / ifac 的下标为 0..MOD-1，C(n,k) 是标量查询；MOD 为可预处理的小素数。构造即 init，重复 init 可用。k<0 或 k>n 返回 0。")

=== 最小调用示例

```cpp
Lucas<7> l;
assert(l.C(10, 3).v == 1);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-math-modint>)[Mint 模整数骨架]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("math/lucas.hpp",))


== #text("整除分块") <algo-math-division-blocks>

#text("原稿来源：6.4 整除分块")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：将 ")#raw("floor(n / i)")#text(" 相同的连续下标合并处理，常用于数论求和。")
#strong[#text("复杂度")]#text("：分块数量 ")#raw("O(√n)")#text("，空间 ")#raw("O(1)")#text("。")


=== #text("现场记忆")

#text("当前左端点 ")#raw("l")#text("：")

#raw("q = n / l")

#text("商为 ")#raw("q")#text(" 的最右位置：")

#raw("r = n / q")

#text("然后整段 ")#raw("[l,r]")#text(" 一次处理，下一段从 ")#raw("r+1")#text(" 开始。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#raw("floor_sum(n)")#text(" 是求和 sum(n/i)，与 ")#ref(<algo-math-floor-sum>, supplement: none)#text(" 的四参数 floorSum 不同。n>=0，无数组下标；答案与累计贡献必须在 long long 内，且 n<LLONG_MAX，避免最后 r+1 溢出。")

=== 最小调用示例

```cpp
assert(floor_sum(5) == 10);
```

=== 模板引用

#source_code(("math/division_blocks.hpp",))


== #text("线性筛") <algo-math-linear-sieve>

#text("原稿来源：6.5 线性筛")


#strong[#text("用途")]#text("：")#raw("O(n)")#text(" 预处理素数和每个数的最小质因子。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n)")#text("，空间 ")#raw("O(n)")#text("。")
#strong[#text("下标")]#text("：")#raw("1-indexed")#text("。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口 / 变量")],[#text("含义")]),
[#raw("init(n)")],[#text("筛 ")#raw("1..n")],
[#raw("primes")],[#text("所有素数")],
[#raw("minp[x]")],[#raw("x")#text(" 的最小质因子")],
)])



=== #text("现场记忆")

#raw("minp[i] == 0")#text(" 时，")#raw("i")#text(" 是新素数。")

#text("枚举素数 ")#raw("p")#text(" 筛掉 ")#raw("i*p")#text("：")

#raw("minp[i*p] = p")

#text("当 ")#raw("p == minp[i]")#text(" 时停止，保证每个合数只被最小质因子筛一次。")


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
LinearSieve s(10);
assert((s.primes == vector<int>{2, 3, 5, 7}) && s.minp[9] == 3);
```

=== 模板引用

#source_code(("math/linear_sieve.hpp",))


== #text("欧拉函数 / 莫比乌斯函数：线性筛") <algo-math-multi-sieve>

#text("原稿来源：6.6 欧拉函数 / 莫比乌斯函数：线性筛")


#strong[#text("用途")]#text("：线性预处理 ")#raw("phi")#text("、")#raw("mu")#text("、最小质因子和素数表。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n)")#text("，空间 ")#raw("O(n)")#text("。")
#strong[#text("下标")]#text("：")#raw("1-indexed")#text("。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口 / 变量")],[#text("含义")]),
[#raw("init(n)")],[#text("预处理 ")#raw("1..n")],
[#raw("primes")],[#text("素数表")],
[#raw("phi[x]")],[#text("欧拉函数 ")#raw("φ(x)")],
[#raw("mu[x]")],[#text("莫比乌斯函数 ")#raw("μ(x)")],
[#raw("minp[x]")],[#text("最小质因子")],
)])



=== #text("现场记忆")

#text("若 ")#raw("p | i")#text("：")

#raw("phi[i*p] = phi[i] * p")

#raw("mu[i*p] = 0")

#text("否则：")

#raw("phi[i*p] = phi[i] * (p-1)")

#raw("mu[i*p] = -mu[i]")


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
MultiSieve s(10);
assert(s.phi[10] == 4 && s.mu[6] == 1 && s.mu[4] == 0);
```

=== 模板引用

#source_code(("math/multi_sieve.hpp",))


== 杜教筛：φ 与 μ 前缀和 <algo-math-dujiao>

*用途：* 在线性筛预处理范围外，利用整除同商段与记忆化递归计算 `Σφ(i)` 和 `Σμ(i)`，两者都含 i=1。

*复杂度：* 对一个上界 N，筛到 B 后常用界为 `O(B + N/sqrt(B))`，B 取约 `N^(2/3)` 时为 `O(N^(2/3))`；不是任意小 B 都能达到该界。常用 B 至少在 sqrt(N) 量级，缓存空间约 `O(N/B)`，加上筛表 `O(B)`；任意多上界查询的缓存会继续增长。

*接口与下标：* `DuJiao d(B)` / `init(B)`，B 至少取 1；`sum(n)` 返回 `{φ前缀, μ前缀}`，前者为 `__int128_t`，后者 ll。n=0 返回 `{0,0}`。`init` 重建筛表并清空缓存；同一对象重复查询复用结果。

*推导：* `φ*1=id`，`μ*1=ε`，所以 `Sφ(n)=n(n+1)/2-Σ(l=2..n) Sφ(floor(n/l))`，`Sμ(n)=1-Σ(l=2..n) Sμ(floor(n/l))`；按相同商合并系数。

*注意事项：* n 非负，GNU++17 使用 i128；不能把 φ 前缀强转 ll。大型 n 的 B、时间和缓存容量须按题目评估；递归深度 `O(log n)`，哈希复杂度为期望界。本模板分别测到 n=1e10 的恒等式，不宣称能处理 ll 全范围的规模。

*现场记忆：* 先算卷积的容易前缀，减去 l>=2 的商值；别漏掉 μ(1)=φ(1)=1。

=== 最小调用示例

```cpp
DuJiao d(1000);
auto [p, m] = d.sum(10);
assert(p == 32 && m == -1);
assert(d.sum(0).first == 0);
```

=== 模板引用

*代码依赖：* #ref(<algo-math-multi-sieve>, supplement: none)。先复制 MultiSieve。

#source_code(("math/dujiao.hpp",))

== #text("莫比乌斯反演：互质对数") <algo-math-mobius-pairs>

#text("原稿来源：6.7 莫比乌斯反演：互质对数")


#strong[#text("用途")]#text("：统计 ")#raw("1 <= i <= a, 1 <= j <= b")#text(" 中 ")#raw("gcd(i,j)=1")#text(" 或 ")#raw("gcd(i,j)=k")#text(" 的有序对数。")
#strong[#text("复杂度")]#text("：预处理时间 ")#raw("O(n)")#text("、空间 ")#raw("O(n)")#text("；单次查询约 ")#raw("O(√a + √b)")#text("。")
#strong[#text("下标")]#text("：")#raw("1-indexed")#text("。")
#strong[#text("限制")]#text("：初始化必须满足 ")#raw("n >= min(a,b)")#text("；")#raw("gcd_eq(a,b,k)")#text(" 要求 ")#raw("k >= 1")#text("。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口 / 变量")],[#text("含义")]),
[#raw("init(n)")],[#text("预处理 ")#raw("mu")#text(" 及其前缀和")],
[#raw("mu[x]")],[#text("莫比乌斯函数")],
[#raw("pre[x]")],[#raw("Σ_{i=1}^x mu[i]")],
[#raw("coprime_pairs(a,b)")],[#raw("gcd(i,j)=1")#text(" 的有序对数")],
[#raw("gcd_eq(a,b,k)")],[#raw("gcd(i,j)=k")#text(" 的有序对数")],
)])



=== #text("现场记忆")

#text("互质对数：")

#raw("Σ mu[d] * floor(a/d) * floor(b/d)")

#text("对两个整除项同时分块：")

#raw("r = min(a/(a/l), b/(b/l))")

#text("区间 ")#raw("[l,r]")#text(" 的 ")#raw("mu")#text(" 用前缀和一次取出。")

#raw("gcd(i,j)=k")#text("：先同时除以 ")#raw("k")#text("，转成互质对数。")



=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
Mobius m(10);
assert(m.coprime_pairs(3, 3) == 7 && m.gcd_eq(6, 6, 2) == 7);
```

=== 模板引用

#source_code(("math/mobius_pairs.hpp",))


== #text("矩阵快速幂") <algo-math-matrix-power>

#text("原稿来源：6.8 矩阵快速幂")


#strong[#text("用途")]#text("：构造转移矩阵后快速计算线性递推、路径计数等问题。")
#strong[#text("复杂度")]#text("：")#raw("k × k")#text(" 矩阵快速幂时间 ")#raw("O(k^3 log p)")#text("，空间 ")#raw("O(k^2)")#text("。")
#strong[#text("下标")]#text("：")#raw("0-indexed")#text("。")
#strong[#text("限制")]#text("：指数必须非负。")
#strong[#text("依赖")]#text("：")#ref(<algo-math-modint>, supplement: none)#text(" 中的 ")#raw("Mint<MOD>")#text("。")





=== #text("现场记忆")

#text("矩阵快速幂和普通快速幂完全一样：")

#raw("res = I")

#text("指数当前位为 ")#raw("1")#text("：")#raw("res = res * base")

#text("每轮：")#raw("base = base * base")

#text("矩阵乘法顺序不能反，通常转移写成 ")#raw("state' = A * state")#text("。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
Matrix<101> A(2);
A[0][0] = A[0][1] = A[1][0] = 1;
assert(A.pow(5)[0][0].v == 8);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-math-modint>)[Mint 模整数骨架]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("math/matrix_power.hpp",))


== #text("高斯消元：实数线性方程组") <algo-math-gauss-double>

#text("原稿来源：6.9 高斯消元：实数线性方程组")


#strong[#text("用途")]#text("：求解实数线性方程组，并判断无解、唯一解或多解。")
#strong[#text("复杂度")]#text("：")#raw("n")#text(" 个方程、")#raw("m")#text(" 个未知数时约 ")#raw("O(nm min(n,m))")#text("，空间 ")#raw("O(nm)")#text("。")
#strong[#text("下标")]#text("：增广矩阵为 ")#raw("n × (m+1)")#text("。")
#strong[#text("注意")]#text("：实数消元必须选绝对值最大的主元，避免数值误差。")





=== #text("现场记忆")

#text("每列从剩余行中选 ")#raw("|a[i][col]|")#text(" 最大的主元。")

#text("没有主元：这一列是自由变量。")

#text("出现：")

#raw("0x1 + 0x2 + ... + 0xm = c (c != 0)")

#text("则无解。")

#text("否则 ")#raw("rank == m")#text(" 唯一解，")#raw("rank < m")#text(" 多解。")


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("增广矩阵 0-indexed，所有行等长且至少含常数列。返回 -1 无解，否则为秩；自由变量取 0。空矩阵按 0 个变量、秩 0 处理，无法从空行推知额外变量数。EPS 是绝对误差，病态矩阵仍需按题目人工评估。")

=== 最小调用示例

```cpp
GaussD g;
vector<double> a;
assert(g.solve({{1, 1, 3}, {1, -1, 1}}, a) == 2);
assert(fabs(a[0] - 2) < 1e-8 && fabs(a[1] - 1) < 1e-8);
```

=== 模板引用

#source_code(("math/gauss_double.hpp",))


== #text("高斯消元：模素数线性方程组") <algo-math-gauss-mod>

#text("原稿来源：6.10 高斯消元：模素数线性方程组")


#strong[#text("用途")]#text("：求解素数模意义下的线性方程组；异或方程可直接取 ")#raw("MOD = 2")#text("。")
#strong[#text("复杂度")]#text("：消元 ")#raw("O(nm min(n,m))")#text("，另有至多 ")#raw("O(min(n,m) log MOD)")#text(" 的求逆；空间 ")#raw("O(nm)")#text("。")
#strong[#text("下标")]#text("：增广矩阵为 ")#raw("n × (m+1)")#text("。")
#strong[#text("限制")]#text("：")#raw("MOD")#text(" 必须为素数。")
#strong[#text("依赖")]#text("：")#ref(<algo-math-modint>, supplement: none)#text(" 中的 ")#raw("Mint<MOD>")#text("。")





=== #text("现场记忆")

#text("模素数下和普通高斯一样，只是：")

#raw("除以 pivot -> 乘 pivot.inv()")

#text("没有主元：该变量可能是自由变量。")

#text("出现：")

#raw("0x1 + ... + 0xm = c (c != 0)")

#text("则无解。")

#text("返回 ")#raw("-1")#text(" 表示无解；否则返回秩，")#raw("rank == m")#text(" 唯一解。")



=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("矩阵与答案 0-indexed，行宽统一；空矩阵约定为 0 变量。MOD 必须为素数，返回值和自由变量语义与实数版一致。")

=== 最小调用示例

```cpp
using Z = Mint<7>;
GaussM<7> g;
vector<Z> a;
assert(g.solve({{1, 1, 3}, {1, -1, 1}}, a) == 2 && a[0].v == 2 &&
       a[1].v == 1);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-math-modint>)[Mint 模整数骨架]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("math/gauss_mod.hpp",))


== 异或高斯消元：bitset <algo-math-gauss-xor>

*用途：* 模 2 的异或方程组；求可解性、系数矩阵秩和一个特解。适用于开关、灯阵、异或约束，区别于最大异或值线性基。

*复杂度：* n 行、m 个变量、bitset 容量 B，时间上界 `O(n*m*ceil(B/w) + m*ceil(B/w))`，空间 `O(n*ceil(B/w)+m)`；w 为机器字长。适合 B 接近变量上限，不能随意给过大的 B。

*下标与接口：* 变量 `0..m-1`，增广列是 m；`B>=m+1`。`gauss_xor<B>(a,m,ans)` 返回 -1 表示无解，否则返回秩 r，特解的自由变量取 0。解数为 `2^(m-r)`，大指数不能用 int 移位。传值消元保留输入，每次重置 ans。

*注意事项：* 输入 B 内其他未用位应为 0；m=0 时检查常数矛盾。无解时 ans 清零，但不能把这个值当成有效解。bitset 的 B 是编译期常量；这里只返回特解，不返回完整零空间基。

*现场记忆：* 行交换找主元，下面各行异或消元，逆序回代；常数位放 m，别误放到 B-1。

=== 最小调用示例

```cpp
vector<bitset<65>> a(2);
a[0][0] = a[0][1] = a[0][2] = 1;
a[1][1] = 1;
bitset<65> ans;
assert(gauss_xor<65>(a, 2, ans) == 2);
assert(ans[0] && !ans[1]);
```

=== 模板引用

#source_code(("math/gauss_xor.hpp",))

== 模素数行列式 <algo-math-determinant>

*用途：* 方阵行列式；为矩阵树定理、LGV 等提供计算接口，不把解方程的秩当作行列式。

*复杂度：* 时间 `O(n³ + n log P)`，空间 `O(n²)`；传值保留原矩阵。

*下标与接口：* 0-index 方阵，`det_mod(a,P)` 返回 `0..P-1`，输入元素可为负。空矩阵行列式为 1，奇异矩阵为 0。P 必须为质数且 `2<=P<=INT_MAX`；使用 ll 乘法在这个范围内安全。

*注意事项：* 换行时变号，主元乘到答案后才能归一或消去。本函数不适用于任意合数模，不在运行时验证素性；多测无全局状态。

*现场记忆：* 累乘主元、交换变号、找不到主元返回 0；行列式、秩、方程解是不同接口。

=== 最小调用示例

```cpp
assert(det_mod({{1, 2}, {3, 4}}, 101) == 99);
assert(det_mod({}, 101) == 1);
assert(det_mod({{1, 2}, {2, 4}}, 101) == 0);
```

=== 模板引用

#source_code(("math/determinant.hpp",))

== #text("容斥原理：子集枚举") <algo-math-subset-ie>

#text("原稿来源：6.11 容斥原理：子集枚举")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：枚举限制集合的非空子集，用奇加偶减计算若干集合的并集贡献。")
#strong[#text("复杂度")]#text("：")#raw("k")#text(" 个限制通常为 ")#raw("O(2^k)")#text("。")
#strong[#text("限制")]#text("：使用 64 位状态，要求 ")#raw("k < 64")#text("；实际受 ")#raw("2^k")#text(" 限制通常远小于此。")


=== #text("现场记忆")

#text("并集容斥：")

#raw("|A1 ∪ A2 ∪ ...| = 单个 - 两两交 + 三个交 - ...")

#text("子集大小为奇数加，偶数减。")

#text("“被若干数至少一个整除”：")

#text("子集交集对应 ")#raw("lcm")#text("，贡献为 ")#raw("floor(n / lcm)")#text("。")

#raw("lcm > n")#text(" 后该子集贡献直接为 ")#raw("0")#text("。")



=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("mask 0-indexed，限制数 0<=m<64；实际只适合可枚举的 m。work(mask) 返回交集贡献，同一调用内不依赖枚举顺序。总和和中间加减须落在返回类型内。")

=== 最小调用示例

```cpp
assert(subset_ie(3, [](uint64_t) { return 1LL; }) == 1);
```

=== 模板引用

#source_code(("math/subset_ie.hpp",))

==== #text("div_any")

```cpp
assert(div_any(10, vector<ll>{2, 3}) == 7);
```

#source_code(("math/div_any.hpp",))

== #text("自适应辛普森积分") <algo-math-adaptive-simpson>

#text("原稿来源：6.12 自适应辛普森积分")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：数值计算定积分，对较平滑函数自动细分误差较大的区间。")
#strong[#text("复杂度")]#text("：与函数形状和精度要求有关；空间为递归深度。")
#strong[#text("注意")]#text("：被积函数应在积分区间内有定义且数值稳定；")#raw("eps")#text(" 为绝对误差控制量。")




=== #text("现场记忆")

#text("整段 Simpson 为 ")#raw("S")#text("，左右两半为 ")#raw("L,R")#text("。")

#text("若：")

#raw("|L + R - S| <= 15 * eps")

#text("则接受，并做 Richardson 修正：")

#raw("L + R + (L + R - S) / 15")

#text("否则左右递归，误差各分 ")#raw("eps / 2")#text("。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("标量积分，无数组下标。eps>0、max_dep>=0，函数在整个区间有定义。达到深度上限会接受近似，误差保证不是对奇点、强振荡和数值病态函数的通用证明；这类应用标记待人工复核。")

=== 最小调用示例

```cpp
assert(fabs(adaptive_simpson([](double x) { return sin(x); }, 0.0,
                             acos(-1.0)) -
            2) < 1e-8);
```

=== 模板引用

#source_code(("math/adaptive_simpson.hpp",))


== #text("CRT 中国剩余定理") <algo-math-crt>

#text("原稿来源：6.13 CRT 中国剩余定理")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：合并模数两两互质的同余方程 ")#raw("x ≡ r[i] (mod m[i])")#text("。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(k log M)")#text("，空间 ")#raw("O(1)")
#strong[#text("限制")]#text("：模数两两互质，所有模数乘积需要能放进 ")#raw("long long")#text("。")
#strong[#text("返回")]#text("：")#raw("{最小非负解, 总模数}")#text("。")


=== #text("现场记忆")

#raw("当前：\nx ≡ ans (mod M)\n\n加入：\nx ≡ r (mod m)\n\n令 x = ans + M*t：\nM*t ≡ r-ans (mod m)\n\n两模数互质 -> M 有逆元。",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("r / m 均为等长 0-indexed 数组。m[i]>0 且两两互质，总模数不超过 LLONG_MAX；空方程返回 {0,1}。本版不是广义 CRT，不接受不互质系统。差值先提升到 i128。")

=== 最小调用示例

```cpp
assert(
    (crt(vector<ll>{2, 3}, vector<ll>{3, 5}) == pair<ll, ll>{8, 15}));
```

=== 模板引用

#source_code(("math/crt.hpp",))


== 扩展 Lucas：合数模组合数 <algo-math-ex-lucas>

*用途：* 求大 n 的 `C(n,k) mod M`，M 可为合数。将 M 分解成互质质数幂，每一部分剥离 p 因子，算单位阶乘后 CRT 合并。

*复杂度：* 初始化试除 `O(sqrt M)`，单位阶乘表时间、空间 `O(Σp^a)`；每次查询约 `O(Σ(log_p(n)*log n + log(p^a)))`。预处理代价取决于最大质数幂，不能因 n 大就忽略模数开表成本。

*接口与下标：* `ExLucas c(M)` / `init(M)`，`C(n,k)` 使用 ll 参数；M 为正 int，`M=1`、`n<0` 或 k 不在 `0..n` 返回 0。重新 init 清空旧模数的表；反复查询同一模数直接复用对象。

*注意事项：* 仅对剥离 p 后的互质部分求逆；p 的总指数保留并乘回。M 接近大质数时整表可能无法分配，本模板不承诺任意 int 模数都适合运行，赛前按 `Σp^a` 评估内存。不要套用质数阶乘逆元公式。

*现场记忆：* 数 p 的次数、剥 p 的阶乘、单位求逆、补回 p 次幂，最后 CRT；指数达到幂次时该部分自然为 0。

=== 最小调用示例

```cpp
ExLucas c(12);
assert(c.C(6, 2) == 3);
assert(c.C(1000000000000000000LL, 1) == 4);
c.init(1);
assert(c.C(0, 0) == 0);
```

=== 模板引用

*代码依赖：* #ref(<algo-math-crt>, supplement: none)。先复制 CRT，包含 exgcd。

#source_code(("math/ex_lucas.hpp",))

== #text("Miller-Rabin 素性测试") <algo-math-miller-rabin>

#text("原稿来源：6.14 Miller-Rabin 素性测试")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：快速判断一个 ")#raw("uint64_t")#text(" 整数是否为质数。")
#strong[#text("复杂度")]#text("：约 ")#raw("O(log n)")#text(" 次模乘，固定常数轮数")
#strong[#text("限制")]#text("：代码中的固定底数集合对 ")#raw("uint64_t")#text(" 范围确定正确。")


=== #text("现场记忆")

#raw("n-1 = d * 2^s，d 为奇数。\n\n检查：\na^d 是否为 1 / -1\n否则不断平方，看能否出现 -1。\n\n64 位确定底数：\n2, 325, 9375, 28178,\n450775, 9780504, 1795265022",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("无数组下标，isPrime(n) 接收 uint64_t；n<2 返回 false，固定底数组覆盖 uint64_t。mulMod / powMod 是内部正模数工具，不应以模数 0 调用。")

=== 最小调用示例

```cpp
assert(isPrime(18446744073709551557ULL) &&
       !isPrime(341550071728321ULL));
```

=== 模板引用

#source_code(("math/miller_rabin.hpp",))


== #text("Pollard-Rho 大整数分解") <algo-math-pollard-rho>

#text("原稿来源：6.15 Pollard-Rho 大整数分解")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：分解 ")#raw("uint64_t")#text(" 范围内的大整数质因数。")
#strong[#text("复杂度")]#text("：随机算法，分解因子 ")#raw("p")#text(" 的期望复杂度约 ")#raw("O(p^{1/2})")#text("；通常远快于试除到 ")#raw("√n")
#strong[#text("限制")]#text("：依赖 Miller-Rabin 判断质数。")





=== #text("现场记忆")

#raw("Miller-Rabin：判断当前数是不是质数。\n\nPollard-Rho：\n随机 f(x)=x²+c\nFloyd 找环，同时 gcd(|x-y|,n) 找因子。\n\n找到非平凡因子 d 后：\n递归分解 d 和 n/d。\n\n随机失败 / 得到 n -> 换随机参数重新跑。",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("无数组下标；factorize(n) 返回排序后的质因数 0-indexed 列表，1 返回空。0 没有有限质因数分解，此实现也返回空以免递归失控，不能将空列表解释为 0 的分解。随机 Rho 最坏时间不保证；Floyd 碰撞寻找因子 p 的典型期望步数约 p^(1/2)。模加先升到 u128，覆盖整个 uint64_t。")

=== 最小调用示例

```cpp
auto f = factorize(360);
assert((f == vector<u64>{2, 2, 2, 3, 3, 5}));
```

=== 模板引用

*代码依赖：* 先复制 #ref(<algo-math-miller-rabin>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("math/pollard_rho.hpp",))


== #text("BSGS / exBSGS 离散对数") <algo-math-bsgs>

#text("原稿来源：6.16 BSGS / exBSGS 离散对数")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：求最小非负整数 ")#raw("x")#text(" 满足 ")#raw("a^x ≡ b (mod m)")#text("；")#raw("exBSGS")#text(" 支持 ")#raw("gcd(a,m) != 1")#text("。")
#strong[#text("复杂度")]#text("：时间、空间均约 ")#raw("O(√m)")
#strong[#text("返回")]#text("：无解返回 ")#raw("-1")#text("。")
#strong[#text("注意")]#text("：")#raw("m")#text(" 很大时 ")#raw("O(√m)")#text(" 的哈希表可能无法承受。")





=== #text("现场记忆")

#raw("BSGS：\nx = i*m + j\na^j 表一边，a^{-m*i} * b 表另一边。\n\nexBSGS：\ng = gcd(a,m)\n\n若 b % g != 0 -> 无解\n否则同时除掉 g，并记录已经强制消掉多少个 a。\n\n直到 gcd(a,m)=1，再进入普通 BSGS。",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("标量接口，无数组下标；mod>0。BSGS 只用于互质底数，exBSGS 可处理不互质。a / b 可负并归一化；返回最小非负指数，无解 -1，mod=1 返回 0。平方根规模的哈希表必须能在比赛时间与内存内承受。")

=== 最小调用示例

```cpp
assert(exBSGS(2, 8, 13) == 3 && exBSGS(2, 4, 8) == 2 &&
       exBSGS(2, 3, 8) == -1);
```

=== 模板引用

#source_code(("math/bsgs.hpp",))


== #text("floor_sum") <algo-math-floor-sum>

#text("原稿来源：6.17 floor_sum")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：快速计算 ")#raw("Σ floor((a*i+b)/m)")#text("，")#raw("i = 0..n-1")#text("。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(log m)")#text("，空间 ")#raw("O(1)")
#strong[#text("限制")]#text("：")#raw("n >= 0")#text("，")#raw("m > 0")#text("；代码支持 ")#raw("a,b")#text(" 为负数。")
#strong[#text("返回")]#text("：使用 ")#raw("__int128")#text(" 避免求和溢出。")


=== #text("现场记忆")

#raw("求：\nΣ floor((a*i+b)/m)\n\n先把 a/m、b/m 的整数部分直接贡献到答案。\n\n剩余 a<m,b<m 后：\n交换坐标轴，问题规模类似 Euclid 下降。\n\n复杂度和 gcd 一样是 O(log m)。",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("标量 n>=0、m>0，i 从 0 到 n-1。返回 i128；有符号 64 位所有参数的任意组合不保证最终或中间结果能放入 i128，需限制总量。n=0 返回 0；a / b 可为负。")

=== 最小调用示例

```cpp
assert(floorSum(4, 3, 2, -1) == 1);
```

=== 模板引用

#source_code(("math/floor_sum.hpp",))


== #text("Cipolla / Tonelli-Shanks 模平方根") <algo-math-mod-sqrt>

#text("原稿来源：6.18 Cipolla / Tonelli-Shanks 模平方根")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：在奇质数模 ")#raw("p")#text(" 下求 ")#raw("x² ≡ n (mod p)")#text("；这里采用 Tonelli-Shanks。")
#strong[#text("复杂度")]#text("：通常 ")#raw("O(log² p)")
#strong[#text("限制")]#text("：")#raw("p")#text(" 必须为质数。")
#strong[#text("返回")]#text("：返回一个平方根；无解返回 ")#raw("-1")#text("。另一个根为 ")#raw("p-x")#text("。")


=== #text("现场记忆")

#raw("先 Euler 判据：\nn^((p-1)/2) != 1 -> 无平方根。\n\np % 4 == 3：\n直接 n^((p+1)/4)。\n\n一般情况：\np-1 = q * 2^s，q 为奇数\n找一个二次非剩余 z，再做 Tonelli-Shanks。",block:true)

=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("标量参数，n 可负，p 必须为正素数；支持 p=2。本模板实际为 Tonelli-Shanks。返回一个根 0..p-1 或 -1；另一个根为 (p-x)%p，x=0 时不要直接用 p-x 当最小非负根。")

=== 最小调用示例

```cpp
assert(modSqrt(4, 7) == 2 || modSqrt(4, 7) == 5);
assert(modSqrt(3, 7) == -1);
```

=== 模板引用

#source_code(("math/mod_sqrt.hpp",))

== 公平组合游戏：Nim 与 SG <algo-math-game-sg>

*用途：* 双方可用操作相同、确定性、无随机因素且有限结束的正常规则游戏。无路可走者输；多个互不影响的子游戏 SG 异或，非 0 为必胜。

*接口、下标与复杂度：* `nim_move(a)` 接收 0-index 非负 ll 堆大小，返回 `{堆下标,操作后大小}`，无必胜步返回 `{-1,0}`；时间 `O(堆数)`。`sg_subtract(n,moves)` 给出石子数 `0..n` 的 SG，每次取 moves 中一个正数，时间 `O(n*|moves|)`，空间 `O(n+|moves|)`。`sg_dag(g)` 用 1-index 有向无环状态图，时间空间 `O(V+E)`，递归深度最长状态链。

*模型与例：* 每次可取 1、2、3 个时，SG 为石子数模 4；两堆大小 1、2 的 Nim，异或为 3，将第二堆变成 1。SG 的 mex 是后继 SG 中最小未出现非负整数，最多等于后继个数。

*反常 Nim：* 若改为取走最后一颗者输，忽略空堆后，有大于 1 的堆时仍按异或判断；全为 1 时奇数堆必败、偶数堆必胜。该结论不能直接推广为任意反常游戏的 SG 异或。空局面的胜负须与题面终止规则一致。

*注意事项：* 本代码按正常规则求解，环、平局、双方操作不同和子游戏互相影响均不能直接套。所有状态局部初始化，可重复调用；空 moves 得到全 0，DAG 的终点为 0。深链需评估栈空间。

*现场记忆：* 先判是否公平、正常、无环、子游戏独立，再 mex / xor；不是所有“轮流操作”都能异或。

=== 最小调用示例

```cpp
auto [p, x] = nim_move({1, 2});
assert(p == 1 && x == 1);
assert(sg_subtract(10, {1, 2, 3})[10] == 2);
vector<vector<int>> g = {{}, {2}, {}};
assert(sg_dag(g)[1] == 1);
```

=== 模板引用

#source_code(("math/game_sg.hpp",))

