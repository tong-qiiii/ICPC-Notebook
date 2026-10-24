= 线段树专题 <chapter-segment-tree-problems>

本章沿用“题意 → 信息 → 合并 / 作用 → 维护”的题型索引，不重复打印整题代码。来源为 #link("https://atcoder.jp/contests/aalc001/tasks")[AAL Contest 001：Let's use segtree! A–X]；每题附官方题解链接。下标统一为 1-index 闭区间，空区间单独返回单位元；若参考 ACL 的左闭右开接口，将 [l,r] 换为 [l−1,r)。

=== 从题意设计 Info / Tag

先写暴力，再问“左右两段的答案怎样合成”。Info 必须含答案与跨界所需信息；合并满足结合律，但不必交换。空段是单位元，不是随便填 0。懒标记要同时满足：能够快速作用于 Info，且一串操作能压成定长 Tag。约定合并 L、R 表示先读 L 再读 R；标记“旧 t 后新 u”表示 $u compose t$，不要与模板接口的传参顺序混淆。

*选型：* 单点改用 #ref(<algo-data-structure-segment-tree-point>, supplement: [Info 线段树])，区间作用用 #ref(<algo-data-structure-segment-tree-lazy>, supplement: [InfoTag 线段树])；简单求和可改 #ref(<algo-data-structure-zkw-segment-tree-point>, supplement: [zkw])。区间 chmin / chmax 且普通 Tag 不封闭，才考虑 #ref(<algo-data-structure-segment-tree-beats>, supplement: [Beats])。本章提到树上找边界时，要在现有模板中按给出的单调判定实现一次下降，不把二分 + 区间查询的 $O(log^2 n)$ 当成 $O(log n)$。

*现场顺序：* 叶子如何初始化 → 空段单位元 → 左右合并 → Tag 作用 → Tag 复合 → 查询顺序 → 清空多测。ll 保存计数 / 路程 / 最优值；矩阵不可达为正 / 负无穷，跳过不可达项再相加。模数题统一按 $998244353$，本章不将取模答案用于大小比较。

=== 基础、信息合并与懒标记

== AAL A — Segment Tree：单点赋值、区间最小 <algo-seg-problem-aalc-a>

*题意：* 单点把 $A_i$ 改成 x，询问 [l,r] 最小值。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_a")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27175")[题解]

*信息与调用：* Info 只有 mn，合并取 min，空段为正无穷；叶子 mn=$A_i$。点改后逐层重算，区间查询合并被覆盖节点；无需 Tag。

*维护：* 建树 $O(n)$，修改 / 查询 $O(log n)$，空间 $O(n)$。赋值不能写成加法；负值场景单位元也不能为 0。*记忆：* 先让空段不影响任何真实答案。

== AAL B — Lazy Segment Tree：区间加、区间最小 <algo-seg-problem-aalc-b>

*题意：* 给 [l,r] 每项加 x，查询区间最小。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_b")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27176")[题解]

*信息与作用：* Info=mn，Tag=add；作用为 mn+=add，旧、新 Tag 相加。完整覆盖只更新当前节点与标记，部分覆盖先下传再递归，返回时合并。

*维护：* 单次 $O(log n)$、空间 $O(n)$。空段不应真的给哨兵加数；多测清空 Tag，范围较大用 ll。*记忆：* 区间最小整体平移，维护最小值就够了。

== AAL C — Binary Search on a Segment Tree：树上找边界 <algo-seg-problem-aalc-c>

*题意：* 非负数组支持点赋值；从 l 向右找第一个区间和至少 t 的 r，或从 r 向左找第一个和至少 t 的 l；无解分别为 n+1、0，t≥1。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_c")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27177")[题解]

*信息与搜索：* Info=sum，单位元 0。带累积和 s 按方向遍历；完整节点若 $s+"sum"<t$，整体跳过并累加；否则下降。向右先左子树，向左先右子树。存在懒标记时下降前 push。

*维护：* 点改 / 搜索 $O(log n)$。搜索条件依赖非负；有负数时前缀和可能回落，不能直接用 sum 剪枝。*记忆：* 树上二分是“一次下降”，不是外层二分套查询。

== AAL D — Inversion Count：小字符集逆序对 <algo-seg-problem-aalc-d>

*题意：* 元素仅 0、1、2，点赋值，询问区间逆序对。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_d")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27172")[题解]

*信息与合并：* Info 为逆序数 v 和各值计数 $c_0,c_1,c_2$；计数相加，跨界贡献是左大右小：

$ v=v_L+v_R+c_(L,1)c_(R,0)+c_(L,2)(c_(R,0)+c_(R,1)). $

