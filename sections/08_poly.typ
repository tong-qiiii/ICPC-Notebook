= 多项式 <chapter-poly>

#text("完整保留 raw/08_Poly.md 的 11 个原始条目及其重要变体，按原顺序排列。原稿附项按当前位置参与算法自动编号。")

#text("统一约定：输入规模、节点号、版本号和查询区间须合法；空区间仅在接口明确支持时使用。递归实现需评估栈空间；整数中间值及累计值必须在对应类型范围内。多测重新调用 init / build / clear，普通函数的外部维护状态由调用者清空。各模板独立选用，同名 Info / Tag / Seg / Query 不应同时粘贴。")

#import "../styles/source.typ": source_code


=== 公共代码与复制顺序

本章源码块仅展示算法自身实现；公共骨架只打印一次。按依赖链接复制所需的公共代码，再复制本算法与调用示例，比赛时不需要仓库头文件。已有基础算法被复用时，直接复制其原条目，不在每个代码块里重贴。

*依赖索引：* #link(<shared-poly-ntt>)[轻量 NTT]、#link(<shared-poly-vector>)[vector 运算]、#link(<shared-poly-core>)[Poly 核心]、#ref(<algo-poly-fps>, supplement: none)、#ref(<algo-poly-division-evaluation>, supplement: none)、#ref(<algo-poly-bostan-mori>, supplement: none)。

仅求卷积：复制轻量 NTT 即可。完整 Poly / FPS：复制 NTT 与 Poly 核心，再按依赖链接补齐成员实现。独立 vector 版多点求值 / 插值：复制 NTT 与 vector 运算，再复制对应乘积树；不必带上完整 Poly / FPS。两套接口混用时明确写 `::Poly`（vector）或 `poly998::Poly`（类），不要同时用裸 `Poly` 和 `using namespace poly998`。

=== 公共骨架：轻量 NTT <shared-poly-ntt>

用途：模 998244353 卷积与原地 NTT；下标从 0 开始，时间 O(n log n)、空间 O(n)。`convolution` 接受有符号系数并返回未裁剪的 n+m−1 项，空输入返回空；直接调用 `ntt` 时系数先归一化到 `[0, MOD)`。非空变换长度必须为 2 的幂且不超过 2^23，中间卷积长度也受限。快速幂指数非负，求逆参数模 MOD 非零。现场记忆：原根 3，逆变换用逆根，最后乘长度的逆元。

#source_code(("poly/ntt.hpp",))

=== 公共骨架：vector 运算 <shared-poly-vector>

先复制 #link(<shared-poly-ntt>)[轻量 NTT]。保持原 `Poly=vector<int>`、`modPow / mul / norm / addPoly / polyInv / polyMod` 接口；`norm` 只删除高位零，不归一化负数。除卷积外的输入系数须在 `[0, MOD)`。`polyInv(a,n)` 返回恰好 n 项，n≤0 返回空，非空求逆要求 a 的常数项非零；`polyMod(a,b)` 要求 b 非空且最高项非零，常数除数的余式为空。乘法、求逆、求余复杂度为 O(M(n)) 量级（以各自参与运算的最大长度计），加法 / 裁尾 O(n)。现场记忆：Newton 求逆 `g ← g(2−fg)`，翻转除法后减回乘积。

#source_code(("poly/vector_core.hpp",))

=== 公共骨架：Poly 核心 <shared-poly-core>

先复制 #link(<shared-poly-ntt>)[轻量 NTT]。完整类只在此处展示一次；用途、接口和调用仍见 #ref(<algo-poly-poly-core>, supplement: none)。

#source_code(("poly/poly_core.hpp",))

== #text("多项式核心 - NTT / Poly") <algo-poly-poly-core>

#text("原稿来源：8.1 多项式核心 - NTT / Poly")


