= 树上题型 <chapter-tree-problems>

本章按“识别 → 状态 / 转化 → 维护 → 边界 → 小例子”整理模型，原树节点与 DFS 序均为 1-index；根的父亲 0 只作哨兵，不参与真实统计。已有源码只链接到原位置，不重复展示。以下自拟例用于理解模型，外部链接为练习题；模型复杂度不等于任意变体均能通过原题。

=== 现场选型

子树先看 DFS 序连续区间；路径求和 / 最大值看 HLD；离线路径加看差分；所有根答案看换根；每次只有少数关键点看虚树；子树颜色看启发式合并；静态点对距离看点分治；边权阈值连通看重构树。递归在链上可能深达 n，先评估栈；所有距离、贡献和与计数用 ll 并检查乘法。

== 路径与子树转换：DFS 序接区间结构 <algo-tree-model-euler-path>

*识别与例题：* 改点权、问子树和或路径最大。#link("https://cses.fi/problemset/task/1137/")[Subtree Queries]、#link("https://cses.fi/problemset/task/2134/")[Path Queries II]。根为 1、边 1–2、1–3，权 2、5、7 时根子树和 14，路径 2→3 最大值 7。

*状态与维护：* 先建 #ref(<algo-tree-hld>, supplement: [HLD])，用 a[dfn[u]]=w[u] 建树；`subtree(u)` 返回 [dfn[u],dfn[u]+siz[u]−1]。`path(u,v,回调)` 把路径拆成若干闭区间，各段接 #ref(<algo-data-structure-segment-tree-point>, supplement: [Info]) 的 query 合并；区间更新改接 InfoTag。

*复杂度与边界：* 预处理 $O(n)$，子树操作 $O(log n)$，路径操作 $O(log^2 n)$，空间 $O(n)$。边权放深端点，使用 path_edge 排除 LCA 对应位置；仅维护根到点和时也可把点改转成子树区间加，再单点查。*记忆：* 同一份 DFS 序同时服务路径与子树，但边权和点权的端点规则不同。

== 树上差分：点路径加与边路径加 <algo-tree-model-difference>

*识别与例题：* 离线给许多路径各加 w，最后问每点 / 边被覆盖多少次。#link("https://cses.fi/problemset/task/1136/")[Counting Paths] 为点统计。链 1–2–3 只加路径 2→3，点计数 0、1、1，边计数 0、1。

*转化：* 令 t=LCA(u,v)。点差分：d[u]+=w、d[v]+=w、d[t]−=w，t 有父亲时 d[fa[t]]−=w；边差分：d[u]+=w、d[v]+=w、d[t]−=2w。最后后序令父亲累加儿子。点版 d[u] 是点答案，边版 d[u] 是边 fa[u]–u 的答案。

*调用与边界：* 引用 #ref(<algo-tree-lca>, supplement: [LCA])，所有路径打完标记后只做一次后序；$O((n+Q)log n)$ 时间、$O(n log n)$ 空间，若使用线性空间的其他 LCA 实现则另计。同点路径在点版加一次、边版加零；不能中途把累加后的 d 当原始差分继续反复汇总。*记忆：* 点减 LCA 和它父亲，边减两次 LCA。

== 换根 DP：全点距离和与排除一个儿子 <algo-tree-model-reroot>

*识别与例题：* 求每个点到全树的距离和。#link("https://cses.fi/problemset/task/1133/")[Tree Distances II]。链 1–2–3、单位边权，答案 3、2、3。

*状态与转移：* 固定根求 siz[u] 和 down[u]；儿子 v 边权 w 的贡献为 down[v]+siz[v]×w。根答案为 down[root]，换到 v 时：

$ "ans"[v]="ans"[u]+(n-2"siz"[v])w. $

*调用与扩展：* 直接调用 #ref(<algo-dp-reroot-distance>, supplement: [reroot_distance(g)])。带点质量时把 siz 换为子树质量和、n 换总质量；一般可结合但不可做除法的贡献，用儿子前缀 / 后缀合并求“排除 v”。两遍 DFS $O(n)$ 时间空间。*边界 / 记忆：* 不要把含 v 的总贡献原样传回 v；对子树变近、对其余点变远。

== 路径方向：非交换 Info 的两端拼接 <algo-tree-model-directed-path>

*识别与自拟例：* 路径字符串、矩阵乘积、函数复合要求沿 u→v 的顺序。链上字符 a、b、c，3→1 应为 cba，不能返回 abc。普通路径和不暴露这种错误。

*维护：* 每个区间存正向 f 与反向 r。合并 $f=f_L compose f_R$、$r=r_R compose r_L$，这里 compose 表示按题目定义的顺序拼接。手写 HLD 的两端循环：u 端取 top[u]→u 的反向摘要追加到左答案；v 端取 top[v]→v 的正向摘要前插到右答案。最终同链段按深度决定方向，结果为左答案、同链段、右答案。