*维护：* 叶子仅对应计数为 1；空段全 0。点改 / 查询 $O(log n)$、空间 $O(n)$，v 用 ll。查询必须按原顺序合并。*记忆：* 小值域把“跨界配对”枚举进 Info；不是只存区间总逆序数。

== AAL E — RGB：带权固定子序列 <algo-seg-problem-aalc-e>

*题意：* 固定 RGB 字符串，点修改权值；区间内所有 RGB 子序列的三点权值乘积求和。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_e")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27165")[题解]

*信息与合并：* 存 R、G、B、RG、GB、RGB 的带权总和。单字符相加；例如 $"RG"="RG"_L+"RG"_R+R_L G_R$，GB 同理。

$ "RGB"="RGB"_L+"RGB"_R+"RG"_L B_R+R_L "GB"_R. $

*维护：* 叶子只有自身字符对应项为权值；其余 0。每次 $O(log n)$，空间 $O(n)$，所有乘加取模。*记忆：* 在模式串的每个切口，把左边前缀与右边后缀相乘；重复字符模式也按位置定义状态。

== AAL F — Run：最长同值连续段 <algo-seg-problem-aalc-f>

*题意：* 点赋值，区间最长连续相同元素段。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_f")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27166")[题解]

*信息与合并：* len、左右端值、最长同值前缀 pre / 后缀 suf、答案 ans。边界相等时，ans 可取 $"suf"_L+"pre"_R$；L 全相同才可把 pre 跨入 R，R 全相同才可延长 suf。边界不同则不跨界。

*维护：* 叶子长度与三种段长均 1；空段 len=0，合并直接返回另一段。单次 $O(log n)$，空间 $O(n)$。*记忆：* 能否延长前后缀取决于“整段都相同”，不是 pre / suf 非零。

== AAL G — Sum of k-th Powers：幂和与区间加 <algo-seg-problem-aalc-g>

*题意：* 区间加，询问区间 k 次幂之和，$1<=k<=4$。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_g")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27189")[题解]

*信息与作用：* 存 0～4 次幂和 $S_j$，$S_0$ 是区间长度。左右逐项相加；加 a 用二项式展开：

$ S'_j=sum_(t=0)^j binom(j,t)a^(j-t)S_t. $

*维护：* Tag 累加。必须读旧数组，或 j 从大到小更新；不能正序原地覆盖低次幂后继续用。$K=4$ 时修改 $O(K^2 log n)$，查询 $O(K log n)$，空间 $O(K n)$。*记忆：* 次数固定且很小，平移后仍封闭在这些幂和里。

== AAL H — Add and Mod：操作函数复合 <algo-seg-problem-aalc-h>

*题意：* 区间从左到右操作 x：$A_i>0$ 时加 $A_i$，$A_i=0$ 时取模 P；支持点改。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_h")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27167")[题解]

*信息与合并：* 无取模段存 $F_a(x)=x+a$；含取模段存 $G_(a,b)(x)=((x+a) mod P)+b$。合并是右函数作用于左函数。四种情况：F 后 F 参数相加；G 后 F 只加尾项 b；F 后 G 把前面加数并入 a；G 后 G 把前者 b 与后者 a 并入第一次取模前的 a，尾项取后者 b。

*维护：* 单次 $O(log n)$、空间 $O(n)$；$F_0$ 为单位操作。a 可模 P，b 不可随意取模，否则区间末尾加数丢失。*记忆：* 不能只保存一个测试输入的输出，要保存整段对任意 x 的函数。

=== 动态 DP、离散化与非交换运算

== AAL I — Subarray：至多两段最大和 <algo-seg-problem-aalc-i>

*题意：* 点赋值，询问区间中至多两个不相交非空连续段的最大总和，可一段也不选。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_i")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27178")[题解]

*状态与转移：* 采用五阶段：0 未开始、1 选第一段、2 间隔、3 选第二段、4 结束。每个元素可让阶段保持或向后跳；终态在 1 / 3 时加当前值，其余阶段加 0。从 0 出发，末尾对所有阶段取 max；越过某阶段表示不使用它。

*维护：* 一项是 5×5 上三角 max-plus 矩阵，左右按顺序相乘，单位矩阵对角 0、其余负无穷；单点重算叶子。单次 $O(5^3 log n)$，可仅枚举非降状态三元组；空间 $O(25 n)$。*记忆：* 连续段变成“只能前进的选取阶段”，全负答案为 0。

== AAL J — Long Binary String：离散化保留长度 <algo-seg-problem-aalc-j>

*题意：* 长度 $10^18$ 的全零串，区间翻转，查询区间 1 的数量。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_j")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27187")[题解]

*信息与作用：* 收集所有修改、查询的 l、r+1 以及全局两端，排序去重。相邻坐标间一个叶子，存实际长度 len 和 ones；翻转 $"ones"= "len"-"ones"$，Tag 异或。初始化 ones=0。

