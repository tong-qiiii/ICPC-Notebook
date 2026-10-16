= 动态规划 <chapter-dp>

#text("完整保留 raw/10_dp.md 的 17 个原始条目及其重要变体，按原顺序排列。原稿附项按当前位置参与算法自动编号。")

#text("统一约定：输入规模、节点号、版本号和查询区间须合法；空区间仅在接口明确支持时使用。递归实现需评估栈空间；整数中间值及累计值必须在对应类型范围内。多测重新调用 init / build / clear，普通函数的外部维护状态由调用者清空。各模板独立选用，同名 Info / Tag / Seg / Query 不应同时粘贴。")

#let source_code(files) = {
  let source = files.map(file => read("../code/" + file)
    .replace(regex("(?m)^#pragma once\r?\n"), "")
    .replace(regex("(?m)^#include \".*\"\r?\n"), "")).join("\n")
  raw(source, lang: "cpp", block: true)
}

== #text("0/1、完全、多重、分组背包") <algo-dp-knapsack01>

#text("原稿来源：10.1 0/1、完全、多重、分组背包")


#strong[#text("用途")]#text("：容量型背包的四个常用版本，求不超过容量 ")#raw("C")#text(" 时的最大价值。")
#strong[#text("复杂度")]#text("：0/1、完全 ")#raw("O(nC)")#text("；多重 ")#raw("O(C \\sum \\log(c_i+1))")#text("；分组 ")#raw("O(C \\sum |G_i|)")#text("；空间均为 ")#raw("O(C)")
#strong[#text("下标")]#text("：物品 ")#raw("1 ~ n")#text("，容量 ")#raw("0 ~ C")
#strong[#text("限制")]#text("：物品重量 ")#raw("w > 0")
#strong[#text("注意")]#text("：默认“不超过容量”且允许不选；恰好装满时 ")#raw("dp[1..C] = -INF")#text("。")

=== #text("0/1 背包")


=== #text("完全背包")


=== #text("多重背包（二进制拆分）")


=== #text("分组背包")


=== #text("现场记忆")

#raw("0/1：容量倒序\n完全：容量正序\n多重：二进制拆成若干个 0/1 物品\n分组：容量倒序，同组物品都从较小容量的旧状态转移\n\n恰好装满：dp[0] = 0，其余 = -INF",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("物品数组 1-indexed，a[0] 占位；每项 {w,v}。函数返回 dp[0..C]，默认允许不选且容量不超过 C。exact=true 则只允许恰好装满，不可达值 LLONG_MIN/4；负价值也可使用。C>=0，所有重量>0，合法最优值远离负无穷且累计不溢出。")

=== 最小调用示例

```cpp
auto dp = knapsack01(5, vector<pair<int, ll>>{{}, {2, 3}, {3, 5}});
assert(dp[5] == 8);
```

=== 模板引用

#source_code(("dp/knapsack01.hpp",))

==== #text("knapsack_complete")

```cpp
auto dp =
    knapsack_complete(5, vector<pair<int, ll>>{{}, {2, 3}, {3, 5}});
assert(dp[5] == 8);
```

#source_code(("dp/knapsack_complete.hpp",))
==== #text("knapsack_bounded")

```cpp
auto d = knapsack_bounded(6, vector<BoundedItem>{{}, {2, 2, 3}});
assert(d[6] == 6);
```

#source_code(("dp/knapsack_bounded.hpp",))
==== #text("knapsack_group")

```cpp
auto d = knapsack_group(
    4, vector<vector<pair<int, ll>>>{{}, {{2, 3}, {3, 7}}, {{1, 2}}});
assert(d[4] == 9);
```

#source_code(("dp/knapsack_group.hpp",))

== #text("区间 DP（石子合并骨架）") <algo-dp-interval-merge>

#text("原稿来源：10.2 区间 DP（石子合并骨架）")


#strong[#text("用途")]#text("：处理“将区间切成左右两部分再合并”的区间 DP；代码对应经典连续石子合并最小代价。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n^3)")#text("，空间 ")#raw("O(n^2)")
#strong[#text("下标")]#text("：")#raw("1-indexed")


=== #text("现场记忆")