*接口与边界：* 复用 #ref(<algo-tree-hld>, supplement: [HLD]) 的 top、fa、dep、dfn；现有 path 回调会交换端点，不能直接视为有序接口。接 #ref(<algo-data-structure-segment-tree-point>, supplement: [Info]) 并补双向摘要，$O(log^2 n)$ 每次、空间 $O(n)$；边权版最终段跳过 LCA，单位元需正确。*记忆：* u 端往后接，v 端往前接，反向区间不等于交换乘法参数一次。

== 虚树：少量关键点的距离与连通骨架 <algo-tree-model-virtual>

*识别与自拟例：* 固定大树，每次给 k 个关键点，问两两距离和或连接它们的最小子树。单位链 1–2–3–4，关键点 {1,3,4} 的两两距离和为 6，连接子树长 3。

*转化与状态：* 关键点按 dfn 排序，加入相邻关键点的 LCA，再排序去重并建虚树。虚边 u→v 长度为原树根距离之差，不能当作长度 1。后序 cnt[u] 为关键点数，虚边长 w、下方有 c 个关键点时，两两无序点对距离和贡献 $w c(k-c)$；连接最小子树仅累加两侧均有关键点的边。

*调用与边界：* #ref(<algo-tree-virtual-tree>, supplement: [VTree]) 先 prep(root)，每次 build(key)，只遍历 nodes；带权距离需另外保存原树根距离，不能拿当前 dep 代替。预处理 $O(n log n)$，每次保守 $O(k log n+k log k)$、额外 $O(k)$。去重后计 k，空集答案 0；不能每次 O(n) 清全树。*记忆：* 虚边压缩的是一整条原树链，长度与贡献必须还原。

== DSU on Tree：子树颜色与频次统计 <algo-tree-model-dsu-colors>

*识别与例题：* 每点问子树不同颜色数，或“出现次数最多的颜色有哪些”。#link("https://cses.fi/problemset/task/1139/")[Distinct Colors]。根与两个儿子颜色为红、蓝、蓝，根的不同颜色数为 2，众数为蓝。

*状态与顺序：* 全局 freq[color]、不同色数 distinct。先递归轻儿子且清空，再递归重儿子保留；把轻子树和当前点加入，记录当前答案；不保留时清除整个子树。众数可增加最大频次 mx、该频次颜色值之和 sum；增加时更新，整棵统计清空时重置。

*调用与边界：* 基础不同色实现直接引用 #ref(<algo-tree-dsu-on-tree>, supplement: [DSU on Tree])；众数是需另改统计器的扩展，任意删除时不能只沿用增加公式。按子树大小选重儿子，静态总 $O(n log n)$ 时间、$O(n)$ 空间；颜色离散化后再下标，颜色值求和用原值。*记忆：* 保留一个大袋子，轻子树中的点只被重新加入对数次。

== 点分治：距离区间点对与同子树扣除 <algo-tree-model-centroid-pairs>

*识别与例题：* 静态树统计距离在 [L,R] 的不同点无序对。#link("https://cses.fi/problemset/task/2081/")[Fixed-Length Paths II] 是无权版。三点链距离恰为 2 的点对仅 {1,3}。

*转化与维护：* 定义 F(K)=距离≤K 的点对数，答案 F(R)−F(L−1)。当前重心 c 收集所有点到 c 的距离（含 c 的 0），排序双指针计和≤K；减去每个儿子分支内部同样的距离配对，再递归分治。这样只保留经过 c 的真实点对，同分支的路不一定经过 c。

*调用与限制：* 复用 #ref(<algo-tree-centroid>, supplement: [Centroid])，分别初始化 K=R 和 L−1 跑两遍。现有排序版 $O(n log^2 n)$ 时间、$O(n)$ 额外空间，不把它说成该题所有环境下最快的无权特解；无权深度桶可另做优化。点对不含自己；LLONG_MIN 时不能直接算 L−1，带权路径和先检查范围。*记忆：* 全部算一次，再把没有经过当前重心的配对扣掉。

== Kruskal 重构树：最小瓶颈与阈值连通块 <algo-tree-model-reconstruction>

*识别与自拟例：* 只用权≤x 的边时点 u 能到多少点，或两点何时首次连通。边 1–2 权 3、2–3 权 8，则 1 和 3 的最小瓶颈为 8；阈值 5 时点 1 的连通块大小为 2。

*转化与调用：* 按边权从小到大 Kruskal，每次合并两个连通块，新建父节点、权为本次边权。引用 #ref(<algo-tree-kruskal-tree>, supplement: [KRT])，add 后 build；connected 先判断，bottleneck(u,v) 取两叶 LCA 的权。climb(u,x) 得权≤x 的最高祖先，其 siz 为原节点数量；要查点权统计，再接它的 DFS 序区间。

*复杂度与边界：* 建树 $O(m log m+n log n)$，查询 $O(log n)$，空间 $O(m+n log n)$；断图为重构森林，不能跨树调用瓶颈。相同点接口约定为 0，负边时别把这个约定当作真实边最大值；多次等权合并不影响阈值连通。*记忆：* 原图的连通变化变成祖先链，时间 / 边权阈值变成向上跳。