*维护：* 离线压缩后 $O(Q log Q)$ 时间、$O(Q)$ 空间。叶子代表的不是一个字符；坐标、r+1、长度都用 ll。*记忆：* 压的是边界，不是把每块的长度都改成 1。

== AAL K — Multiset：排序后排名加权和 <algo-seg-problem-aalc-k>

*题意：* 多重集插入 / 删除一个数，求排序后“排名×数值”的总和，排名从 1 开始，空集为 0。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_k")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27188")[题解]

*信息与合并：* 按值建树，Info 为个数 c、数值和 s、排名加权和 w。L 在 R 前：

$ c=c_L+c_R, quad s=s_L+s_R, quad w=w_L+w_R+c_L s_R. $

*维护：* 值 a 出现 t 次的叶子初始化为：

$ (c,s,w)=(t,t a,(t(t+1))/2 a). $

离散化所有可能值，每次点改 $O(log(n+Q))$，空间 $O(n+Q)$；c 保留真实整数，s、w 取模，除 2 用逆元或先整除。*记忆：* 右段所有排名统一平移左段长度。

== AAL L — Swap between Arrays：置换标记、最大前缀 <algo-seg-problem-aalc-l>

*题意：* 三数组支持区间交换任意两数组；从 i 起找 A 的区间和第一次大于 0 的右端点。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_l")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27173")[题解]

*信息与合并：* 每数组存 sum、非空最大前缀 pre；$"pre"=max("pre"_L,"sum"_L+"pre"_R)$。Tag 为三个槽位的置换，作用于三组 Info。若新槽 i 取旧槽 p[i]，旧 p 后新 q 的复合为 p[q[i]]。

*搜索：* 携带此前和 s，仅当 $s+"pre">0$ 才可能在当前段找到答案；否则累加 sum 跳过，按左到右下降。无解 n+1。单次 $O(log n)$，空间 $O(n)$。*记忆：* 有负数时总和不单调，但“最大前缀是否越线”可剪枝。

== AAL M — NAND：非结合运算改存函数 <algo-seg-problem-aalc-m>

*题意：* 30 位非负数点改；区间从 $A_l$ 开始，逐项做 NAND。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_m")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27171")[题解]

*信息与合并：* NAND 本身不结合，但单项函数 $g_a(x)=not(x and a)$ 可以复合。存各位输入全 0 / 全 1 时的输出 f0、f1；任意 x 输出为 `((~x & f0) | (x & f1)) & mask`，mask 为低 30 位全 1。L、R 合并是 R(L(x))，单位函数为 (0,mask)。

*维护：* 查询 [l+1,r] 的复合后作用于 $A_l$；单元素直接返回它。单次 $O(log n)$、空间 $O(n)$。*记忆：* 别把第一个数也当操作再执行一次；C++ 的按位取反必须截断到 30 位。

=== 扫描线、历史信息与边界摘要

== AAL N — Times Tables：覆盖次数最小值与权重 <algo-seg-problem-aalc-n>

*题意：* n×n 网格初值为 $i j$，若干矩形置零，求剩余总和。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_n")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27196")[题解]

*扫描与信息：* 扫行 i，矩形在 a 行对列 [b,d] 覆盖次数加 1，在 c+1 行减 1；本行事件处理完再计答案。Info 存覆盖次数最小值 mn，以及达到 mn 的列号之和 w；合并只累加最小值对应的权重，Tag 加法仅改 mn。

*答案与维护：* 根 mn=0 时，本行贡献 i×w，否则为 0。覆盖次数始终非负才可把最小值当作“未覆盖”；权重是列号而不是列数。$O(n+Q log n)$ 时间、$O(n+Q)$ 空间，答案取模。*记忆：* 扫描线 + 区间加，零点计数可换成带权零点和。

== AAL O — Minimum Value：单调栈与历史区间和 <algo-seg-problem-aalc-o>

*题意：* 静态数组，查询 [l,r] 内所有子数组的最小值之和。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_o")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27168")[题解]

*状态与顺序：* 扫右端 r，令 $C_i=min(A_i,dots,A_r)$，$H_i$ 为截至当前各右端的 $C_i$ 累积。单调栈找到最近严格小于 $A_r$ 的位置 p，对 [p+1,r] 赋值 $C_i=A_r$；随后全体活跃位置执行 $H_i+=C_i$，查询答案为 [l,r] 的 H 和。

*信息与标记：* Info 存 C 和、H 和、len。用 $H'=H+u C+v$、$C'=w C+z$ 表示每项作用；后接参数 $(U,V,W,Z)$ 的标记，复合参数为 $(u+U w,v+U z+V,W w,W z+Z)$。赋 C=a 为 (0,0,0,a)，历史累积为 (1,0,1,0)。