#raw("枚举顺序：区间长度 -> 左端点 -> 断点 k\ndp[l][r] = min(dp[l][k] + dp[k+1][r] + merge_cost)\n初始化 dp[i][i] = 0",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
assert(interval_merge(vector<ll>{0, 1, 2, 3}) == 9);
```

=== 模板引用

#source_code(("dp/interval_merge.hpp",))


== #text("树形 DP（最大权独立集骨架）") <algo-dp-tree-independent-set>

#text("原稿来源：10.3 树形 DP（最大权独立集骨架）")


#strong[#text("用途")]#text("：树上父子状态合并；代码对应“相邻点不能同时选择”的最大权独立集，也是常用 ")#raw("dp[u][0/1]")#text(" 骨架。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n)")#text("，空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：节点 ")#raw("1 ~ n")
#strong[#text("注意")]#text("：链状树较深时递归可能爆栈。")


=== #text("现场记忆")

#raw("dp[u][0]：不选 u\ndp[u][1]：选 u\n\n不选 u：dp[u][0] += max(dp[v][0], dp[v][1])\n选 u：  dp[u][1] += dp[v][0]",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
assert(tree_independent_set(vector<vector<int>>{{}, {2, 3}, {1}, {1}},
                            vector<ll>{0, 5, 4, 3}) == 7);
```

=== 模板引用

#source_code(("dp/tree_independent_set.hpp",))


== #text("换根 DP（全点距离和）") <algo-dp-reroot-distance>

#text("原稿来源：10.4 换根 DP（全点距离和）")


#strong[#text("用途")]#text("：求树上每个点到所有点的距离和；无权树和加权树均可使用。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n)")#text("，空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：节点 ")#raw("1 ~ n")
#strong[#text("注意")]#text("：")#raw("siz[u]")#text(" 为子树节点数；链状树较深时递归可能爆栈。")


=== #text("现场记忆")

#raw("第一次 DFS：\ndown[u] += down[v] + siz[v] * w\n\nu -> 儿子 v 换根：\nv 子树 siz[v] 个点距离减少 w\n其余 n-siz[v] 个点距离增加 w\n\nans[v] = ans[u] + (n - 2*siz[v]) * w",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
auto a = reroot_distance(
    vector<vector<pair<int, ll>>>{{}, {{2, 3}}, {{1, 3}}});
assert(a[1] == 3 && a[2] == 3);
```

=== 模板引用

#source_code(("dp/reroot_distance.hpp",))


== #text("数位 DP（记忆化通用骨架）") <algo-dp-digit-dp>

#text("原稿来源：10.5 数位 DP（记忆化通用骨架）")


#strong[#text("用途")]#text("：统计 ")#raw("[0, x]")#text(" 中满足数位限制的整数个数，区间询问用前缀差。")

#strong[#text("复杂度")]#text("：时间 ")#raw("O(D * S * 10)")#text("，空间 ")#raw("O(D * S)")#text("，")#raw("D <= 19")

#strong[#text("下标")]#text("：数位 ")#raw("0-indexed")

#strong[#text("注意")]#text("：")
#text("• 按题主要修改：")#raw("S")#text("、终止条件、状态转移。")
#text("• 默认前导 ")#raw("0")#text(" 可直接参与转移；若首位、相邻位等受前导零影响，再加 ")#raw("started")#text("。")
#text("• 只记忆化 ")#raw("lim = false")#text(" 的状态。")





=== #text("现场记忆")

#raw("dfs(pos, st, lim)：处理到 pos，当前状态 st，前缀是否贴着上界\nup = lim ? dig[pos] : 9\nnlim = lim && (d == dig[pos])\n\n按题只想三件事：\n1. st 记录什么\n2. 放入 d 后 st 怎么转移\n3. pos == n 时哪些状态合法\n\n区间答案 = calc(r) - calc(l - 1)\n前导零影响状态时，再加 started",block:true)



=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("digit_count(x,S,initial,next,accept) 统计 [0,x]；digit_query(l,r,...) 取前缀差，负整数不计入。状态 0..S-1，数位 0-indexed；next(st,d,started) 返回新状态或 -1 禁止，前导零不调用 next。终止 accept(st,started) 决定是否合法及是否计入 0。每次调用独立清空 memo；使用 GNU i128 返回完整 [0,LLONG_MAX] 的计数，不能直接 cout，需转换或打印函数。调用者必须保证 S>0、initial 合法、状态充分且回调无历史依赖。")

=== 最小调用示例

```cpp
auto next = [](int st, int d, bool) { return (st + d) % 3; };
auto accept = [](int st, bool) { return st == 0; };
assert(digit_query(0, 20, 3, 0, next, accept) == 7);
```

=== 模板引用

#source_code(("dp/digit_dp.hpp",))


== #text("状压 DP（Hamilton 路径 / TSP 骨架）") <algo-dp-hamilton-path>

#text("原稿来源：10.6 状压 DP（Hamilton 路径 / TSP 骨架）")


#strong[#text("用途")]#text("：小规模点集上枚举已选集合；代码求从 ")#raw("s")#text(" 出发、经过所有点、最终到 ")#raw("t")#text(" 的最短 Hamilton 路径。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n^2 2^n)")#text("，空间 ")#raw("O(n 2^n)")
#strong[#text("下标")]#text("：节点 ")#raw("0 ~ n-1")
#strong[#text("限制")]#text("：通常 ")#raw("n <= 20")


=== #text("现场记忆")

#raw("dp[mask][u] = 已访问 mask，当前停在 u 的最优值\nmask,u -> mask|(1<<v),v\n\nHamilton 路径：dp[full][t]\nTSP 回到起点：枚举终点 u，再加 w[u][s]",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("矩阵、s、t 0-indexed，非空时 s/t 必须合法；n<=20 且能承受 O(n2^n) 内存。无边用 >=INF/2，INF=LLONG_MAX/4；有限路径和必须远离 INF。返回 dp[full][t]，无解仍为 INF。空图约定返回 0。")

=== 最小调用示例

```cpp
assert(
    hamilton_path(vector<vector<ll>>{{0, 2, 10}, {2, 0, 3}, {10, 3, 0}},
                  0, 2) == 5);
