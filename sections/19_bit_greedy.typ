= 位运算贪心与构造 <chapter-bit-greedy>

#import "../styles/source.typ": source_code

本章收录 12 类贪心 / 位分治模型及 4 类配套性质与构造；符号 ⊕、AND、OR 分别表示按位异或、与、或。每项给出简要证明与独立函数，题目链接用于练习，不把一种模型推广到任意位运算题。数组默认 a[1…n]、a[0] 不用；边表和构造结果不带哨兵，分组内部自然用 0-index。

=== 现场检查

整数非负才可直接按二进制字典序比较。从高位贪心之前，先证明高位优先，再证明“固定高位后的可行方案”仍被完整保留；不能只凭高位权重大就忽略进位、预算和配对约束。加法预算、掩码上界、固定 popcount 分别是不同约束。

源码使用 GNU++17；除特别注明外，数值限制为小于 $2^62$ 的非负 ll，位下标 0…61，统一用 `1LL << b`。最大 AND 加法版初值与预算须小于 $2^61$；图、数组长度须能由 int 表示。平方和、材料总和等中间值另验范围，GNU `__int128` 也有上界。不得对 0 调用 clz / ctz，不把 C++ `^` 的异或当作乘方。函数均用局部状态，可独立多次调用。

已有 #ref(<algo-string-trie01>, supplement: [01 Trie])、#ref(<algo-data-structure-linear-basis>, supplement: [线性基])、#ref(<algo-math-gauss-xor>, supplement: [异或消元]) 按用途引用，不在本章重复打印。

== 高位试填答案：加法预算下的最大 AND <algo-bit-greedy-max-and>

*识别与例题：* 每次给一个数加 1，最多 k 次，最大化全体 AND。#link("https://codeforces.com/problemset/problem/1903/D1")[CF1903D1]。a=[1,2]、k=3，可变 [3,3]，答案 3。

*维护：* 高位到低位尝试置 1；当前位已经为 1 的数不变，为 0 的数加到最近该位为 1 的数，清掉其低位。总增量≤剩余预算才提交这批变化。数组按值传入，每个查询从原数组重新开始，不共享上次修改结果。

*简证：* 对当前数，该次向上取整是满足新增位的最小可达值，且不会改动已确定的更高位。因此当前数组逐项保留了满足已选位的最小代价状态。若本位可行，保留它优于任何低位组合；若不可行，增加更多低位限制也无法使其可行。

*接口 / 边界：* max_and_increment(a,k) 返回最大值，$O(n B)$ 时间、$O(n)$ 额外空间，B=62；这里只是 D1 的单次查询模型，不能声称支持 D2 的全部规模。逐项扣预算避免求和溢出，不能把加法简化成按位 OR。

*记忆：* 试位、算真实进位代价、可行才改数组。

```cpp
assert(max_and_increment(vector<ll>{0, 1, 2}, 3) == 3);
```

#source_code(("bit_greedy/max_and_increment.hpp",))

== 高位清零答案：最小 OR 生成树 <algo-bit-greedy-min-or-tree>

*识别与例题：* 选生成树，使边权 OR 最小。#link("https://codeforces.com/problemset/problem/1624/G")[CF1624G]。三角形边权 1、2、4，答案为 3。

*维护与简证：* 初始允许位为所有边权 OR。逐位尝试禁止该位，只保留权值不含任何禁止位的边；DSU 判断能否连通。可连通即存在一棵符合限制的生成树，可安全禁位；不连通说明任何生成树都必用至少一条被删边，当前已定高位下不能再清此位。每次判定都重建 DSU，不能复用旧合并。

*代码依赖：* #link(<shared-graph-dsu>)[DSU 并查集骨架]，先复制该实现，再复制本函数。

*接口 / 边界：* min_or_tree(n,edges)，点 1…n，边表为 (u,v,w)，不带哨兵；断图返回 −1，n=1 返回 0。$O(B(m alpha(n)+n))$ 时间、$O(n)$ 额外空间，允许自环、重边。*记忆：* 禁位后看整体连通，不是直接按普通边权跑 MST。