*维护：* Info 中常数项乘 len；尚未激活叶子长度设 0，激活时清空旧状态 / Tag。$O((n+Q)log n)$ 时间、$O(n+Q)$ 空间。*记忆：* 先改当前最小值，再记这一步历史，不能只维护当前答案。

== AAL P — Connect the Dots：层叠区间与滑窗哈希 <algo-seg-problem-aalc-p>

*题意：* 圆周端点各不相同；询问编号连续的一组配对能否在圆内互不相交地连线。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_p")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27169")[题解]

*转化与维护：* 各对写成 $a<b$。相交恰为 $a<c<b<d$，合法集合只能包含或分离。双指针求每个 L 的最大合法 R；为每条配对分配独立随机权，加入时对端点坐标 [a,b) 加权，删除做逆操作。在当前窗口已合法的前提下，新 [a,b] 可加入当且仅当坐标 a 与 b−1 的覆盖哈希相同。

*复杂度与限制：* 区间加 / 单点查可用树状数组差分或懒线段树，$O(n log n+Q)$ 时间、$O(n+Q)$ 空间。64 位无符号溢出哈希有碰撞风险，可双哈希；不同端点、合法窗口两个前提不可漏。*记忆：* 比较两端被哪些外层区间覆盖，不是只比较覆盖数量。

== AAL Q — Number of Moves：截断平移与路程 <algo-seg-problem-aalc-q>

*题意：* 指令依次向左 / 右走 $A_i$ 步，越出 [0,M] 的步数不执行、不计路程；点翻方向，查询从 x 执行 [l,r] 的总路程。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_q")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27179")[题解]

*信息：* 一段用 (L,R,a,b) 表示终点 f(x) 与路程 g(x)：

$ f(x)&=min(max(x,L),R)+a, \
  g(x)&=b-max(L-x,0,x-R). $

加 s 的叶子取 $h=min(s,M)$ 后为 (0,M−h,h,h)，减 s 为 (h,M,−h,h)；单位元 (0,M,0,0)。

*合并：* 把右段中间输入区间平移为 $[u,v]=[L_2-a_1,R_2-a_1]$。与 $[L_1,R_1]$ 相交时，新 L、R 取交集，a、b 分别相加；若 $R_1<u$，取 L=R=$R_1$、$a=L_2+a_2-R_1$、$b=b_1+b_2-(u-R_1)$；若 $v<L_1$ 对称取 L=R=$L_1$、$a=R_2+a_2-L_1$、$b=b_1+b_2-(L_1-v)$。

*维护：* 单次 $O(log n)$、空间 $O(n)$；返回 g(x)，各项用 ll。*记忆：* 只维护终点会丢失中途折返的收益，必须同时压缩路程函数。

== AAL R — Connectivity：窄网格边界连通性 <algo-seg-problem-aalc-r>

*题意：* 宽 W≤10 的数字网格，整行修改；查询两格能否只经相同数字、上下左右移动互达。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_r")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27180")[题解]

*信息与合并：* 按行建树，仅保留顶 / 底两行共 2W 个格子的连通分组与数字。合并时开临时 4W 并查集，导入两子的分组，连接相邻边界同数字格，再投影回最外两行；叶子先连接本行相邻同数字格，顶底副本是同一个格。

*查询：* 设目标行 a≤c，分别取 [1,a]、[a,c]、[c,n] 的摘要，合并重合边界格，判断两目标的分组。不能只查 [a,c]，合法路径可能先绕到外面。单次 $O(W alpha(W)log n)$、空间 $O(n W)$。*记忆：* 只有穿过边界的信息影响更大区间；查询端点要作为边界留下。

=== 较难模型：矩阵、整值函数与图论优化

== AAL S — Absolute Value：绝对值拆符号、动态 DP <algo-seg-problem-aalc-s>

*题意：* 点赋值，区间选长度至少 3 的子序列 B，最大化相邻三项 $abs(B_i+2B_(i+1)+3B_(i+2))$ 的和。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_s")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27181")[题解]

*状态与转移：* 每个绝对值独立选符号 $s_i in {-1,1}$，选中第 i 项的系数为 $s_i+2s_(i-1)+3s_(i-2)$。共 10 状态：未开始 1、只选第一项并定首符号 2、记末两符号 4、倒数第二项 2、结束 1。常规选数时枚举新符号；收尾两项不再开新符号，依次加 $(2s_(i-1)+3s_(i-2))A$、$3s_(i-1)A$。每项也可跳过而保持状态。

*维护：* 用 10×10 max-plus 矩阵；首向量仅未开始为 0，答案只取结束，保证至少三项。建树 $O(10^3 n)$，点改 $O(10^3 log n)$；查询可让向量依次乘各覆盖节点，降为 $O(10^2 log n)$，空间 $O(10^2 n)$。*记忆：* 拆符号后只需记短历史；不能在更新矩阵时原地连选同一元素两次。