```

=== 模板引用

#source_code(("dp/hamilton_path.hpp",))


== #text("轮廓线 DP（多米诺骨牌铺满）") <algo-dp-domino-tilings>

#text("原稿来源：10.7 轮廓线 DP（多米诺骨牌铺满）")


#strong[#text("用途")]#text("：网格按行推进，只保留当前轮廓状态；代码统计带障碍网格的 ")#raw("1×2")#text(" 多米诺骨牌铺满方案数。")
#strong[#text("复杂度")]#text("：时间最坏 ")#raw("O(n 3^m)")#text("，空间 ")#raw("O(2^m)")#text("，代码自动令 ")#raw("m")#text(" 为较短边。")
#strong[#text("下标")]#text("：网格 ")#raw("0-indexed")
#strong[#text("限制")]#text("：")#raw("'#'")#text(" 为障碍，其余位置可放置骨牌。")
#strong[#text("注意")]#text("：方案数可能超过 ")#raw("long long")#text("，需要取模时修改累加位置。")





=== #text("现场记忆")

#raw("mask：本行已被上一行竖骨牌占掉的位置\nnxt：本行向下一行伸出的竖骨牌\n\n当前位置空：\n横放 -> 一次吃掉两列\n竖放 -> nxt 对应位设 1\n\n一行结束 dp = ndp\n最终必须回到 mask = 0",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
assert(dominoTilings({"..", ".."}) == 2 && dominoTilings({}) == 1);
```

=== 模板引用

#source_code(("dp/domino_tilings.hpp",))


== #text("插头 DP（网格 Hamilton 单回路）") <algo-dp-plug-hamilton-cycle>

#text("原稿来源：10.8 插头 DP（网格 Hamilton 单回路）")


#strong[#text("用途")]#text("：统计带障碍网格中经过所有可走格恰好一次的 Hamilton 回路；标准括号插头 DP。")
#strong[#text("复杂度")]#text("：设可达插头状态数为 ")#raw("S")#text("，时间最坏 ")#raw("O(nm^2S)")#text("，空间 ")#raw("O(S)")
#strong[#text("下标")]#text("：网格 ")#raw("0-indexed")
#strong[#text("限制")]#text("：")#raw("'#'")#text(" 为障碍；状态使用 64 位编码，较短边需 ")#raw("<= 31")#text("，实际通常只适合约 ")#raw("12 ~ 15")#text("。")
#strong[#text("注意")]#text("：代码自动令较短边为状态宽度；答案可能溢出 ")#raw("long long")#text("。")