#strong[#text("用途")]#text("：模 ")#raw("998244353")#text(" 下的多项式基础运算与 NTT 卷积；其余 FPS / 求值 / 复合接口统一声明在 ")#raw("Poly")#text(" 中。")
#strong[#text("复杂度")]#text("：乘法 ")#raw("O(n log n)")#text("，加减 / 截断 / 翻转 ")#raw("O(n)")#text("。")
#strong[#text("下标")]#text("：")#raw("0-indexed")#text("，")#raw("a[i] = [x^i]f(x)")#text("。")
#strong[#text("限制")]#text("：单次 NTT 长度 ")#raw("<= 2^23")#text("。")
#strong[#text("注意")]#text("：NTT、vector 运算与 Poly 核心分层集中在章首；后续板块只展示本算法对 Poly 成员函数的定义。按依赖链接依次复制所需实现，共用核心、FPS 等代码只粘贴一次。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口 / 变量")],[#text("含义")]),
[#raw("Poly(v)")],[#text("系数数组构造")],
[#raw("f.a")],[#text("系数数组")],
[#raw("f[i]")],[#raw("[x^i]f")#text("，越界返回 ")#raw("0")],
[#raw("f.raw(i)")],[#text("直接引用 ")#raw("a[i]")],
[#raw("f.size()")#text(" / ")#raw("f.deg()")],[#text("系数长度 / 次数")],
[#raw("f.trim()")#text(" / ")#raw("normalize()")],[#text("删除高位 ")#raw("0")],
[#raw("f.mod_xk(k)")],[#raw("f mod x^k")],
[#raw("f.mul_xk(k)")],[#raw("f * x^k")],
[#raw("f.div_xk(k)")],[#text("丢掉低 ")#raw("k")#text(" 项")],
[#raw("f.rev(n)")],[#text("按长度 ")#raw("n")#text(" 翻转")],
[#raw("convolution(a,b)")],[#text("NTT 卷积")],
[#raw("qpow(a,k)")],[#text("模幂")],
[#raw("invmod(a)")#text(" / ")#raw("inv_int(a)")],[#text("模逆")],
[#raw("mod_sqrt(a)")],[#text("模平方根，可能无解")],
)])



=== #text("现场记忆")

#raw("998244353 = 119 * 2^23 + 1，原根 G = 3。\nNTT 长度必须是 2 的幂，且 <= 2^23。\nPoly 自动去掉末尾 0；零多项式 empty()，deg() = -1。\nmod_xk(k) = 只保留 0 ... k-1 次项。",block:true)



=== 组织方式与维护约定

#text("struct Poly 保存系数，namespace poly998 仅隔离模数和通用函数；继承原实现的轻量组织。")

=== 接口、下标与适用边界补充

#text("统一 namespace poly998；外部若需归一化请写 poly998::norm(x)，避免 std::norm(int) 的重载。NTT 长度 2 的幂且 <=2^23；乘法先检查卷积所需长度。复合 FPS 的中间卷积长度也受此限制，不能只检查最终截断长度。向量动态分配仍受比赛内存限制。")

=== 轻量 NTT 最小调用

```cpp
auto c = poly998::convolution({1, 2, 3}, {4, 5});
assert((c == vector<int>{4, 13, 22, 15}));
```

#source_code(("poly/ntt.hpp",), shared: <shared-poly-ntt>)

=== 最小调用示例

```cpp
using namespace poly998;
Poly f({1, 2, 3}), g({4, 5});
assert((f * g)[2] == 22);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]；各依赖只复制一次。

#source_code(("poly/poly_core.hpp",), shared: <shared-poly-core>)


== #text("形式幂级数 FPS - 逆 / ln / exp / 幂 / sqrt / 三角函数") <algo-poly-fps>

#text("原稿来源：8.2 形式幂级数 FPS - 逆 / ln / exp / 幂 / sqrt / 三角函数")


#strong[#text("用途")]#text("：形式幂级数的求导、积分、逆、对数、指数、整数幂、平方根及三角 / 反三角函数。")
#strong[#text("复杂度")]#text("：主要操作 ")#raw("O(M(n))")#text("，其中 ")#raw("M(n)=O(n log n)")#text("。")
#strong[#text("下标")]#text("：")#raw("0-indexed")#text("。")
#strong[#text("依赖")]#text("：")#raw("多项式核心 - NTT / Poly")#text("。")
#strong[#text("注意")]#text("：")#raw("inv")#text(": ")#raw("f[0]!=0")#text("；")#raw("log")#text(": ")#raw("f[0]==1")#text("；")#raw("exp/sin/cos/tan/asin/atan")#text(": ")#raw("f[0]==0")#text("。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("f.deriv()")],[#text("求导")],
[#raw("f.integr()")],[#text("积分，常数项取 ")#raw("0")],
[#raw("f.inv(n)")],[#raw("1/f mod x^n")],
[#raw("f.log(n)")],[#raw("ln(f) mod x^n")],
[#raw("f.exp(n)")],[#raw("exp(f) mod x^n")],
[#raw("f.pow(k,n)")],[#raw("f^k mod x^n")],
[#raw("f.sqrt(n)")],[#text("平方根，可能无解")],
[#raw("f.sin(n)")],[#raw("sin(f) mod x^n")],
[#raw("f.cos(n)")],[#raw("cos(f) mod x^n")],
[#raw("f.tan(n)")],[#raw("tan(f) mod x^n")],
[#raw("f.asin(n)")],[#raw("asin(f) mod x^n")],
[#raw("f.atan(n)")],[#raw("atan(f) mod x^n")],
)])



=== #text("现场记忆")

#raw("inv：g <- g(2 - fg)\n\nlog f = ∫ f' / f\n\nexp：g <- g(1 + f - log g)\n\npow：\nf = x^t * c * g, g[0]=1\nf^k = x^(tk) * c^k * exp(k log g)\n\nsqrt：\n最低非零次数 t >= n -> 直接为 0\n否则 t 必须为偶数\ng <- (g + f/g) / 2\n\nsin/cos：用 exp(i f)，i^2 = -1\nasin' = f' / sqrt(1-f^2)\natan' = f' / (1+f^2)",block:true)


=== 组织方式与维护约定

#text("保留原 Poly 成员函数及对应普通辅助函数，直接复用核心系数状态；不增加通用策略封装。")

=== 接口、下标与适用边界补充

#text("n>=0 且 n<MOD，输入系数 0-indexed；各操作按原接口检查常数项前提。sqrt 的最低非零次数为奇数或常数项非剩余时无解，返回 nullopt。所有中间卷积均不得超过 2^23。逆三角函数使用常数项 0 的形式分支。")

=== 最小调用示例

```cpp
using namespace poly998;
Poly f({1, 2, 3});
assert((f * f.inv(10)).mod_xk(10) == Poly(1));
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]、#link(<shared-poly-core>)[Poly 核心]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("poly/fps.hpp",))


== #text("多项式除法 / 多点求值 / 插值") <algo-poly-division-evaluation>

#text("原稿来源：8.3 多项式除法 / 多点求值 / 插值")


#strong[#text("用途")]#text("：快速求多项式商余、多点求值，以及由任意互异点值恢复多项式。")
#strong[#text("复杂度")]#text("：除法 ")#raw("O(M(n))")#text("；多点求值 / 插值 ")#raw("O(M(n) log n)")#text("。")
#strong[#text("下标")]#text("：")#raw("0-indexed")#text("。")
#strong[#text("依赖")]#text("：多项式核心、FPS ")#raw("inv()")#text("。")
#strong[#text("注意")]#text("：插值要求 ")#raw("xs[i]")#text(" 在模 ")#raw("998244353")#text(" 意义下两两不同。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("f.divmod(g)")],[#raw("{商, 余数}")],
[#raw("f / g")],[#text("商")],
[#raw("f % g")],[#text("余数")],
[#raw("f.eval(xs)")],[#text("返回所有 ")#raw("f(xs[i])")],
[#raw("Poly::interpolate(xs,ys)")],[#text("插值，重复横坐标返回 ")#raw("nullopt")],
)])



=== #text("现场记忆")

#raw("除法：\n翻转 A、B，把最高次项变成常数项；\nQ^R = A^R / B^R，再翻回来。\n\n多点求值：\n乘积树存 ∏(x-x_i)\n根处 f % prod[root]\n向儿子继续取模\n\n插值：\nP(x)=∏(x-x_i)\n叶子权值 = y_i / P'(x_i)\n向上合并：\nleft * prod[right] + right * prod[left]",block:true)


=== 组织方式与维护约定

#text("保留原 Poly 成员函数及对应普通辅助函数，直接复用核心系数状态；不增加通用策略封装。")

=== 最小调用示例

```cpp
using namespace poly998;
Poly f({1, 2, 3}), g({1, 1});
auto [q, r] = f.divmod(g);
assert(q * g + r == f);
auto ys = f.eval({0, 1, 2});
assert(Poly::interpolate({0, 1, 2}, ys) == f);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]、#link(<shared-poly-core>)[Poly 核心]、#ref(<algo-poly-fps>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("poly/division_evaluation.hpp",))


== #text("多项式平移 / Chirp-Z / 连续点值平移") <algo-poly-taylor-shift>

#text("原稿来源：8.4 多项式平移 / Chirp-Z / 连续点值平移")


#strong[#text("用途")]#text("：快速求 ")#raw("f(x+c)")#text("、等比点 ")#raw("f(1),f(q),...")#text("，以及已知 ")#raw("f(0..n-1)")#text(" 后求另一段连续点值。")
#strong[#text("复杂度")]#text("：均为 ")#raw("O(M(n+m))")#text(" 量级。")
#strong[#text("下标")]#text("：")#raw("0-indexed")#text("。")
#strong[#text("依赖")]#text("：多项式核心。")
#strong[#text("注意")]#text("：")#raw("consecutive_value_shift(y,c,m)")#text(" 中 ")#raw("y[i]=f(i)")#text("，返回 ")#raw("f(c),...,f(c+m-1)")#text("。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("f.shift(c)")],[#raw("f(x+c)")],
[#raw("chirp_z(f,q,m)")],[#raw("f(1), f(q), ..., f(q^(m-1))")],
[#raw("consecutive_value_shift(y,c,m)")],[#text("已知 ")#raw("f(0..n-1)")#text("，求 ")#raw("f(c..c+m-1)")],
)])



=== #text("现场记忆")

#raw("Taylor shift：\nA_i = a_i * i! 后翻转\nB_i = c^i / i!\n卷积后再除 i!\n\nChirp-Z：\nki = C(k+i,2) - C(k,2) - C(i,2)\n转成一次卷积。\n\n连续点值平移：\nLagrange 权值 (-1)^(n-1-i) / i! / (n-1-i)!\n1/(c+j-i) 转卷积。\n目标正好落在 0..n-1 时直接返回已知值。",block:true)


=== 组织方式与维护约定

#text("保留原 Poly 成员函数及对应普通辅助函数，直接复用核心系数状态；不增加通用策略封装。")

=== 最小调用示例

```cpp
using namespace poly998;
assert(Poly({1, 2, 3}).shift(1) == Poly({6, 8, 3}));
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]、#link(<shared-poly-core>)[Poly 核心]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("poly/taylor_shift.hpp",))

==== #text("chirp_z")

```cpp
using namespace poly998;
assert((chirp_z(Poly({1, 2}), 2, 3) == vector<int>{3, 5, 9}));
```

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]、#link(<shared-poly-core>)[Poly 核心]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("poly/chirp_z.hpp",))
==== #text("consecutive_shift")

```cpp
using namespace poly998;
assert((consecutive_value_shift({1, 3}, 2, 3) == vector<int>{5, 7, 9}));
```

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]、#link(<shared-poly-core>)[Poly 核心]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("poly/consecutive_shift.hpp",))

== #text("位运算卷积 - FWT XOR / OR / AND") <algo-poly-fwt-xor>

#text("原稿来源：8.5 位运算卷积 - FWT XOR / OR / AND")


#strong[#text("用途")]#text("：求下标通过 ")#raw("xor / or / and")#text(" 合并得到的卷积。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(N log N)")#text("，空间 ")#raw("O(N)")#text("，")#raw("N")#text(" 为补齐后的 2 的幂。")
#strong[#text("下标")]#text("：mask ")#raw("0-indexed")#text("。")
#strong[#text("依赖")]#text("：多项式核心中的 ")#raw("MOD / norm / invmod")#text("。")
#strong[#text("注意")]#text("：返回长度为 ")#raw(">= max(|a|,|b|)")#text(" 的最小 2 的幂。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("计算")]),
[#raw("xor_convolution(a,b)")],[#raw("c[k] = sum_{i^j=k} a[i]b[j]")],
[#raw("or_convolution(a,b)")],[#raw("c[k] = sum_{i|j=k} a[i]b[j]")],
[#raw("and_convolution(a,b)")],[#raw("c[k] = sum_{i&j=k} a[i]b[j]")],
)])



=== #text("现场记忆")

#raw("XOR：\n(x,y) -> (x+y,x-y)\n逆变换再做一次，最后整体 / N\n\nOR：\n有 bit 的位置 += 没 bit 的位置\n逆变换改成 -=\n\nAND：\n没 bit 的位置 += 有 bit 的位置\n逆变换改成 -=",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
using namespace poly998;
auto r = xor_convolution({1, 2}, {3, 4});
assert(r.size() == 2);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]、#link(<shared-poly-core>)[Poly 核心]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("poly/fwt_xor.hpp",))

==== #text("fwt_or")

```cpp
using namespace poly998;
auto r = or_convolution({1, 2}, {3, 4});
assert(r.size() == 2);
```

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]、#link(<shared-poly-core>)[Poly 核心]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("poly/fwt_or.hpp",))
==== #text("fwt_and")

```cpp
using namespace poly998;
auto r = and_convolution({1, 2}, {3, 4});
assert(r.size() == 2);
```

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]、#link(<shared-poly-core>)[Poly 核心]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("poly/fwt_and.hpp",))

== #text("线性递推 - Bostan-Mori / Berlekamp-Massey") <algo-poly-bostan-mori>

#text("原稿来源：8.6 线性递推 - Bostan-Mori / Berlekamp-Massey")


#strong[#text("用途")]#text("：求常系数线性递推的超远项；由序列前缀恢复最短线性递推；求有理生成函数 ")#raw("[x^n]P/Q")#text("。")
#strong[#text("复杂度")]#text("：Bostan-Mori ")#raw("O(M(d) log n)")#text("；BM ")#raw("O(L^2)")#text("。")
#strong[#text("下标")]#text("：序列从 ")#raw("a[0]")#text(" 开始。")
#strong[#text("依赖")]#text("：多项式核心、FPS、多项式除法。")
#strong[#text("注意")]#text("：递推系数约定为 ")#raw("a[n] = coef[0]a[n-1] + ... + coef[d-1]a[n-d]")#text("。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("bostan_mori(P,Q,n)")],[#raw("[x^n] P(x)/Q(x)")],
[#raw("linear_recurrence_nth(init,coef,k)")],[#text("求递推第 ")#raw("k")#text(" 项")],
[#raw("berlekamp_massey(prefix)")],[#text("猜最短线性递推")],
)])



=== #text("现场记忆")

#raw("递推：\na[n] = c1*a[n-1] + ... + cd*a[n-d]\nQ(x) = 1 - c1*x - ... - cd*x^d\nP = (A * Q) mod x^d\n\nBostan-Mori：\nQ-(x) = Q(-x)\nR = P(x)Q(-x)\nS = Q(x)Q(-x)\n\nn 奇 -> P 取 R 奇数项\nn 偶 -> P 取 R 偶数项\nQ 始终取 S 偶数项\nn >>= 1\n\nBM 返回的 coef 与 linear_recurrence_nth 的方向一致。",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
using namespace poly998;
assert(bostan_mori(Poly(1), Poly({1, -1}), 1000000000000LL) == 1);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]、#link(<shared-poly-core>)[Poly 核心]、#ref(<algo-poly-fps>, supplement: none)、#ref(<algo-poly-division-evaluation>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("poly/bostan_mori.hpp",))

==== #text("linear_recurrence")

```cpp
using namespace poly998;
assert(linear_recurrence_nth({0, 1}, {1, 1}, 10) == 55);
```

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]、#link(<shared-poly-core>)[Poly 核心]、#ref(<algo-poly-fps>, supplement: none)、#ref(<algo-poly-division-evaluation>, supplement: none)、#ref(<algo-poly-bostan-mori>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("poly/linear_recurrence.hpp",))
==== #text("berlekamp_massey")

```cpp
using namespace poly998;
assert(
    (berlekamp_massey({0, 1, 1, 2, 3, 5, 8, 13}) == vector<int>{1, 1}));
```

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]、#link(<shared-poly-core>)[Poly 核心]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("poly/berlekamp_massey.hpp",))

== #text("多项式复合 / 拉格朗日反演") <algo-poly-composition>

#text("原稿来源：8.7 多项式复合 / 拉格朗日反演")


#strong[#text("用途")]#text("：小规模形式幂级数复合、复合逆，以及用 Lagrange Inversion 求单个系数。")
#strong[#text("复杂度")]#text("：")#raw("compose_slow")#text(" 约 ")#raw("O(n M(n))")#text("；复合逆同量级；拉格朗日单项系数主要为 FPS ")#raw("pow/inv")#text("。")
#strong[#text("下标")]#text("：")#raw("0-indexed")#text("。")
#strong[#text("依赖")]#text("：多项式核心、FPS。")
#strong[#text("限制")]#text("：")#raw("compose_slow")#text(" 要求 ")#raw("g[0]=0")#text("；复合逆要求 ")#raw("f[0]=0, f[1]!=0")#text("。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("f.compose_slow(g,n)")],[#raw("f(g(x)) mod x^n")],
[#raw("f.compositional_inverse_slow(n)")],[#text("求 ")#raw("g")#text(" 使 ")#raw("f(g(x))=x mod x^n")],
[#raw("lagrange_y_power_coeff(phi,n,k)")],[#raw("y=x*phi(y)")#text(" 时 ")#raw("[x^n]y^k")],
[#raw("lagrange_inverse_coeff(f,n)")],[#raw("[x^n] f^{-1}(x)")#text("，这里是复合逆")],
)])



=== #text("现场记忆")

#raw("y = x * phi(y)：\n\n[x^n] y^k\n= k/n * [t^(n-k)] phi(t)^n\n\n若 g = f^{-1}（复合逆）：\nf(t) = t * h(t)\nphi(t) = 1 / h(t)\n\n[x^n] g\n= 1/n * [t^(n-1)] (t/f(t))^n\n\n复合逆 Newton：\ng <- g - (f(g)-x) / f'(g)",block:true)


=== 组织方式与维护约定

#text("保留原 Poly 成员函数及对应普通辅助函数，直接复用核心系数状态；不增加通用策略封装。")

=== 最小调用示例

```cpp
using namespace poly998;
Poly f({0, 1, 1});
auto g = f.compositional_inverse_slow(10);
assert(f.compose_slow(g, 10) == Poly({0, 1}));
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]、#link(<shared-poly-core>)[Poly 核心]、#ref(<algo-poly-fps>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("poly/composition.hpp",))


== #text("生成函数 OGF / EGF") <algo-poly-ogf>

#text("原稿来源：8.8 生成函数 OGF / EGF")


#strong[#text("用途")]#text("：把序列系数转为普通生成函数或指数生成函数。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n)")#text("，空间 ")#raw("O(n)")#text("。")
#strong[#text("下标")]#text("：")#raw("0-indexed")#text("。")
#strong[#text("依赖")]#text("：多项式核心。")



=== #text("现场记忆")

#raw("OGF：\nF(x) = sum a[n] x^n\nPoly 系数直接等于 a[n]\n\nEGF：\nF(x) = sum a[n] x^n / n!\nPoly 系数等于 a[n] / n!",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
using namespace poly998;
assert(ogf_from_sequence({1, 2, 3}) == Poly({1, 2, 3}));
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]、#link(<shared-poly-core>)[Poly 核心]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("poly/ogf.hpp",))

==== #text("egf")

```cpp
using namespace poly998;
assert(egf_from_sequence({1, 2, 6}) == Poly({1, 2, 3}));
```

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]、#link(<shared-poly-core>)[Poly 核心]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("poly/egf.hpp",))

== #text("多项式多点求值") <algo-poly-multipoint>

#text("原稿来源：8.9 多项式多点求值")


#strong[#text("用途")]#text("：给定多项式 ")#raw("f(x)")#text(" 和 ")#raw("x[0..n-1]")#text("，快速计算全部 ")#raw("f(x[i])")#text("。")
#strong[#text("复杂度")]#text("：")#raw("O(n log² n)")#text("，空间 ")#raw("O(n log n)")
#strong[#text("下标")]#text("：系数、点均 ")#raw("0-indexed")
#strong[#text("限制")]#text("：模数固定为 ")#raw("998244353")#text("。")





=== #text("现场记忆")

#raw("建乘积树：\nP_node(x) = ∏(x-x_i)\n\n根上：\nf %= P_root\n\n向下：\n左儿子拿 f % P_left\n右儿子拿 f % P_right\n\n到叶子时余式就是常数 f(x_i)。\n\n多项式取模：\n反转 -> 求逆 -> 做多项式除法。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("此版使用全局 Poly=vector<int>，与 poly998::Poly 为不同实现，保留为独立选择；只依赖章首 NTT 与 vector 运算。系数须已归一化到 [0,MOD)，点坐标构造时归一化；查询可重复。多项式除数非空且最高项非零，NTT 长度 <=2^23。")

=== vector 公共接口最小调用

```cpp
Poly a{1, 2, 3};
auto iv = polyInv(a, 4);
auto one = mul(a, iv);
one.resize(4);
assert((one == Poly{1, 0, 0, 0}));
assert((polyMod(a, Poly{1, 1}) == Poly{2}));
```

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]；各依赖只复制一次。

#source_code(("poly/vector_core.hpp",), shared: <shared-poly-vector>)

=== 最小调用示例

```cpp
Multipoint t(vector<int>{0, 1, 2});
assert((t.eval(Poly{1, 2, 3}) == vector<int>{1, 6, 17}));
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]、#link(<shared-poly-vector>)[vector 运算]；各依赖只复制一次。