== AAL T — K-Inversion Count：插入位置最优值、min-plus 作用 <algo-seg-problem-aalc-t>

*题意：* 对前 m 项插入 x，最小化满足大值−小值≤K 的逆序对数量。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_t")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27197")[题解]

*状态与转移：* 原前缀逆序数用树状数组预处理。对每个待插值 x 存 e=插末尾的新增数、q=任意位置新增数的最小值，初始均 0。新读 v：对 $v<x<=v+K$ 的 q 加 1；对 $v-K<=x<v$ 的 e 加 1；最后全局 $q=min(q,e)$。答案为原逆序数 + q。

*作用与维护：* 对 (e,q) 的三种操作均为 2×2 min-plus 矩阵，复合保持执行顺序。只查叶子，可用仅维护 Tag 的双线段树；离散化查询 x，区间用 lower_bound / upper_bound 严格处理端点。$O((n+Q)log(n+Q))$ 时间、$O(n+Q)$ 空间。*记忆：* 保留“以前最优”与“现在放末尾”两个来源。

== AAL U — Second：次极值贡献矩形、分段 DP <algo-seg-problem-aalc-u>

*题意：* 分割成若干长度至少 4 的段，每段收益为第二大−第二小，最大化总收益。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_u")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27198")[题解]

*贡献：* 相同值按下标打破平局。对 i，设比它优先的大值位置最近两侧为 $k_1<k_2<i<k_4<k_5$。i 是次大恰有两类：$k_1<l<=k_2$ 且 $i<=r<k_4$；或 $k_2<l<=i$ 且 $k_4<=r<k_5$。不存在的位置用 0 / n+1 哨兵。次小同理、贡献取负，总共至多 4n 个 (l,r) 矩形。

*状态与维护：* 扫 r，把矩形在进入 / 离开时化为左端范围加；树上位置 j 表示 $D_j=f_j+"score"(j+1,r)$。$f_0=0$，其余负无穷；先处理事件，再查 j≤r−4 的 max 得 $f_r$，最后令 $D_r=f_r$。有序集合求相邻边界，总 $O(n log n)$、空间 $O(n)$。*记忆：* 次极值看两侧两个优先位置；贡献矩形比直接合并整个段分数更容易。

== AAL V — Connectivity 2：连通块数等于一的历史次数 <algo-seg-problem-aalc-v>

*题意：* 树的点按编号排列，询问 [L,R] 内有多少编号子区间诱导子图连通。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_v")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27182")[题解]

*扫描：* 树的诱导子图是森林，$C_l=r-l+1-"edges"([l,r])$ 为连通块数。加入 r 时旧位置 C 全加 1；每条邻边 $v<r$ 对 [1,v] 减 1；激活新位置 C=1。随后对 C=1 的位置令 D+=1；离线查询 [L,R] 的 D 和。

*信息与 Tag：* 存 C 最小值 mn、其个数 cnt、D 总和。Tag 用 (s,h,b)：C 最终平移 s；所有“若 C=1 则 D++”中，最低累计平移为 h，在该高度发生 b 次，未发生取 h=正无穷、b=0。旧后新合并，比较 $h_1$ 与 $s_1+h_2$，取较小者及其次数，相等则次数相加；s 相加。作用时 mn+h=1 才给 D 和加 b×cnt，再让 mn+=s。

*限制与维护：* 依赖每次统计时活跃位置 C≥1，故更高位置的统计可忽略。未激活位置不能计入 cnt。总边事件 O(n)，时间 $O((n+Q)log n)$、空间 $O(n+Q)$。*记忆：* 先把连通变成点数−边数，再把“当前是否为 1”累积成历史答案。

== AAL W — Adjacent Add：整值部分函数的复合 <algo-seg-problem-aalc-w>

*题意：* 点改；问子数组能否反复给相邻两项分别加 x、2x，使所有项相等，x 可为负整数。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_w")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27190")[题解]

*等价模型：* 设相邻操作总量 $x_i$，边界 $x_0=x_k=0$，目标 C 满足 $B_i+2x_(i-1)+x_i=C$。令 $T_i=C-3x_i$，则：

$ T_(i-1)=f_(B_i)(T_i), quad f_a(t)=(3a-t)/2. $

仅在结果为整数时有定义；问题等价于 $F=f_(B_1) compose dots compose f_(B_k)$ 有整数不动点。