```cpp
vector<tuple<int, int, ll>> e = {{1, 2, 1}, {2, 3, 2}, {1, 3, 4}};
assert(min_or_tree(3, e) == 3);
```

#source_code(("bit_greedy/min_or_tree.hpp",))

== 二次幂拆分：低位需求与向上结余 <algo-bit-greedy-split-powers>

*识别与例题：* 材料大小为二次幂，每次可一分为二，选部分材料凑目标，最少拆分次数。#link("https://codeforces.com/problemset/problem/1303/D")[CF1303D]。目标 10，材料 1、1、32，拆两次获得 8 即可。

*维护：* cnt[b] 记大小 $2^b$ 的数量，从低位处理目标；需求位为 1 却没有该材料时，从最近有货的高位逐层拆下来。消耗需求后，每两个剩余低位材料可作为下一位的一份等价总量，向上传递；这只是记账，不计真实合并操作。

*简证：* 较大材料不能直接满足缺失的低位，必须至少拆到该位；选最近高位所需层数最少。更低位已处理，多余的小材料只通过总量影响未来，成对进位不丢可行性。拆更高的材料替代最近材料会增加拆分层数，而产生的结余可由所保留的最近材料及更低结余等价替代，不能改善最优值。

*接口 / 边界：* split_powers(a,target) 返回次数，无解 −1；材料为 1-index，允许空材料配目标 0。输入材料≤$2^60$，目标小于 $2^61$。$O(n+B^2)$ 时间、$O(B)$ 空间；材料总和用 128 位，不能要求所有材料都用完。*记忆：* 低位缺口先填，最近高位拆开，剩余两份再向上算。

```cpp
assert(split_powers(vector<ll>{0, 1, 1, 32}, 10) == 2);
```

#source_code(("bit_greedy/split_powers.hpp",))

== 位计数集中：AND / OR 操作最大平方和 <algo-bit-greedy-concentrate>

*识别与例题：* 可把一对数替换为它们的 AND、OR，最大化平方和。#link("https://codeforces.com/problemset/problem/1368/D")[CF1368D]。1、3、5 可变成 7、1、1，平方和为 51。

*维护：* 每一位的 1 的总数不变。把第 b 位分配给前 cnt[b] 个数，形成按位包含的嵌套结果。函数返回这些数；按题目范围再计算平方和，不在通用函数中暗中取模。

*简证：* 两数可写为 c+p、c+q，c 是共有位，p、q 是各自独有位。操作变为 c、c+p+q，平方和增加 $2p q>=0$。反复让一个数接收 OR，可把所有尚存位集中到它，再处理剩余数，最终总能到达同一嵌套结果且每步不减，故该结果全局最优。

*接口 / 边界：* concentrate_bits(a) 返回 1-index 数组；$O(n B)$ 时间、$O(n)$ 空间。平方前先提升类型；一般 62 位数的平方和未必能由 128 位保存。*记忆：* 先找位计数不变量，再用凸性证明集中更好；不能对任意目标函数都集中。

```cpp
auto a = concentrate_bits(vector<ll>{0, 1, 3, 5});
assert(a == vector<ll>({0, 7, 1, 1}));
```

#source_code(("bit_greedy/concentrate_bits.hpp",))

== 统一异或：按位最小化逆序对 <algo-bit-greedy-xor-inversions>

*识别与例题：* 所有元素异或同一掩码，使逆序数最小，平局取最小掩码。#link("https://codeforces.com/problemset/problem/1416/C")[CF1416C]。a=[1,0]，掩码 1 可消除唯一逆序对。

*维护：* 在相同高位前缀组内，统计当前位的“1 在 0 前”数量与“0 在 1 前”数量，分别是掩码本位为 0 / 1 的代价；再稳定分成零组、一组处理低位。把所有组的同位代价相加后统一选更小的一侧，平局选 0。