#source_code(("poly/multipoint.hpp",))


== #text("多项式快速插值") <algo-poly-fast-interpolation>

#text("原稿来源：8.10 多项式快速插值")


#strong[#text("用途")]#text("：给定 ")#raw("n")#text(" 个互异点 ")#raw("(x_i,y_i)")#text("，在 ")#raw("998244353")#text(" 下快速恢复次数 ")#raw("< n")#text(" 的多项式。")
#strong[#text("复杂度")]#text("：")#raw("O(n log² n)")#text("，空间 ")#raw("O(n log n)")
#strong[#text("下标")]#text("：系数 ")#raw("0-indexed")
#strong[#text("限制")]#text("：所有 ")#raw("x_i")#text(" 在模 ")#raw("998244353")#text(" 意义下两两不同。")





=== #text("现场记忆")

#raw("先令：\nP(x) = ∏(x-x_i)\n\nLagrange：\nf(x) = Σ y_i / P'(x_i) * P(x)/(x-x_i)\n\n步骤：\n1. 乘积树求 P\n2. 求 P'\n3. 多点求值 P'(x_i)\n4. w_i = y_i / P'(x_i)\n5. 分治合并 w_i * 其它因子\n\n核心和多点求值共用同一棵乘积树。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("保留原独立 ProductTree 实现，共用章首 NTT 与 vector 运算，不与 ")#ref(<algo-poly-division-evaluation>, supplement: none)#text(" 合并。x / y 等长且 x 模 MOD 互异；y 已补上负数归一化，重复点断言拒绝。空输入返回空数组。中间 NTT 长度 <=2^23。")