*压缩与维护：* 长 m 的定义域为 $r+2^m z$，输出 $d+(-1)^m z$，存 (m,r,d)。m≤31 时直接复合；m≥32 时，仅保留 $I=[-10^9,2 times 10^9]$ 内唯一的定义域点及输出，或空集，因为区间宽小于 $2^32$，且各 $f_a$ 把 I 映回 I。不动点解 $(2^m-(-1)^m)z=d-r$，检查整除与范围。时间 $O(n+Q log n)$、空间 $O(n)$。

*易错 / 记忆：* 本题合并是左函数复合右函数，与 H 的处理方向不同；先做右侧函数。不能只检验整个有理式值为整数，必须每一步都有定义。负数整除、短长摘要合并需专门对拍，Python 模型不代替 C++ 实现验证。

== AAL X — Unevenness：链上最小费用循环流 <algo-seg-problem-aalc-x>

*题意：* 点改 A，C 可任取整数，$g_i in {1,2}$，最小化下式。#link("https://atcoder.jp/contests/aalc001/tasks/aalc001_x")[原题] · #link("https://atcoder.jp/contests/aalc001/editorial/27191")[题解]

$ sum_i g_i abs(A_i-C_i)+sum_i d_i abs(C_i-C_(i+1)). $

*图模型：* 新建 S；S→i 容量 $g_i$、费用 $-A_i$，i→S 同容量、费用 $A_i$，相邻点双向容量 $d_i$、费用 0。答案是最小费用循环流费用的相反数。改 A 等价于删旧 / 加新单位容量边，因 $g_i<=2$，每次仅常数条。

*维护：* 插边只在含该边的最小费用环为负时增广 1；删已有流量的边，须沿含其反边的最小费用环退流，即使环费用非负。链上环形如 S→i→…→j→S：两棵树维护双向剩余容量的区间加 / 最小值，找首个 0 限定可达范围；另外按 S 边是否可用维护端点 $A_j$ 最小值与 $-A_j$ 最小值。范围极值选环，增广后更新路径容量与两端。

*复杂度与限制：* $O((n+Q)log n)$ 时间、$O(n)$ 空间。需保留正反残量边，区分入边 / 出边的方向，排除无效自环。这里是官方满分思路索引，未新增该动态循环流实现，也未做全规模对拍；不能拿普通最小费用流模板直接声称动态复杂度成立。*记忆：* 线段树加速的是链上的可达范围与选端点，最优性仍来自残量图无负环。

=== 补充经典模型

== 仿射标记：区间加、乘、赋值与区间和 <algo-seg-model-affine>

*识别与例题：* 对区间整体加、乘或赋成常数，再问区间和。#link("https://atcoder.jp/contests/practice2/tasks/practice2_k")[Range Affine Range Sum] 是加、乘的组合；本模型另支持乘数为 0 的赋值。自拟例：[1,2] 先加 3 再乘 2，得到 [8,10]。

*信息与作用：* Info=(sum,len)，Tag=(a,b) 表示 $x'=a x+b$。加 c 为 (1,c)，乘 c 为 (c,0)，赋 c 为 (0,c)，单位元 (1,0)。作用为 $"sum"'=a "sum"+b "len"$；旧 (a,b) 后新 (c,d) 得 (ca,cb+d)。

*调用与边界：* 改 #ref(<algo-data-structure-segment-tree-lazy>, supplement: [InfoTag]) 的作用与复合，区间改后查 sum；不要用 a=0 表示“无标记”。单次 $O(log n)$、空间 $O(n)$。整数版检查乘法范围，模数版负数归一化。*记忆：* 赋值就是斜率为 0 的仿射函数，复合必须按时间顺序。

== 二进制串翻转：最长连续 1 <algo-seg-model-flip-run>

*识别与例题：* 区间 0/1 翻转，询问最长连续 1。#link("https://atcoder.jp/contests/abc322/tasks/abc322_f")[ABC322 F — Vacation Query]。自拟例：00110 的 [1,2] 翻转后为 11110，答案 4。

*信息与合并：* 长度 len；对 b=0、1 各存 pre[b]、suf[b]、best[b]。合并时 best 可跨界取 sufL[b]+preR[b]；仅当左段 pre[b]=lenL 时前缀才可延长，后缀对称。叶子的自身位三个长度为 1，另一位为 0。

*作用与调用：* 翻转交换两套信息，Tag 异或；改 #ref(<algo-data-structure-segment-tree-lazy>, supplement: [InfoTag])，查询 best[1]。空段 len=0 为单位元，单次 $O(log n)$、空间 $O(n)$。*记忆：* 为了修改后仍知道答案，要把当前看似无用的 0 段也存下来。

== 二进制串翻转：区间逆序对 <algo-seg-model-flip-inversions>

*识别与例题：* 区间翻转与区间逆序对数。#link("https://atcoder.jp/contests/practice2/tasks/practice2_l")[Practice2 L]。自拟例：1100 有 4 个逆序对，翻转后 0011 有 0 个。