=== #text("现场记忆")

#raw("每格读：\nL = slot[j]，U = slot[j+1]\n\n处理后写：\nD = slot[j]，R = slot[j+1]\n\n00 -> 建 D,R，形成 ()\n单插头 -> 延伸到 D 或 R\n11 -> 向右找匹配 2，把它改成 1\n22 -> 向左找匹配 1，把它改成 2\n21 -> 直接合并\n12 -> 闭环，只允许最后一个可走格且无其他插头\n\n每行结束：state <<= 2",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
assert(plugHamiltonCycle({"..", ".."}) == 1 &&
       plugHamiltonCycle({"."}) == 0);
```

=== 模板引用

#source_code(("dp/plug_hamilton_cycle.hpp",))


== #text("SOS DP（子集 / 超集和）") <algo-dp-sos-dp>

#text("原稿来源：10.9 SOS DP（子集 / 超集和）")


#strong[#text("用途")]#text("：对所有 ")#raw("mask")#text(" 同时求其所有子集或所有超集的权值和，并支持 Möbius 逆变换。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n2^n)")#text("，空间 ")#raw("O(2^n)")
#strong[#text("下标")]#text("：二进制位 ")#raw("0 ~ n-1")


=== #text("现场记忆")

#raw("子集和：\nmask 有第 i 位\nf[mask] += f[mask ^ (1<<i)]\n\n超集和：\nmask 没第 i 位\nf[mask] += f[mask | (1<<i)]\n\nMöbius 逆变换：把 += 改成 -=",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
vector<ll> a{1, 2, 3, 4};
sosSubset(a, 2);
assert((a == vector<ll>{1, 3, 4, 10}));
mobiusSubset(a, 2);
assert((a == vector<ll>{1, 2, 3, 4}));
```

=== 模板引用

#source_code(("dp/sos_dp.hpp",))


== #text("单调队列优化 DP") <algo-dp-monotone-queue-dp>

#text("原稿来源：10.10 单调队列优化 DP")


#strong[#text("用途")]#text("：优化形如 ")#raw("dp[i] = A[i] + min/max F(j)")#text("，且合法决策 ")#raw("j")#text(" 位于滑动窗口中的转移。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n)")#text("，空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：")#raw("1-indexed")
#strong[#text("限制")]#text("：代码对应 ")#raw("dp[i] = a[i] + min(dp[j])")#text("，其中 ")#raw("i-k <= j < i")#text("，")#raw("k >= 1")#text("。")


=== #text("现场记忆")

#raw("先把转移整理成：dp[i] = A[i] + min/max F(j)\n\n队首：当前最优决策\n队尾：删掉以后永远不可能更优的决策\n\n过期：j < i-k -> pop_front()\n维护最小值：F(back) >= F(i) -> pop_back()\n维护最大值时反转比较符号",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
assert(monotone_queue_dp(vector<ll>{0, 2, 3, 1}, 2)[3] == 3);
```

=== 模板引用

#source_code(("dp/monotone_queue_dp.hpp",))


== #text("斜率优化 CHT") <algo-dp-cht-min>

#text("原稿来源：10.11 斜率优化 CHT")


#strong[#text("用途")]#text("：优化形如 ")#raw("dp[i] = A[i] + min_j(k[j] * x[i] + b[j])")#text(" 的线性转移。")
#strong[#text("复杂度")]#text("：每条直线均摊 ")#raw("O(1)")#text(" 加入、查询，总时间 ")#raw("O(n)")#text("，空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：按原 DP 自然决定")
#strong[#text("限制")]#text("：直线斜率 ")#raw("k")#text(" 按加入顺序单调不增，查询 ")#raw("x")#text(" 单调不减。")
#strong[#text("注意")]#text("：乘法和交点判断使用 ")#raw("__int128")#text(" 防溢出；若斜率或查询顺序不单调，改用 Li Chao Tree。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("add(k, b)")],[#text("加入直线 ")#raw("y = kx + b")],
[#raw("query(x)")],[#text("查询所有直线在 ")#raw("x")#text(" 处的最小值")],
)])



=== #text("现场记忆")

#raw("先把 DP 化成：\ndp[i] = A[i] + min(k[j] * x[i] + b[j])\n\n一个决策 j -> 一条直线 y = kx + b。\n\n斜率单调 -> 插入时弹掉无用直线。\nx 单调 -> 查询时不断弹队首。\n\n三线 a,b,c：\nintersection(a,b) >= intersection(b,c) -> b 永远无用。\n\n非单调查询 / 非单调斜率 -> Li Chao Tree。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
CHTMin c;
c.add(2, 1);
c.add(-1, 3);
assert(c.query(4) == -1);
```