*简证：* 两数大小仅由最高不同位决定，低位无法改变这一对是否逆序；高位相同的数对才继续分组。因此每对恰在一位计费，各掩码位独立决定其负责数对的方向。逐位取较小代价得到总最小，平局为 0 得最小掩码。

*接口 / 边界：* xor_inversions(a) 返回 (逆序数,掩码)，$O(n B)$ 时间、$O(n+B)$ 空间；逆序数用 ll，重复值不算逆序。不能在统计前将原数组整体排序。*记忆：* 保序分组，数对只向最高不同位交一次费用。

```cpp
auto [inv, mask] = xor_inversions(vector<ll>{0, 1, 0});
assert(inv == 0 && mask == 1);
```

#source_code(("bit_greedy/xor_inversions.hpp",))

== 两组匹配：最大化异或结果的 AND <algo-bit-greedy-pair-and>

*识别与例题：* 任意重排 B，最大化所有 $A_i ⊕ B_i$ 的 AND。#link("https://codeforces.com/problemset/problem/1721/D")[CF1721D]。A=[0,1]、B=[2,3]，交叉配对后两项均为 3。

*维护：* 初始一个配对组；每个组中的 A、B 可任意互配，但不能跨组。试本位为 1 时，组内必须满足 A 的零数=B 的一数；成立后细分为 A0/B1、A1/B0 两组。不成立就保持旧组不变、跳过本位。所有组同时通过才提交。

*简证：* 本位异或为 1 必须配不同位，数量相等是必要条件。组内现阶段没有其他限制，分别任意配对即充分。细分完整保留所有满足已选高位的配对，后续无法推翻此前决定，故可以从高位贪心。

*接口 / 边界：* max_pair_and(a,b) 返回最优值，数组等长、非空、1-index；$O(n B)$ 时间、$O(n)$ 峰值空间。不同组不能重新混合，否则破坏高位约束；这里只返回值，恢复排列可在组内保留原下标。*记忆：* 不是全局零一数量相等，而是每个旧组都必须相等。

```cpp
assert(max_pair_and(vector<ll>{0, 0, 1}, vector<ll>{0, 2, 3}) == 3);
```

#source_code(("bit_greedy/max_pair_and.hpp",))

== 最小异或数对：排序后只看相邻 <algo-bit-greedy-min-pair>

*识别与例题：* 找两不同位置使 XOR 最小；#link("https://codeforces.com/problemset/problem/1851/F")[CF1851F] 可先化成此问题。对 k 位数 a、b，选择掩码后两个异或结果的 AND 最大值为 $(2^k-1)-(a ⊕ b)$：相同位可同变 1，不同位必有一个 0。

*简证：* 设排序后 $u<v$、其间有 w，u、v 的最高不同位为 b。w 与它们共享更高前缀，在第 b 位必和一端相同，故与该端的 XOR 小于 $2^b$，而 u⊕v 至少为 $2^b$。因此非相邻异值对不会优于所有相邻对；重复值的最优 0 也能由相邻位置取得。

*接口 / 边界：* min_xor_pair(a) 返回原下标，n≥2，$O(n log n)$ 时间、$O(n)$ 空间。CF1851F 得下标后可取 `x = (~a[i]) & ((1LL << k) - 1)`；必须限制到原题 k 位。*记忆：* 这是 XOR 最小，不是 XOR 最大；数值排序相邻不能用来求最大 XOR。

```cpp
vector<ll> a = {0, 1, 2, 3};
auto [i, j] = min_xor_pair(a);
assert(i != j && (a[i] ^ a[j]) == 1);
```

#source_code(("bit_greedy/min_xor_pair.hpp",))

== 有界掩码：异或后差值最小 <algo-bit-greedy-distance>

*识别与例题：* $0<=x<=r$，最小化 $abs((a ⊕ x)-(b ⊕ x))$。#link("https://codeforces.com/problemset/problem/1918/C")[CF1918C]。a=0、b=3、r=2，取 x=1，差为 1。