*信息与作用：* 存零数 z、一数 o、逆序数 v。合并 $v=v_L+v_R+o_L z_R$；翻转后 $v'=z o-v$，再交换 z、o，Tag 异或。理由：不同位的每对位置在翻转前后恰贡献一次，数量为 zo。

*调用与边界：* 接 #ref(<algo-data-structure-segment-tree-lazy>, supplement: [InfoTag])，叶子 v=0，查询 v。单次 $O(log n)$、空间 $O(n)$；v 与 zo 用 ll，合并不可交换左右。*记忆：* 翻转不是把逆序数取负，而是取不同位配对的补集。

== 最大子段和：区间取反与双套极值 <algo-seg-model-max-subarray>

*识别与例题：* 点改 / 区间取反，问区间最大子段和。#link("https://cses.fi/problemset/task/1190/")[CSES Subarray Sum Queries] 是点改且允许空段的基础版；区间取反是这里的扩展。自拟例：[2,−5,3] 最大和为 3，全段取反后最大和为 5。

*信息与合并：* 存 sum，最大 / 最小前缀、后缀、子段和。最大前缀取 max(preL,sumL+preR)，最大后缀对称，最大子段取 max(bestL,bestR,sufL+preR)；最小套把 max 改成 min。取反让 sum 变号，每种最大与最小交换并变号。

*调用与边界：* 点改接 #ref(<algo-data-structure-segment-tree-point>, supplement: [Info])，加取反则接 InfoTag。允许空段时叶子最大项 max(0,x)、最小项 min(0,x)，单位元全 0；非空版另用 len=0 特判空段，不能把全负答案抬到 0。单次 $O(log n)$、空间 $O(n)$。*记忆：* 最大取反变最小，区间加却不能仅靠这七项直接维护。

== 区间加与区间 GCD：差分拆出首项 <algo-seg-model-range-gcd>

*识别与自拟例：* [l,r] 加 x，询问区间所有数的 GCD。[6,10,14] 的 GCD 为 2；全加 2 后 [8,12,16] 为 4，故不能直接把 GCD 加 x。

*转化与维护：* 定义 $d_1=A_1$、$d_i=A_i-A_(i-1)$，则：

$ gcd(A_l,dots,A_r)=gcd(abs(A_l),abs(d_(l+1)),dots,abs(d_r)). $

区间加只点改 d[l]+=x、存在 r+1 时 d[r+1]−=x。树状数组维护 d 的前缀和得到 $A_l$，#ref(<algo-data-structure-segment-tree-point>, supplement: [Info]) 改为 abs-GCD，查 [l+1,r] 后与首项合并。空区间 GCD=0。

*调用与边界：* 两次差分点改后查询；单次 $O(log n)$、空间 $O(n)$。允许负数与 0，但 LLONG_MIN 的 abs 不能放回 ll，须限制范围或用更宽类型。*记忆：* 区间整体平移不改内部差分，唯一变化藏在首项与两端。

== 等差数列区间加：下标的一次函数 <algo-seg-model-arithmetic-add>

*识别与例题：* [l,r] 依次加 K、K+D、…，问点值或区间和。#link("https://www.luogu.com.cn/problem/P1438")[P1438 无聊的数列] 是点查询版。自拟例：[2,4] 加首项 3、公差 2，增量为 3、5、7。

*信息与作用：* 第 i 位增量为 $D i+(K-D l)$。Info 存 sum、len 和固定下标和 idx；Tag=(a,b) 表示加 $a i+b$，作用 sum+=a×idx+b×len，复合逐项相加。叶子 idx=i，合并相加。

*调用与边界：* 接 #ref(<algo-data-structure-segment-tree-lazy>, supplement: [InfoTag])，对 [l,r] 传 (D,K−Dl)，查询 sum；单次 $O(log n)$、空间 $O(n)$。i 必须是原下标，不能每个节点重新从 1 起；压缩坐标时长度与下标权重须用真实值。中间乘法检查 ll。*记忆：* 先把局部首项换成全局下标表达式。

== 扫描线：矩形面积并与周长并 <algo-seg-model-rectangle-union>

*识别与自拟例：* 求轴对齐矩形覆盖的总面积或外边界周长，重叠只算一次。两个并排、共享边的单位正方形，面积 2、周长 6；共享边不能计两次。

*面积维护：* 扫 x，压缩 y 的端点，叶子对应半开线段而非一个点；存覆盖次数 cover 与覆盖长度 len。cover>0 则 len 是整段长度，否则从儿子合并。两相邻 x 之间贡献 len×Δx；源码引用 #ref(<algo-geometry-rectangle-union>, supplement: [矩形面积并])，不重复打印。