=== 模板引用

#source_code(("dp/cht_min.hpp",))


== #text("分治优化 DP") <algo-dp-divide-conquer-dp>

#text("原稿来源：10.12 分治优化 DP")


#strong[#text("用途")]#text("：优化分段 DP：")#raw("dp[t][i] = min_{j<i}(dp[t-1][j] + cost(j+1,i))")#text("。")
#strong[#text("复杂度")]#text("：若 ")#raw("cost")#text(" 为 ")#raw("O(1)")#text("，时间通常由 ")#raw("O(Kn^2)")#text(" 优化到 ")#raw("O(Kn log n)")#text("，空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：位置 ")#raw("1 ~ n")#text("，前驱位置 ")#raw("0 ~ n-1")
#strong[#text("限制")]#text("：必须满足最优决策单调：")#raw("opt[t][i] <= opt[t][i+1]")#text("。")
#strong[#text("注意")]#text("：决策单调性不是代码自动保证的，必须由题目性质 / Monge 性质 / 四边形不等式等证明。")





=== #text("现场记忆")

#raw("原转移：\ndp[t][i] = min(dp[t-1][j] + cost(j+1,i))\n\n核心条件：\nopt[t][i] <= opt[t][i+1]\n\n算 mid 时只搜 [optL, optR]。\n得到最优 bestPos 后：\n\n左半边决策范围  [optL, bestPos]\n右半边决策范围  [bestPos, optR]\n\n不要看到“分段 DP”就直接套，先确认决策单调。",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("输入 n>=0、K>=0，cost(l,r) 为 1-indexed 非空区间。返回恰好 K 段的 dp[0..n]，不可达 INF=LLONG_MAX/4；有限状态和代价须绝对值显著小于 INF。只在能证明 argmin 单调时使用；全不可达时不再向递归传 -1。")

=== 最小调用示例

```cpp
auto d = divideConquerDP(4, 2,
                         [](int l, int r) -> ll
                         { return 1LL * (r - l + 1) * (r - l + 1); });
assert(d[4] == 8);
```

=== 模板引用

#source_code(("dp/divide_conquer_dp.hpp",))


== #text("Knuth 优化（区间 DP）") <algo-dp-knuth-merge>

#text("原稿来源：10.13 Knuth 优化（区间 DP）")


#strong[#text("用途")]#text("：将满足 Knuth 决策单调条件的区间 DP 从 ")#raw("O(n^3)")#text(" 优化到 ")#raw("O(n^2)")#text("；代码对应相邻石子合并最小代价。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n^2)")#text("，空间 ")#raw("O(n^2)")
#strong[#text("下标")]#text("：")#raw("1-indexed")
#strong[#text("限制")]#text("：经典石子合并版本要求权值非负。")
#strong[#text("注意")]#text("：必须满足 ")#raw("opt[l][r-1] <= opt[l][r] <= opt[l+1][r]")#text("，不能只因为形式像区间 DP 就套。")


=== #text("现场记忆")