*维护与简证：* 先令 a≥b。相同位不影响差且消耗掩码额度，全部选 0。存在最优解不翻最高不同位：若翻了，把所有不同位上的掩码取反会只反转差的符号，且得到更小的掩码，仍满足上界。此后差始终非负；低位中只有 a=1、b=0 的位值得翻，耗费 $2^i$、减少差 $2^(i+1)$。这些收益 / 费用同倍，且某位大于所有低位费用之和，因此从高到低能放就放最优。

*接口 / 边界：* xor_distance(a,b,r) 返回 (最小差,一种掩码)，$O(B)$ 时间、$O(1)$ 空间；a=b 返回 (0,0)。不需要枚举 r 的所有子集，也不能不看上界就翻全部低位。*记忆：* 最高不同位定符号，只花预算去削弱它下面的差。

```cpp
auto [gap, mask] = xor_distance(0, 3, 2);
assert(gap == 1 && mask <= 2 && abs((0 ^ mask) - (3 ^ mask)) == gap);
```

#source_code(("bit_greedy/xor_distance.hpp",))

== 位分治：最小化最大异或值 <algo-bit-greedy-minimax>

*识别与例题：* 选一个掩码 x，使所有 $A_i ⊕ x$ 的最大值尽量小。#link("https://codeforces.com/problemset/problem/1285/D")[CF1285D]。a=[1,2,3]，可取 x=3，最大异或为 2。

*转移与简证：* 当前位若全 0 或全 1，掩码同位照抄即可使本位全 0。若两组均有，则最大值这一位必为 1：x 位选 0 时，最大值只由原 1 组的低位决定；选 1 时只由原 0 组决定。另一组最高位为 0，无论低位怎样都更小。因此结果为 $2^b+min(F_0,F_1)$，还原对应分支即可。

*接口 / 边界：* minimax_xor(a) 返回 (最小最大值,一种掩码)，空数组 {0} 约定 (0,0)。排序加分治 $O(n log n+n B)$ 时间，空间 $O(n+B)$；递归深度≤62。和“选一个现有元素”作掩码不同，不能只枚举原数组。*记忆：* 高位分出赢家组，低位只需优化会成为最大值的那一组。

```cpp
auto [val, mask] = minimax_xor(vector<ll>{0, 1, 2, 3});
assert(val == 2 && max({1 ^ mask, 2 ^ mask, 3 ^ mask}) == val);
```

#source_code(("bit_greedy/minimax_xor.hpp",))

== 和与异或：最少元素构造 <algo-bit-greedy-sum-xor>

*识别与例题：* 构造最短正整数数组，元素和为 s、异或为 u，允许空数组表示零。#link("https://codeforces.com/problemset/problem/1325/D")[CF1325D]。u=2、s=4，可取 1、3。

*构造与简证：* 任意非负数的和至少为异或，二者奇偶相同，故 u>s 或 s−u 为奇数时无解。u=s 时至多一个元素。否则令 d=(s−u)/2。若能用两个数，其 AND 必为 d、XOR 必为 u，而同一位不能同时属于 AND、XOR，所以 d AND u=0 必要；该条件成立时 d、d+u 即充分。不成立则两个数不可能，而 u、d、d 总能实现，恰好最少三个。

*接口 / 边界：* sum_xor_construct(u,s) 返回不带哨兵的数组，无解为 {-1}；u=s=0 返回空数组。时间空间 $O(1)$，u、s 均非负 ll。*记忆：* 先排除大小与奇偶，再问 AND 和 XOR 是否占了同一个位。

```cpp
auto a = sum_xor_construct(2, 4);
assert(a.size() == 2 && a[0] + a[1] == 4 && (a[0] ^ a[1]) == 2);
```

#source_code(("bit_greedy/sum_xor_construct.hpp",))

== 独有位贡献：重排的首元素 <algo-bit-greedy-unique>