*周长扩展：* 再存覆盖段数 cnt、左右端覆盖标志；未被整段覆盖时 $"cnt"="cnt"_L+"cnt"_R-["right"_L and "left"_R]$。横向边界贡献 2×cnt×Δx；同一 x 先处理全部加入，再处理删除，每个事件加 abs(len_after−len_before)。重叠与相邻 y 段正确拼接。

*复杂度与限制：* $O(n log n)$ 时间、$O(n)$ 空间；过滤零面积矩形。现有函数仅实现面积，周长需按上述字段扩展并对拍；整数坐标面积中间值可能需 128 位。*记忆：* 面积看覆盖长度，周长还要看有几段，以及事件前后暴露了多少边。

== Beats：区间限高、限低、加与和 <algo-seg-model-beats>

*识别与自拟例：* 区间 $A_i=min(A_i,x)$ / max、加法，查询和与最值。[2,7,7,4] 限高到 5，仅两个最大值变动，和减少 4。

*信息与作用：* 维护最大值 mx、严格次大 mx2、最大值个数 cnt 和 sum。若 mx≤x 无需改；若 $"mx2"<x<"mx"$，只有最大值组变化，sum+=(x−mx)×cnt；x≤mx2 必须递归。限低对称维护 mn、mn2、cntmin。加法平移有限极值，不修改次极值哨兵。

*调用与边界：* 复用 #ref(<algo-data-structure-segment-tree-beats>, supplement: [Beats])；下传先加法再用父节点最值约束儿子。复杂度按这组完整操作为摊还 $O((n+Q)log^2 n)$ 的常用上界，单次最坏可线性，空间 $O(n)$；不得把任意新增操作也归入该界。*记忆：* 快速作用的门槛是“只改变一组相同的极值”，等于次极值也不能直接过。

== 区间取模与开方：收敛剪枝 <algo-seg-model-shrinking-updates>

*识别与例题：* 非负数组区间取模 / 下取整平方根，查询和；#link("https://codeforces.com/problemset/problem/438/D")[CF438D] 另有点赋值，#link("https://www.luogu.com.cn/problem/P4145")[P4145] 为开方版。自拟例：[1,9,16] 开方后 [1,3,4]，重复后很快稳定于 0 / 1。

*维护：* Info 存 sum、mx。取模 x>0 且 $"mx"<x$ 时整段跳过，否则下探到叶子；开方且 mx≤1 时跳过，必要时用 mn、mx 判断全段是否落入同一个平方根桶并赋值。这不是普通 Tag 的封闭作用。

*界限与调用：* 可在 #ref(<algo-data-structure-segment-tree-point>, supplement: [点改树]) 加剪枝递归。有效取模使该数至少减半，值上界 V、p 次点赋值时，总体可用 $O((n+p)log(V+1)log n+Q log n)$ 上界；没有重新增大的开方，每项改变 $O(log log(V+2))$ 次。大量区间加会破坏这一分析。整数平方根用近似后乘法校正，128 位避免平方溢出。*记忆：* 复杂度来自一个数还能变几次，不是每次操作都只走一条链。

== 主席树题型：区间第 k 小、计数与前 k 小和 <algo-seg-model-persistent-order>

*识别与例题：* 静态数组、多次 [l,r] 第 k 小。#link("https://www.luogu.com.cn/problem/P3834")[P3834]；值域计数、前 k 小之和是同一对版本的扩展。自拟例：[5,1,5,3] 的 [2,4] 第 2 小为 3，前 2 小和为 4。

*状态与下降：* 按值域建前缀版本 root[i]，每次由 root[i−1] 插入 $A_i$。查询用 root[r]−root[l−1]；左儿子计数 c≥k 则向左，否则 k−=c 向右。问值域计数时做版本差区间和；问前 k 小和时额外存原值 sum，向右前先累加左儿子 sum，叶子再加 k×原值。

*调用与边界：* 引用 #ref(<algo-data-structure-persistent-segment-tree-count>, supplement: [主席树计数])，和字段属于需另加的扩展；不要用离散化排名代替原值求和。建树 $O(n log V)$ 时间空间，查询 $O(log V)$；第 k 小要求 k≥1，前 k 小和允许 k=0，均须 k≤区间长度。*记忆：* 减的是两个版本的统计，不是把两棵树真实相减或修改旧根。

=== 验证范围与现场复核

本章的 Python 测试检查题目 / 标签覆盖，并将若干 Info 合并、Tag 作用、函数复合、扫描递推与小规模暴力比较；它不包含 24 题的完整 C++ 提交，也不证明原题最大数据的时空限制。P 是随机算法；R、U、W 的摘要实现边界以及 X 的动态残量维护仍须在实际落地时专门测试。公式使用前先核对值域、非负性、端点唯一性、模数和扫描事件顺序。