#raw("普通区间 DP：枚举 l,r，再枚举全部 k -> O(n^3)\n\nKnuth 只搜：\nopt[l][r-1] <= k <= opt[l+1][r]\n\n即：\nopt[l][r-1] <= opt[l][r] <= opt[l+1][r]\n\n石子合并：\ndp[l][r] = min(dp[l][k] + dp[k+1][r]) + sum(l,r)",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
assert(knuth_merge(vector<ll>{0, 1, 2, 3}) == 9);
```

=== 模板引用

#source_code(("dp/knuth_merge.hpp",))


== #text("Li Chao Tree 优化 DP") <algo-dp-li-chao-min>

#text("原稿来源：10.14 Li Chao Tree 优化 DP")


#strong[#text("用途")]#text("：维护若干直线 ")#raw("y = kx + b")#text(" 的最小值，处理斜率、查询 ")#raw("x")#text(" 均不保证单调的线性 DP。")
#strong[#text("复杂度")]#text("：单次插入 / 查询 ")#raw("O(log |R-L|)")#text("，空间 ")#raw("O(q log |R-L|)")
#strong[#text("下标")]#text("：查询坐标为整数 ")#raw("x ∈ [L,R]")
#strong[#text("限制")]#text("：代码维护最小值；实际答案需能放入 ")#raw("long long")#text("。")
#strong[#text("注意")]#text("：若斜率和查询都满足单调性，优先用单调 CHT，常数更小。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("LiChaoMin(L,R)")],[#text("初始化查询坐标范围")],
[#raw("add(k,b)")],[#text("加入直线 ")#raw("y = kx + b")],
[#raw("query(x)")],[#text("查询所有直线在 ")#raw("x")#text(" 处的最小值")],
)])



=== #text("现场记忆")

#raw("目标先化成：\ndp[i] = A[i] + min(k[j] * x[i] + b[j])\n\nLi Chao：每个线段树节点保留“中点更优”的那条线。\n剩下那条线只可能在左半或右半继续有用。\n\n斜率 / x 不单调 -> Li Chao\n二者单调 -> 优先单调 CHT\n\n求最大值：把 k、b 取反，最后答案再取反。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("整数坐标 L<=x<=R，构造要求 L<=R。空树 query 返回 LLONG_MAX；非空最小值必须在 ll 内。中点使用 i128 差值，覆盖跨越 LLONG_MIN..LLONG_MAX 的域；移除原版 ±2^62 截断，避免合法大答案被改值。多测重新构造对象。")

=== 最小调用示例

```cpp
LiChaoMin t(-10, 10);
t.add(2, 1);
t.add(-1, 3);
assert(t.query(4) == -1);
```

=== 模板引用

#source_code(("dp/li_chao_min.hpp",))


== #text("Bitset 优化 DP（0/1 子集和）") <algo-dp-bitset-subset-sum>

#text("原稿来源：10.15 Bitset 优化 DP（0/1 子集和）")


#strong[#text("用途")]#text("：快速求若干非负整数能组成哪些和，是布尔 0/1 背包的高效写法。")
#strong[#text("复杂度")]#text("：时间约 ")#raw("O(nC / 64)")#text("，空间 ")#raw("O(C)")
#strong[#text("下标")]#text("：")#raw("dp[s] = 1")#text(" 表示和 ")#raw("s")#text(" 可达")
#strong[#text("限制")]#text("：")#raw("MAXC")#text(" 为编译期常量，物品权值非负。")


=== #text("现场记忆")

#raw("普通 0/1 布尔背包：dp[j] |= dp[j-w]\nbitset：dp |= dp << w\n\n左移 w：所有原来能组成的和整体 +w\nMAXC 必须覆盖题目需要的最大和",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("subset_sum<MAXC>(a) 的物品数组为 1-indexed，a[0] 占位；每个权重非负。返回位集 dp[0..MAXC]，dp[0]=1，超过容量的位自动截去；重复调用每次重新初始化，不累计上次结果。")

=== 最小调用示例

```cpp
auto d = subset_sum<100>(vector<int>{0, 2, 3});
assert(d[5] && !d[4] && d[0]);
```

=== 模板引用

#source_code(("dp/bitset_subset_sum.hpp",))


== #text("WQS 二分 / Aliens Trick") <algo-dp-wqs>

#text("原稿来源：10.16 WQS 二分 / Aliens Trick")


#strong[#text("用途")]#text("：将“恰好选 ")#raw("K")#text(" 个 / 分成 ")#raw("K")#text(" 段”的 DP，转化为给每次选择增加惩罚 ")#raw("lambda")#text(" 的一维 DP。")
#strong[#text("复杂度")]#text("：")#raw("O(Tcheck log V)")#text("；代码骨架中 ")#raw("check")#text(" 为 ")#raw("O(n^2)")#text("，总计 ")#raw("O(n^2 log V)")
#strong[#text("下标")]#text("：位置 ")#raw("1 ~ n")
#strong[#text("限制")]#text("：最优代价关于选择次数需满足离散凸性，使选择次数随 ")#raw("lambda")#text(" 单调。")
#strong[#text("注意")]#text("：最小化问题中每段增加 ")#raw("+lambda")#text("；同代价时必须优先选择更多段。")


=== #text("现场记忆")

#raw("原问题：恰好选 K 次\n加入惩罚：new_cost = old_cost + lambda * cnt\n\nlambda 越大 -> 越不愿意多选 -> cnt 单调不增\n\n最小化：\n同 penalized cost 时优先 cnt 更多\n二分最大的 lambda，使 cnt >= K\n\n答案：F(lambda) - lambda*K\ncheck() 仍可继续用 CHT / 分治等优化",block:true)



=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("函数 wqs(n,K,LIM,cost)，位置 1-indexed；n>=1、1<=K<=n、LIM>=0。调用者须证明离散凸性，且 [-LIM,LIM] 覆盖所需惩罚转折。有限 cost 与惩罚累计远离 2^62，恢复答案也须在 ll 内；没有凸性或界不够时结果不保证，必须按题复核。")

=== 最小调用示例

```cpp
assert(wqs(4, 2, 100,
           [](int l, int r) -> ll
           { return 1LL * (r - l + 1) * (r - l + 1); }) == 8);