*识别与例题：* 定义 f(x,y)=(x OR y)−y，连续折叠数组，重排后最大化结果。#link("https://codeforces.com/problemset/problem/1299/A")[CF1299A]。a=[4,0,11,6]，把 11 放首位可得到 9。

*简证：* x OR y 等于 y 加上 x 中而 y 没有的位，这两部分无重叠，故 f(x,y)=x AND NOT y。连续折叠后，仅保留首元素拥有、所有其他元素均没有的位。因此统计各位出现次数，只给次数为 1 的位计算首元素得分；选该得分最大的元素置首，其余顺序任意。原数最大不一定得分最大。

*接口 / 边界：* unique_bits(a) 返回首元素原下标，$O(n B)$ 时间、$O(B)$ 空间，非空数组；全无独有位时任一位置均可，函数取最前位置。重复数必须按出现次数计，不能先去重。*记忆：* 留到最后的是“我独有”，不是“我拥有”。

```cpp
assert(unique_bits(vector<ll>{0, 4, 0, 11, 6}) == 3);
```

#source_code(("bit_greedy/unique_bits.hpp",))

== 包含关系补位：字典序最小掩码数组 <algo-bit-greedy-growing>

*识别与例题：* 给定 a，求字典序最小 y，使 $a_i ⊕ y_i$ 的 1 集合包含前一项。#link("https://codeforces.com/problemset/problem/1547/D")[CF1547D]。a=[1,2,4]，掩码为 [0,1,3]，变成 [1,3,7]。

*维护与简证：* pre 为此前变换结果；它已有而 a[i] 缺少的每一位，y[i] 必须取 1；其余位置取 0 就能得到当前最小 y[i]。结果为 pre OR a[i]，以后仍可同样补位，当前最小选择永不导致未来无解，故按序贪心就是全局字典序最小。

*接口 / 边界：* growing_mask(a) 返回同长度 1-index 数组，$O(n)$ 时间、$O(n)$ 输出空间；首项 y=0，允许空数组 {0}。只要求通常数值非降不能用本式，因为数值非降不等于按位包含。*记忆：* 只补以前必保留而当前缺少的位。

```cpp
assert(growing_mask(vector<ll>{0, 1, 2, 4}) == vector<ll>({0, 0, 1, 3}));
```

#source_code(("bit_greedy/growing_mask.hpp",))

=== 配套性质与构造

== 区间最大 XOR：最高不同位 <algo-bit-greedy-range-xor>

*识别与例题：* l≤a≤b≤r，最大化 a⊕b。#link("https://codeforces.com/problemset/problem/276/D")[CF276D]。区间 [8,13] 内 10⊕13=7。

*简证：* l=r 时答案 0。否则 l⊕r 的最高位为 b，区间内所有数在更高位都相同，答案至多 $2^(b+1)-1$。在该前缀内，第 b 位从 0 切换为 1 的边界 t 满足 $l<=t-1<t<=r$，选 t−1、t，其低 b+1 位全部不同，达到上界。

*接口 / 边界：* range_max_xor(l,r)，闭区间、小于 $2^62$；$O(B)$ 时间、$O(1)$ 空间。允许取相同数；若题目要求不同数，单元素区间应另判无解。*记忆：* 找端点第一次分叉，分叉以下能补满。

```cpp
assert(range_max_xor(8, 13) == 7 && range_max_xor(5, 5) == 0);
```

#source_code(("bit_greedy/range_xor.hpp",))

== 连续区间 AND / OR：公共前缀与端点反推 <algo-bit-greedy-range-and-or>

*识别与例题：* 求连续整数区间的 AND、OR，或固定 n、目标 x 求最小 m 使 [n,m] 的 AND=x。#link("https://codeforces.com/problemset/problem/1775/C")[CF1775C]。n=10、x=8，最小 m=12。