=== 最小调用示例

```cpp
assert((fastInterpolation({0, 1, 2}, {1, 6, 17}) == Poly{1, 2, 3}));
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-poly-ntt>)[轻量 NTT]、#link(<shared-poly-vector>)[vector 运算]；各依赖只复制一次。

#source_code(("poly/fast_interpolation.hpp",))


== #text("拉格朗日插值") <algo-poly-lagrange>

#text("原稿来源：8.11 拉格朗日插值")


#strong[#text("用途")]#text("：给定若干互异点 ")#raw("(x_i,y_i)")#text("，多次查询次数 ")#raw("< n")#text(" 的插值多项式在任意 ")#raw("x")#text(" 处的值。")
#strong[#text("复杂度")]#text("：预处理 ")#raw("O(n²)")#text("，单次查询 ")#raw("O(n log MOD)")#text("（每点单独求逆），空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：点 ")#raw("0-indexed")
#strong[#text("限制")]#text("：模数 ")#raw("MOD")#text(" 为质数，所有 ")#raw("x_i")#text(" 模 ")#raw("MOD")#text(" 两两不同。")





=== #text("现场记忆")

#raw("重心形式：\n\nw_i = 1 / ∏_{j!=i}(x_i-x_j)\n\nf(x) =\n∏(x-x_j) *\nΣ y_i*w_i/(x-x_i)\n\n查询点恰好等于某个 x_i 时，直接返回 y_i，不能除 0。",block:true)

=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("x / y 等长，模 MOD 互异；构造时检查分母非零。查询的原实现逐点快速幂求逆，实际 O(n log MOD)，不是严格 O(n)。空输入视为零多项式，query 返回 0。")

=== 最小调用示例

```cpp
Lagrange l({0, 1, 2}, {1, 6, 17});
assert(l.query(3) == 34);
```

=== 模板引用

#source_code(("poly/lagrange.hpp",))