```

=== 模板引用

#source_code(("dp/wqs.hpp",))


== #text("CDQ 优化三维偏序 DP") <algo-dp-cdq-dominance>

#text("原稿来源：CDQ 优化三维偏序 DP")


#strong[#text("用途")]#text("：求三维偏序下的最长 / 最大权链，转移形如")
#raw("dp[i] = max(dp[i], dp[j] + w[i])")#text("，其中 ")#raw("j")#text(" 在三个维度上均满足对应的 ")#raw("<")#text(" 或 ")#raw("<=")#text("。")

#strong[#text("复杂度")]#text("：时间 ")#raw("O(n log^2 n)")#text("，空间 ")#raw("O(n)")

#strong[#text("下标")]#text("：点 ")#raw("1-indexed")

#strong[#text("注意")]#text("：")
#text("1. ")#raw("STRICT_X / STRICT_Y / STRICT_Z = true")#text(" 表示该维必须严格 ")#raw("<")#text("，")#raw("false")#text(" 表示允许 ")#raw("<=")#text("。")
#text("2. 第一维是否允许取等，影响的是 ")#strong[#text("CDQ 分治边界")]#text("；第二维影响 ")#strong[#text("扫描加入条件")]#text("；第三维影响 ")#strong[#text("BIT 查询位置")]#text("。")
#text("3. 若三个维度全部允许 ")#raw("<=")#text("，完全相同的点也可以依次转移；模板使用 ")#raw("id")#text(" 打破完全相同点的顺序。")



#text("如果是普通最长链，每个点权值直接令：")
#raw("w = 1")#text("。")

#text("例如要求：")

#raw("x[j] <= x[i]\ny[j] <  y[i]\nz[j] <= z[i]",block:true)

#text("则：")
#raw("CDQDominance<false, true, false>")#text("；三个参数依次对应 x、y、z。")

=== #text("三维取等速查")

#figure(placement:top,scope:"parent",align(center)[#table(columns:6,inset:3pt,
table.header([#text("第一维")],[#text("第二维")],[#text("第三维")],[#raw("STRICT_X")],[#raw("STRICT_Y")],[#raw("STRICT_Z")]),
[#raw("<")],[#raw("<")],[#raw("<")],[#raw("true")],[#raw("true")],[#raw("true")],
[#raw("<")],[#raw("<")],[#raw("<=")],[#raw("true")],[#raw("true")],[#raw("false")],
[#raw("<")],[#raw("<=")],[#raw("<")],[#raw("true")],[#raw("false")],[#raw("true")],
[#raw("<")],[#raw("<=")],[#raw("<=")],[#raw("true")],[#raw("false")],[#raw("false")],
[#raw("<=")],[#raw("<")],[#raw("<")],[#raw("false")],[#raw("true")],[#raw("true")],
[#raw("<=")],[#raw("<")],[#raw("<=")],[#raw("false")],[#raw("true")],[#raw("false")],
[#raw("<=")],[#raw("<=")],[#raw("<")],[#raw("false")],[#raw("false")],[#raw("true")],
[#raw("<=")],[#raw("<=")],[#raw("<=")],[#raw("false")],[#raw("false")],[#raw("false")],
)])



=== #raw("< / <=")#text(" 到底改哪里")

#text("三个维度分别对应三处，比赛现场只记这三句话。")

=== #text("第一维 ")#raw("x")

#text("要求：")

#raw("x[j] < x[i]",block:true)

#text("则：")
#raw("STRICT_X = true")#text("，即第一个模板参数为 true。")

#text("CDQ 必须保证：")

#raw("左半所有 x < 右半所有 x",block:true)

#text("所以 ")#strong[#text("相同 `x` 不能被分到 CDQ 左右两边")]#text("。")

#text("要求：")

#raw("x[j] <= x[i]",block:true)

#text("则：")
#raw("STRICT_X = false")#text("。")

#text("普通：")
#raw("mid = (l + r) >> 1")

#text("即可，相同 ")#raw("x")#text(" 可以发生左 → 右转移。")


=== #text("第二维 ")#raw("y")

#text("严格：")

#raw("y[j] < y[i]",block:true)

#text("扫描条件：")
#raw("a[L[p]].y < a[v].y")

#text("即：")
#raw("STRICT_Y = true")#text("，即第二个模板参数为 true。")

#text("可取等：")

#raw("y[j] <= y[i]",block:true)

#text("扫描条件：")
#raw("a[L[p]].y <= a[v].y")

#text("即：")
#raw("STRICT_Y = false")#text("。")


=== #text("第三维 ")#raw("z")

#text("严格：")

#raw("z[j] < z[i]",block:true)

#text("只能查询比当前 ")#raw("z")#text(" 小的值：")
#raw("bit.query(rz - 1)")

#text("即：")
#raw("STRICT_Z = true")#text("，即第三个模板参数为 true。")

#text("可取等：")

#raw("z[j] <= z[i]",block:true)

#text("当前 ")#raw("z")#text(" 本身也可以作为前驱：")
#raw("bit.query(rz)")

#text("即：")
#raw("STRICT_Z = false")#text("。")

=== #text("现场记忆")

#raw("三维偏序：\n\n第 1 维 x：\n    <  -> CDQ 不能切开相同 x\n    <= -> 普通 CDQ 分治\n\n第 2 维 y：\n    <  -> 加入 left.y < right.y\n    <= -> 加入 left.y <= right.y\n\n第 3 维 z：\n    <  -> BIT.query(rk - 1)\n    <= -> BIT.query(rk)\n\n记忆：\n第一维看“分治边界”\n第二维看“什么时候加入 BIT”\n第三维看“BIT 查到哪里”",block:true)

=== #text("常见改法")

#text("如果求最长链：")
#raw("a[i].w = 1")

#text("转移就是：")
#raw("dp[i] = max(dp[i], dp[j] + 1)")

#text("如果题目是：")

#raw("x[j] <  x[i]\ny[j] <= y[i]\nz[j] <  z[i]",block:true)

#text("直接：")
#raw("CDQDominance<true, false, true> solver;")

#text("如果题目是：")

#raw("x[j] <= x[i]\ny[j] <= y[i]\nz[j] <= z[i]",block:true)

#text("直接：")
#raw("CDQDominance<false, false, false> solver;")

=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("CDQDominance<SX,SY,SZ> 三个布尔值控制严格小于。Node 输入 1-indexed，至少含 0 号占位；填 x,y,z,w 即可，work 重设 id=i、离散化并重置 BIT，返回非空链最大权，空集约定 0。相同点在全 <= 时按 id 定序，不能在不限定顺序的有环关系上解释成无限转移。权与链和必须大于 LLONG_MIN 且不溢出；NEG 占用 LLONG_MIN。原局部变量 ll 与个人宏冲突，已改为 left。工作状态放入轻量 struct，便于反复调用及八种配置对拍。")

=== 最小调用示例

```cpp
CDQDominance<> s;
assert(s.work(vector<Node>{{}, {1, 1, 1, 3}, {2, 2, 2, 4}}) == 7);
```

=== 模板引用

#source_code(("dp/cdq_dominance.hpp",))