*简证：* 端点的公共高位前缀在整个区间内不变；其下存在 t−1、t 的二次幂边界，使所有低位在 AND 中清零、OR 中填 1。反推端点时，n 为 0 的位不可能新变 1；每个需要清掉的第 b 位，m 至少达到下一个 $2^(b+1)$ 的倍数。取所有下界的最大值是最早候选，再计算区间 AND；若它已清掉某个必须保留位，后续只会继续清零，永无解。

*接口 / 边界：* range_and_or(l,r) 返回 (AND,OR)，端点小于 $2^62$；min_and_end(n,x) 要求 n、x 小于 $2^61$，无解 −1。均 $O(B)$ 时间、$O(1)$ 空间。目标仅是 n 的子掩码还不充分，例如 n=10、x=2 无解。*记忆：* 要清的位给下界，要留的位会否决过大的下界。

```cpp
assert(range_and_or(10, 12) == make_pair(8LL, 15LL));
assert(min_and_end(10, 8) == 12 && min_and_end(10, 2) == -1);
```

#source_code(("bit_greedy/range_and_or.hpp",))

== Popcount：恰好 k 个二次幂之和 <algo-bit-greedy-power-terms>

*识别与例题：* 判断非负 S 能否拆成恰好 k 个正二次幂，允许重复；#link("https://codeforces.com/problemset/problem/1225/C")[CF1225C] 先枚举项数 k，再令 S=n−kp。自拟例：S=10 可以有 2 项 8+2、3 项 4+4+2，直到 10 个 1。

*简证：* 二进制表示用 popcount(S) 项，是最少项数，因为任意表示可不断合并相同幂到规范二进制；每项至少为 1，至多 S 项。只要还不是全 1，就可拆开一个≥2 的幂，使项数恰增 1，故两个端点间所有项数均可达：

$ "popcount"(S)<=k<=S. $

*接口 / 边界：* power_terms(S,k) 返回可行性，$O(B)$ 保守时间、$O(1)$ 空间；S=k=0 表示空和。CF1225C 原约束下枚举 k=1…31 即够：S(31) 小于 $2^30$，若 S(31)≥31 就已满足；$p<0$ 时该条件必成立，$p>=0$ 且 $S(31)<31$ 则以后 S 只会不增、k 增大，无法再可行。不将 31 当任意数据范围的通用界。*记忆：* 最少看 1 的个数，最多看能否拆成全 1。

```cpp
assert(power_terms(10, 3) && !power_terms(10, 1));
assert(power_terms(0, 0) && !power_terms(0, 1));
```

#source_code(("bit_greedy/power_terms.hpp",))

== 借位链：固定零一数量构造差的 Popcount <algo-bit-greedy-borrow-chain>

*识别与例题：* 两个二进制数各有 a 个 0、b 个 1，无前导 0，要求较大减较小后的 1 数为 k。#link("https://codeforces.com/problemset/problem/1492/D")[CF1492D]。a=4、b=2、k=3，可取 101000 与 100001，差为 111。

*构造与简证：* k=0 两串相同即可。k>0 必须有 a≥1、b≥2；否则每串已被唯一确定。两串最高 1 相同，抵消后均不到 $2^(n-1)$；差若有 n−1 个 1，只能是 $2^(n-1)-1$，对应一串低位全 1、另一串全 0，与同样的 1 数矛盾，因此 k≤n−2 必要。

以上条件也充分：先让首位同为 1，再在两个相距 k 的位置放互换的 1，其余 b−2 个 1 放相同空位。共同部分相消，差为：

$ 2^r-2^l=sum_(i=l)^(r-1)2^i, quad r-l=k. $

*接口 / 边界：* borrow_chain(a,b,k) 返回不带哨兵的两个二进制字符串，失败为两个空串；a+b≤200000。时间空间 $O(a+b)$，长串不要转 ll 再相减。*记忆：* 不是直接摆 k 个 1，而是移动一个 1，让减法借位拉出 k 个连续 1。

```cpp
auto [u, v] = borrow_chain(4, 2, 3);
assert(u == "110000" && v == "100010");
```

#source_code(("bit_greedy/borrow_chain.hpp",))
