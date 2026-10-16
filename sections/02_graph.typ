= 图论 <chapter-graph>

本章迁移 `raw/02_graph.md` 的全部 34 个小节，共拆分为 37 份独立模板。
保留原算法思路，普通图使用简单邻接表；需要分量、历史、残量或辅助节点状态时才保留轻量 `struct`。
节点默认 `1..n`，下标 `0` 保留；边号、残量边下标、返回序列位置和匈牙利输入矩阵的例外另行标明。

*比赛复制：* 先复制下面的个人基础模板，再复制所需算法实现。源码中的标准头文件与 `ll` 兜底宏用于独立维护和测试，可保留。
需要 DSU / Dinic 的模板已将依赖完整展开；PDF 中不出现仓库头文件导入。组合多份模板时，共用的 DSU / Dinic 只复制一份。
两版费用流分别名为 `MCMF` 与 `MCMFPotential`，避免类型冲突。

*统一检查：* 输入端点和区间必须合法；换图或多测按照各模板的初始化约定清空状态。
递归模板需评估链状图的栈深度。最短路的 INF 是数值范围约定，不能把合法答案做到哨兵之外；费用和总权值注意乘法、加法与取负溢出。
本章的 `__int128` 运算依赖支持该扩展的 GCC 目标；其余代码遵守 C++17。

*个人基础模板（STYLE.md 原文）：*
#let personal_base = read("../STYLE.md").replace("\r\n", "\n").split("```cpp\n").at(1).split("\n```").at(0)
#raw(personal_base, lang: "cpp", block: true)

#let graph_code(file, deps: ()) = {
  let source = deps.map(dep => read("../code/graph/" + dep)).join("\n") + "\n" + read("../code/graph/" + file)
  source = source.replace(regex("(?m)^#pragma once\r?\n"), "")
  source = source.replace(regex("(?m)^#include \"[^\"]+\"\r?\n"), "")
  raw(source, lang: "cpp", block: true)
}


== 强连通分量 SCC - Tarjan + 缩点 <algo-graph-scc>

*来源：* 原文 2.1。
*用途：* 求有向图强连通分量，并可缩点得到 DAG。
*形式：* struct；需要维护 DFS 时间、栈、分量归属及缩点结果，保留轻量 struct。


=== 复杂度与下标

- SCC 时间 `O(n+m)`，缩点时间 `O(n+m)`；空间 `O(n+m)`。
- 节点 `1-indexed`，SCC 编号 `1..cnt`。


=== 接口说明

- `add(u, v)`：加有向边 `u -> v`
- `work()`：求 SCC
- `dag()`：SCC 缩点，需先 `work()`
- `cnt`：SCC 数量
- `bel[u]`：`u` 所属 SCC
- `siz[id]`：SCC `id` 的大小


=== 调用方式

```cpp
SCC scc(3);
scc.add(1, 2); scc.add(2, 1); scc.add(2, 3);
scc.work();
cout << scc.cnt << endl;
auto dag = scc.dag();
```


=== 注意事项

- 支持重边、自环、不连通图和空图；缩点自动去重。
- 先加边，再 work，再读取 bel / siz / dag。work 可重复运行并重置算法状态；init 会清空原图。
- 递归深度最坏 O(n)，链状大图需评估栈限制。


=== 现场记忆

树边用 low[v]，仍在栈的返祖边用 dfn[v]。low[u] == dfn[u] 时弹出一个 SCC；弹栈编号是拓扑反序。


=== 模板引用

#graph_code("scc.hpp")


== 割点 / 桥 <algo-graph-cut-bridge>

*来源：* 原文 2.2。
*用途：* 求无向图所有割点和桥。
*形式：* struct；边编号、DFS 状态和两个结果共用一次遍历，保留 struct。


=== 复杂度与下标

- 时间 `O(n+m)`，空间 `O(n+m)`。
- 节点 `1-indexed`。


=== 接口说明

- `add(u, v)`：加无向边
- `work()`：求割点和桥
- `is_cut[u]`：`u` 是否为割点
- `bridges`：所有桥的两个端点


=== 调用方式

```cpp
CutBridge cb(3);
cb.add(1, 2); cb.add(2, 3);
cb.work();
cout << int(cb.is_cut[2]) << " " << cb.bridges.size() << endl;
```


=== 注意事项

- 父边按边 id 跳过，支持重边、自环、不连通图。边编号从 0 开始。
- work 可重复运行；init 清图并清空结果。桥端点方向取决于 DFS，不能假设有序。
- 递归深度最坏 O(n)。


=== 现场记忆

非根割点 low[v] \>= dfn[u]；桥 low[v] \> dfn[u]。根看 DFS 儿子数至少二，重边不能只跳父节点。


=== 模板引用

#graph_code("cut_bridge.hpp")


== 边双连通分量 EBCC <algo-graph-ebcc>

*来源：* 原文 2.3。
*用途：* 求无向图边双连通分量，并可缩点得到桥树 / 桥森林。
*形式：* struct；维护桥标记、分量归属、大小及桥森林，保留 struct。


=== 复杂度与下标

- 时间 `O(n+m)`，空间 `O(n+m)`。
- 节点 `1-indexed`，边双编号 `1..cnt`。


=== 接口说明

- `add(u, v)`：加无向边
- `work()`：求桥和边双
- `bridge_tree()`：缩点建桥树 / 森林，需先 `work()`
- `cnt`：边双数量
- `bel[u]`：`u` 所属边双
- `siz[id]`：边双大小
- `is_bridge[id]`：第 `id` 条边是否为桥


=== 调用方式

```cpp
EBCC ebcc(3);
ebcc.add(1, 2); ebcc.add(1, 2); ebcc.add(2, 3);
ebcc.work();
auto tree = ebcc.bridge_tree();
cout << ebcc.cnt << endl;
```


=== 注意事项

- 边编号从 0 开始，支持重边、自环和孤点。先 work 再 bridge\_tree。
- 不连通原图得到桥森林；每条桥在邻接表中出现两个方向。
- work 重算；init 清图。找桥与分量 DFS 都可能达到 O(n) 递归深度。


=== 现场记忆

先用 low[v] \> dfn[u] 找桥，再忽略桥 DFS 分块。跨边双的边都是桥，缩点得到森林。


=== 模板引用

#graph_code("ebcc.hpp")


== 点双连通分量 VBCC / 圆方树 <algo-graph-vbcc>

*来源：* 原文 2.4。
*用途：* 求无向图点双连通分量和割点，并建立圆方树。
*形式：* struct；一个节点可属于多个点双，维护点栈、割点和块列表，适合 struct。


=== 复杂度与下标

- 时间 `O(n+m)`，空间 `O(n+m)`。
- 原节点 `1..n`，点双编号 `1..cnt`；圆方树方点为 `n+1..n+cnt`。


=== 接口说明

- `add(u, v)`：加无向边
- `work()`：求点双和割点
- `block_tree()`：建圆方树 / 森林，需先 `work()`
- `cnt`：点双数量
- `bcc[id]`：第 `id` 个点双包含的原节点
- `is_cut[u]`：`u` 是否为割点


=== 调用方式

```cpp
VBCC vbcc(3);
vbcc.add(1, 2); vbcc.add(2, 3);
vbcc.work();
auto tree = vbcc.block_tree();
cout << vbcc.cnt << " " << int(vbcc.is_cut[2]) << endl;
```


=== 注意事项

- 输入禁止自环，支持重边、断开的图；孤点单独作为一个点双。
- 不存在唯一 bel[u]，应遍历 bcc[id]。圆方树边是双向的；原图不连通时得到森林。
- work 可重复；init 清图；递归深度最坏 O(n)。


=== 现场记忆

low[v] \>= dfn[u] 时弹点栈到 v，并额外加入 u；u 留在栈中参与其他块。根割点仍看儿子数，方点连接块内所有原点。


=== 模板引用

#graph_code("vbcc.hpp")


== 2-SAT <algo-graph-two-sat>

*来源：* 原文 2.5。
*用途：* 判断只含二元布尔约束的系统是否可满足，并构造一组可行赋值。
*形式：* struct；共享蕴含图、Tarjan 状态和赋值结果，保留 struct。


=== 复杂度与下标

- 设变量数为 `n`、蕴含边数为 `m`，时间 `O(n+m)`，空间 `O(n+m)`。
- 变量 `1..n`，`assignment[i]` 为 `0/1`。


=== 接口说明

- `either(x, vx, y, vy)`：`(x=vx) OR (y=vy)`
- `force(x, v)`：强制 `x=v`
- `eq(x, y)`：`x == y`
- `neq(x, y)`：`x != y`
- `satisfiable()`：判断是否有解并生成方案
- `assignment[x]`：一组可行布尔赋值


=== 调用方式

```cpp
TwoSAT sat(2);
sat.either(1, true, 2, false);
sat.neq(1, 2); sat.force(1, true);
if (sat.satisfiable()) cout << sat.assignment[2] << endl;
```


=== 注意事项

- 本实现 true 对应 2*i-1，false 对应 2*i；文字反转由 opp 处理。
- 先添加约束，再 satisfiable；可重复求解，也可继续加约束后重算。无解时不读取 assignment。
- init 清空全部约束；蕴含图有 2*n 个节点，递归深度最坏 O(n)。


=== 现场记忆

(a OR b) 连 !a -\> b 与 !b -\> a。真假在同 SCC 则无解；本 Tarjan 编号反拓扑，赋值用 bel[true] \< bel[false]。


=== 模板引用

#graph_code("two_sat.hpp")


== 链式前向星 Vector 版 <algo-graph-forward-star>

*来源：* 原文 2.6。
*用途：* 用连续边数组维护邻接表，适合需要直接操作边编号或反向边的图算法。
*形式：* struct；这是一份显式边号存储工具，head 与边数组需要共同维护，保留原 struct，不用它替代普通图。


=== 复杂度与下标

- vector 加边均摊 `O(1)`，单次扩容最坏 `O(m)`；遍历 `u` 的出边 `O(outdegree(u))`，空间 `O(n+m)`。
- 节点 `1-indexed`，边编号 `0-indexed`。


=== 接口说明

- `init(n, m)`：初始化，可预留 `m` 条边
- `add(u, v, w)`：加边并返回边编号
- `head[u]`：`u` 第一条出边编号
- `e[i]`：第 `i` 条边
- `e[i].to`：终点
- `e[i].w`：边权
- `e[i].nxt`：下一条同起点边


=== 调用方式

```cpp
ForwardStar g(2, 2);
g.add(1, 2, 5); g.add(2, 1, 5);
for (int i = g.head[1]; i != -1; i = g.e[i].nxt)
{
    cout << g.e[i].to << " " << g.e[i].w << endl;
}
```


=== 注意事项

- 普通无权 / 带权图优先简单 vector 邻接表；只有需要连续边号时使用。
- add 可能引起 vector 扩容，不持有跨 add 的 Edge 引用。init 会清空 head 与 e，预留容量仅是优化。
- 连续加入两个方向且此前边数为偶数时，反向边才是 id ^ 1。


=== 现场记忆

head 初值 -1；新边插到链表头；遍历 i = e[i].nxt，顺序与 add 相反。


=== 模板引用

#graph_code("forward_star.hpp")


== Dijkstra 最短路 <algo-graph-dijkstra>

*来源：* 原文 2.7。
*用途：* 求非负权图中单源最短路。
*形式：* 函数式；只需图与源点，状态在函数内初始化，不包存图类。


=== 复杂度与下标

- 堆实现时间 `O((n+m) log(m+1))`，通常简写为 `O((n+m) log n)`；空间 `O(n+m)`。
- 节点 `1-indexed`。


=== 接口说明

- dijkstra(g,s)：返回 dis；g[0] 保留不用。


=== 调用方式

```cpp
vector<vector<pair<int, ll>>> g(4);
g[1].push_back({2, 4}); g[1].push_back({3, 1});
g[3].push_back({2, 1});
auto dis = dijkstra(g, 1);
cout << dis[2] << endl;
```


=== 注意事项

- 边权必须非负，源点合法。图边可为 pair\<int,ll\> 或能分解为 (v,w) 的 SegGraph::Edge。
- 不可达返回 INF = 2^62；需保证有效最短距离小于 INF。候选 \>= INF 时跳过，避免 d+w 溢出。
- 图不会被修改，可重复调用和切换源点；无向边手动加入两个方向。多重边时堆实现更精确为 O((n+m) log(m+1))。


=== 现场记忆

堆存 (距离, 点)；弹出旧距离立即跳过。非负边才能用；先判断 w \< INF-d，再相加。


=== 模板引用

#graph_code("dijkstra.hpp")


== SPFA 判负环 <algo-graph-negative-cycle>

*来源：* 原文 2.8。
*用途：* 判断整张有向图中是否存在负权环。
*形式：* 函数式；只判断整图负环，无需持久状态，使用函数和 queue。


=== 复杂度与下标

- 最坏时间 `O(nm)`，空间 `O(n+m)`。
- 节点 `1-indexed`。


=== 接口说明

- neg\_cycle(g)：返回整张图是否含负环，不输出最短路。


=== 调用方式

```cpp
vector<vector<pair<int, ll>>> g(3);
g[1].push_back({2, -1}); g[2].push_back({1, 0});
cout << neg_cycle(g) << endl;
```


=== 注意事项

- 所有点同时以距离 0 入队，能检测非源点所在连通块。
- cnt 记录当前更短路径的边数，不是入队次数；cnt[v] \>= n 判负环。
- 内部距离改用 GNU \_\_int128，避免大负权在判环前溢出；接口仍是 ll 边权。空图返回 false；每次调用独立。


=== 现场记忆

超级源到所有点都是零边；松弛时 cnt[v] = cnt[u]+1，至少 n 条边意味着经过负环。


=== 模板引用

#graph_code("negative_cycle.hpp")


== Floyd 全源最短路 <algo-graph-floyd>

*来源：* 原文 2.9。
*用途：* 小规模有向或无向图的全源最短路。
*形式：* 函数式；就地更新距离矩阵，使用函数，不封装图。


=== 复杂度与下标

- 时间 O(n^3)，额外空间 O(1)，输入矩阵 O(n^2)。
- 节点 `1-indexed`。


=== 接口说明

- floyd(dis)：就地计算，调用后直接读取 dis[i][j]。


=== 调用方式

```cpp
const ll INF = 1LL << 62;
vector<vector<ll>> dis(4, vector<ll>(4, INF));
for (int i = 1; i <= 3; ++i) dis[i][i] = 0;
dis[1][2] = 4; dis[2][3] = -2;
floyd(dis);
cout << dis[1][3] << endl;
```


=== 注意事项

- 输入为方阵，dis[i][i]=0，重边取最小值；不可达必须使用同一个 INF = 2^62。
- 允许负边。存在负环时相关最短路无定义，只用负的 dis[i][i] 诊断。负距离饱和到 -INF，避免反复绕负环溢出。
- 使用 GNU \_\_int128 做中间加法；无负环时真实有限距离须严格落在 (-INF, INF)。修改矩阵，不清空输入；无负环结果可再运行。


=== 现场记忆

k 放最外层，两段都是有限值才能相加。零对角、重边取 min；负对角看负环。


=== 模板引用

#graph_code("floyd.hpp")


== bitset 传递闭包 <algo-graph-transitive-closure>

*来源：* 原文 2.9。
*用途：* 以 bitset 批量求有向图传递闭包。
*形式：* 函数式；bitset 行的可达性传播，使用函数。


=== 复杂度与下标

- 时间 O(n^2*ceil(N/w))，N=O(n) 时约 O(n^3/w)；输入空间 O(n*ceil(N/w)) 个机器字。
- 节点 `1-indexed`。


=== 接口说明

- closure(reach,n)：就地更新；w 为机器字位数。


=== 调用方式

```cpp
vector<bitset<8>> reach(4);
reach[1][2] = 1; reach[2][3] = 1;
closure(reach, 3);
cout << reach[1][3] << endl;
```


=== 注意事项

- 模板位数 N 必须大于 n，reach 至少有 n+1 行。图点虽 1-index，bitset 的存储位天然从 0 开始。
- 只求正长度路径可达性时不必预置对角；需要自反闭包则先设 reach[i][i]=1。
- 就地修改 reach，可重复运行；换图须重新初始化矩阵。


=== 现场记忆

reach[i][k] 成立就 reach[i] |= reach[k]；中转点 k 最外层。


=== 模板引用

#graph_code("transitive_closure.hpp")


== 差分约束 <algo-graph-difference-constraints>

*来源：* 原文 2.10。
*用途：* 判断一组形如 `x_v - x_u <= w` 的差分约束是否有解，并构造一组可行解。
*形式：* struct；保留约束添加与求解接口，用轻量 struct 保存约束；内部仍是 queue SPFA。


=== 复杂度与下标

- SPFA 最坏时间 `O(nm)`，空间 `O(n+m)`。
- 变量 `1-indexed`。


=== 接口说明

- `leq(u, v, w)`：加入 `x_v - x_u <= w`
- `solve()`：返回 `{是否有解, 一组可行解}`


=== 调用方式

```cpp
DiffCons dc(2);
dc.leq(1, 2, -3); dc.leq(2, 1, 4);
auto [ok, val] = dc.solve();
if (ok) cout << val[2] - val[1] << endl;
```


=== 注意事项

- solve 不修改约束，重复求解和追加约束后求解均可；init 清空图。
- 所有初始距离为 0，存在任意负环即无解；无解时返回空解向量。
- 需保证路径累加和能由 ll 表示；建议 n\*max|w| \< 2^60。将 \>= w 改成反向 \<= -w 时也要防止对最小负数取负。


=== 现场记忆

x\_v-x\_u \<= w 连 u-\>v(w)；\>= w 反向连 -w。负环就是矛盾，dis 是可行解，全体加常数仍满足。


=== 模板引用

#graph_code("difference_constraints.hpp")


== Kruskal 最小生成树 <algo-graph-kruskal>

*来源：* 原文 2.11。
*用途：* 求无向带权图的最小生成树。
*形式：* 函数式；排序和合并是单次流程，使用函数；复用普通 DSU，不增加 MST 外壳。


=== 复杂度与下标

- 时间 `O(n+m log(m+1))`，包含 DSU 初始化；空间 `O(n+m)`。
- 节点 `1-indexed`。


=== 接口说明

- MstEdge {u,v,w}：无向边。
- kruskal(n,e)：返回 {是否连通,权值}。


=== 调用方式

```cpp
vector<MstEdge> e{{1, 2, 3}, {2, 3, 1}, {1, 3, 9}};
auto [ok, ans] = kruskal(3, e);
if (ok) cout << ans << endl;
```


=== 注意事项

- 无向边只列一次，支持负边、重边、自环。边参数按值传入，排序不改变调用方数组。
- 不连通返回 false，此时 ans 只是已选森林权值，不是 MST。n=0 或 1 约定返回 {true,0}。
- 累加总权值应能放入 ll。每次建立新的 DSU，重复调用独立。PDF 已展开 DSU 实现。


=== 现场记忆

按边权从小到大；不同集合才选边；选够 n-1 条即停止。


=== 模板引用

#graph_code("kruskal.hpp", deps: ("dsu.hpp",))


== Prim 最小生成树 <algo-graph-prim>

*来源：* 原文 2.12。
*用途：* 用邻接表求无向带权图的最小生成树，适合直接从图上跑 MST。
*形式：* 函数式；堆与 best、vis 是临时数组，使用函数，不建立图类。


=== 复杂度与下标

- 堆实现时间 `O(n+m log(m+1))`，通常简写为 `O(m log n)`；空间 `O(n+m)`。
- 节点 `1-indexed`。


=== 接口说明

- prim(g,s=1)：返回 {是否连通,权值}，不会修改图。


=== 调用方式

```cpp
vector<vector<pair<int, ll>>> g(4);
auto add = [&](int u, int v, ll w)
{
    g[u].push_back({v, w}); g[v].push_back({u, w});
};
add(1, 2, -1); add(2, 3, 4); add(1, 3, 8);
auto [ok, ans] = prim(g);
if (ok) cout << ans << endl;
```


=== 注意事项

- 每条无向边要加入两个方向；允许负边、重边和自环。
- best 初值 INF=2^62，边权须小于 INF，总权值必须能放入 ll。不连通时 false，ans 只是源点所在部分的树权值。
- 空图约定 {true,0}，单点图也返回零；每次调用重新初始化状态。


=== 现场记忆

best[v] 是树连到 v 的最小边权，不是最短路距离。每次选尚未加入且 best 最小的点。


=== 模板引用

#graph_code("prim.hpp")


== 并查集 DSU <algo-graph-dsu>

*来源：* 原文 2.13。
*用途：* 维护集合合并、连通性和集合大小。
*形式：* struct；fa、siz 与路径压缩共同维护，适合 struct。


=== 复杂度与下标

- 均摊时间 `O(α(n))`，空间 `O(n)`。
- 节点 `1-indexed`。


=== 接口说明

- `find(x)`：查询代表元
- `merge(x, y)`：合并，已连通返回 `false`
- `same(x, y)`：是否属于同一集合
- `size(x)`：`x` 所在集合大小
- `fa[x]`：父节点
- `siz[root]`：集合大小


=== 调用方式

```cpp
DSU dsu(3);
dsu.merge(1, 2);
cout << dsu.same(1, 2) << " " << dsu.size(1) << endl;
```


=== 注意事项

- find 递归路径压缩，merge 按大小合并；只有根的 siz 有意义。
- merge 已连通返回 false。普通 DSU 不支持撤销；多测用 init(n) 清空父亲和大小。


=== 现场记忆

路径压缩，小树挂大树；size 查询先 find，不能读取非根 siz。


=== 模板引用

#graph_code("dsu.hpp")


== 可撤销并查集 Rollback DSU <algo-graph-rollback-dsu>

*来源：* 原文 2.14。
*用途：* 支持撤销集合合并，常用于线段树分治、离线动态连通性。
*形式：* struct；必须维护撤销记录，使用 struct。


=== 复杂度与下标

- `find/merge` 为 `O(log n)`，单次撤销 `O(1)`；空间 `O(n+q)`。
- 节点 `1-indexed`。


=== 接口说明

- `merge(x, y)`：合并两个集合
- `same(x, y)`：判断连通
- `size(x)`：集合大小
- `snapshot()`：保存当前状态编号
- `rollback(snap)`：回滚到指定状态
- `undo()`：撤销最近一次 `merge`


=== 调用方式

```cpp
RollbackDSU dsu(3);
int snap = dsu.snapshot();
dsu.merge(1, 2); dsu.merge(1, 2);
dsu.undo(); dsu.rollback(snap);
cout << dsu.same(1, 2) << endl;
```


=== 注意事项

- 禁止路径压缩；按大小合并控制树高。一次 merge 对应一条历史，即使没有真正合并。
- undo 要求历史非空，rollback 的 snap 必须在 [0,snapshot()]。快照只能用于当前历史的祖先状态。
- rollback 撤销 k 条记录的总时间 O(k)，不是整个回滚 O(1)。init 清空历史，多测不能沿用旧快照。


=== 现场记忆

不压缩，每次合并记录小根、大根和大根旧大小；无效合并也压空记录。


=== 模板引用

#graph_code("rollback_dsu.hpp")


== 可持久化并查集 Persistent DSU <algo-graph-persistent-dsu>

*来源：* 原文 2.15。
*用途：* 保留并查集的所有历史版本，可从任意旧版本继续合并或查询。
*形式：* struct；线段树节点池、版本根和并查集秩需共存，保留 struct。


=== 复杂度与下标

- `find/merge/same` 时间 `O(log^2 n)`；`q` 次修改空间 `O(n+q log n)`。
- 节点 `1-indexed`。


=== 接口说明

- `base`：初始版本根
- `find(root, x)`：在指定版本查询代表元
- `merge(root, x, y)`：返回合并后的新版本根
- `same(root, x, y)`：指定版本中是否连通


=== 调用方式

```cpp
PDSU dsu(3);
int r0 = dsu.base;
int r1 = dsu.merge(r0, 1, 2);
int r2 = dsu.merge(r0, 2, 3);
cout << dsu.same(r1, 1, 2) << " " << dsu.same(r2, 1, 2) << endl;
```


=== 注意事项

- 根是版本标识，可从任意旧根分支；已经连通时 merge 返回原根。
- 禁止路径压缩，按秩合并。版本根属于当前对象和当前 init 生命周期；init 后旧根全部失效。
- 节点池扩容会使 Node 引用失效，原模板始终用下标访问。n=0 只可初始化，不能查不存在节点。


=== 现场记忆

可持久化线段树存 fa/rk；一次合并改一个父亲，必要时再改一个秩。每次找父亲需一次线段树查询。


=== 模板引用

#graph_code("persistent_dsu.hpp")


== 加法差分带权并查集 <algo-graph-weighted-dsu>

*来源：* 原文 2.16。
*用途：* 维护加法差值约束并检测矛盾。
*形式：* struct；相对关系与父链一起压缩，保留 struct。


=== 复杂度与下标

- 均摊时间 `O(α(n))`，空间 `O(n)`。
- 节点 `1-indexed`。


=== 接口说明

- merge(u,v,w)：加入 val[v]-val[u]=w，冲突返回 false。
- same(u,v)：是否已关联。
- diff(u,v)：返回 val[v]-val[u]，要求已关联。


=== 调用方式

```cpp
WeightedDSU dsu(3);
dsu.merge(1, 2, 5); dsu.merge(2, 3, -2);
if (dsu.same(1, 3)) cout << dsu.diff(1, 3) << endl;
```


=== 注意事项

- dis[u] = val[parent[u]]-val[u]，merge(u,v,w) 表示 val[v]-val[u]=w。
- diff 前先检查 same；同集合 merge 可用于判矛盾，返回 false 时原关系仍保留。
- 所有差值和压缩累加需能放入 ll；交换两点时对 w 取负，不能输入 LLONG\_MIN。init 重置全部关系。


=== 现场记忆

压缩 dis[u]+=dis[parent]；挂 fu-\>fv 时 dis[fu]=dis[v]-dis[u]+w；查询是 dis[u]-dis[v]。交换 u,v 要反转 w。


=== 模板引用

#graph_code("weighted_dsu.hpp")


== XOR / 奇偶带权并查集 <algo-graph-xor-dsu>

*来源：* 原文 2.16。
*用途：* 维护 XOR 或奇偶关系，检测矛盾。
*形式：* struct；保留与加法版相同的父链结构，仅替换关系运算，使用 struct。


=== 复杂度与下标

- 均摊时间 `O(α(n))`，空间 `O(n)`。
- 节点 `1-indexed`。


=== 接口说明

- merge(u,v,w)、same(u,v)、diff(u,v)：接口同加法版，diff 返回 XOR。


=== 调用方式

```cpp
XorDSU dsu(3);
dsu.merge(1, 2, 1); dsu.merge(2, 3, 1);
cout << dsu.diff(1, 3) << endl;
```


=== 注意事项

- dis[u]=val[u] XOR val[parent[u]]；merge(u,v,w) 加入两点 XOR 为 w。
- 本模板关系值使用非负 ll，可用于奇偶或最多 63 位非负 XOR；交换点无需反转 w。
- diff 要求连通；重复合并可判矛盾，init 清空关系。


=== 现场记忆

路径压缩用 XOR；挂根用 dis[u]^dis[v]^w；查询 dis[u]^dis[v]。


=== 模板引用

#graph_code("xor_dsu.hpp")


== 模 k 带权并查集 <algo-graph-mod-dsu>

*来源：* 原文 2.16。
*用途：* 维护模 k 的相对关系，例如食物链关系。
*形式：* struct；保留带权并查集结构，增加模数与归一化，使用 struct。


=== 复杂度与下标

- 均摊时间 `O(α(n))`，空间 `O(n)`。
- 节点 `1-indexed`。


=== 接口说明

- ModDSU(n,k)、init(n,k)：指定节点数和正模数。
- norm(w)：归一化。merge / same / diff 同加法版方向。


=== 调用方式

```cpp
ModDSU dsu(3, 3);
dsu.merge(1, 2, 1); dsu.merge(2, 3, 1);
cout << dsu.diff(1, 3) << endl;
```


=== 注意事项

- 模数 k 为正 int；所有 dis 归一化到 [0,k)，输入 w 可为任意 ll，先归一化再交换取负。
- merge(u,v,w) 表示 val[v]-val[u] = w (mod k)；diff 先检查 same。k=1 时所有关系为零。
- init(n,k) 同时重置关系和模数。归一化后的中间加减用 ll，覆盖整个正 int 模数范围。


=== 现场记忆

加法版四处公式都套 norm；交换点时 w=norm(-w)，不能只在最后取一次模。


=== 模板引用

#graph_code("mod_dsu.hpp")


== 拓展域并查集 <algo-graph-extended-domain-dsu>

*来源：* 原文 2.17。
*用途：* 把“同类 / 异类”等有限分类关系拆成多个并查集节点，用普通 DSU 维护关系与矛盾。
*形式：* 函数式建模；二域建模只需普通 DSU，使用函数加入关系，不再包一层类。


=== 复杂度与下标

- 每次关系需要常数次 DSU 操作，均摊 `O(α(n))`。
- 原节点 1..n，对立域 n+1..2\*n。


=== 接口说明

- merge\_domain(dsu,n,u,v,same)：same=true 同类，false 异类；返回是否一致。


=== 调用方式

```cpp
DSU dsu(6);
merge_domain(dsu, 3, 1, 2, true);
merge_domain(dsu, 3, 2, 3, false);
cout << merge_domain(dsu, 3, 1, 3, true) << endl;
```


=== 注意事项

- 传入 DSU 必须有 2\*n 个节点。函数只处理二域的同类 / 异类，k 域的语义应按题目重新建立。
- 冲突时返回 false，先检查再合并，不破坏已有有效关系。重复添加一致关系返回 true。
- 多测重新 init(2\*n)；PDF 已展开普通 DSU。


=== 现场记忆

同类合并 u-v 与 u+n-v+n；异类交叉合并。先查相反关系，再做两次 merge。


=== 模板引用

#graph_code("extended_domain_dsu.hpp", deps: ("dsu.hpp",))


== 并查集虚点 / 参考点 <algo-graph-dsu-reference>

*来源：* 原文 2.18。
*用途：* 把 `val[x]=w` 这类绝对值约束转成并查集维护的相对关系约束。
*形式：* 函数式建模；这是参考点建模方法，复用带权 DSU，仅增加查询函数，不包装图或并查集。


=== 复杂度与下标

- 单次加入 / 查询均摊 O(α(n))，DSU 空间 O(n)。
- 原点 1..n；参考点 S=n+1，DSU 初始化 n+1。


=== 接口说明

- get\_value(dsu,S,u)：返回 optional\<ll\>，加法版绝对值未知时为空。


=== 调用方式

```cpp
int S = 4;
WeightedDSU dsu(4);
dsu.merge(S, 1, 5); dsu.merge(1, 2, -3);
auto val = get_value(dsu, S, 2);
if (val) cout << *val << endl;
```


=== 注意事项

- 设置 val[S]=0，将 val[u]=w 写成 merge(S,u,w)。S 可以被挂到其他根，查询不能假设 S 是根。
- get\_value 未确定绝对值时返回 nullopt；确定时返回 diff(S,u)。注意零值 optional 仍然有值。
- XOR / 模 k 版也建立 S，使用对应 DSU 的 merge(S,u,w) 与 diff(S,u)，并先检查 same。
- 多测重新建立包含参考点的 DSU；加法差值的 ll 限制同前。


=== 现场记忆

绝对值转成与 S 的相对值；S 是参考点而非固定根。


=== 模板引用

#graph_code("dsu_reference.hpp", deps: ("weighted_dsu.hpp",))


== 拓扑排序 <algo-graph-topological-sort>

*来源：* 原文 2.19。
*用途：* 求有向图的一个拓扑序，并判断是否存在有向环。
*形式：* 函数式；图外计算入度并使用 queue，状态局部化，采用函数式 Kahn。


=== 复杂度与下标

- 时间 `O(n+m)`，空间 `O(n+m)`。
- 节点 `1-indexed`。


=== 接口说明

- topo(g)：返回点序列，序列位置是 0-index，节点值仍为 1-index。


=== 调用方式

```cpp
vector<vector<int>> g(4);
g[1].push_back(2); g[2].push_back(3);
auto order = topo(g);
if (order.size() == g.size() - 1)
{
    for (int u : order) cout << u << " ";
}
```


=== 注意事项

- 有环返回空序列；n=0 的合法顺序也是空，应通过 order.size()==n 判断。
- 重边必须逐条计入并减少入度。函数不修改图，可重复调用。
- 需要字典序最小时才将 queue 改成小根堆，此时复杂度会增加。


=== 现场记忆

零入度全入队；弹 u 后删出边，入度降零再入队。取出 n 个点才无环。


=== 模板引用

#graph_code("topological_sort.hpp")


== 欧拉路径 / 欧拉回路：有向图 <algo-graph-euler-directed>

*来源：* 原文 2.20。
*用途：* 求经过每条有向边恰好一次的欧拉路径 / 欧拉回路。
*形式：* struct；保留度数、边数组及检查接口的原轻量 struct；Hierholzer 显式栈也是原文做法，避免边数级递归。


=== 复杂度与下标

- 时间 `O(n+m)`，空间 `O(n+m)`。
- 节点 `1-indexed`。


=== 接口说明

- add(u,v)：加入有向边。
- work()：返回点序列；长度必须 m+1；没有方案返回空。


=== 调用方式

```cpp
EulerDir euler(3);
euler.add(1, 2); euler.add(2, 3);
auto path = euler.work();
for (int u : path) cout << u << " ";
```


=== 注意事项

- 支持重边、自环，不要求孤立点参与连通。检查最终路径长度等于 m+1，以排除边分散在不同连通块。
- n\>0 且无边时返回 {1}；n=0 返回空。不存在欧拉路也返回空。返回点序列的位置从 0 开始。
- work 用局部游标与标记，不消耗原图，支持重复调用；init 清空原图。


=== 现场记忆

起点 out-in=1，终点 in-out=1，其余相等；回路所有入出度相等。从有边点开始，最终反转。


=== 模板引用

#graph_code("euler_directed.hpp")


== 欧拉路径 / 欧拉回路：无向图 <algo-graph-euler-undirected>

*来源：* 原文 2.21。
*用途：* 求经过每条无向边恰好一次的欧拉路径 / 欧拉回路。
*形式：* struct；保留度数、边数组及检查接口的原轻量 struct；Hierholzer 显式栈也是原文做法，避免边数级递归。


=== 复杂度与下标

- 时间 `O(n+m)`，空间 `O(n+m)`。
- 节点 `1-indexed`，边编号 `0-indexed`。


=== 接口说明

- add(u,v)：加入无向边。
- work()：返回点序列；长度必须 m+1；没有方案返回空。


=== 调用方式

```cpp
EulerUndir euler(3);
euler.add(1, 2); euler.add(2, 3);
auto path = euler.work();
for (int u : path) cout << u << " ";
```


=== 注意事项

- 支持重边、自环，不要求孤立点参与连通。检查最终路径长度等于 m+1，以排除边分散在不同连通块。
- n\>0 且无边时返回 {1}；n=0 返回空。不存在欧拉路也返回空。返回点序列的位置从 0 开始。
- work 用局部游标与标记，不消耗原图，支持重复调用；init 清空原图。


=== 现场记忆

奇度点须为零或二；从奇度点或有边点开始。边 id 标记已用，不能只删一个方向；最后反转。


=== 模板引用

#graph_code("euler_undirected.hpp")


== 线段树优化建图 <algo-graph-segment-graph>

*来源：* 原文 2.22。
*用途：* 大量维护点到区间、区间到点、区间到区间的有向边，常配合最短路 / 可达性。
*形式：* struct；两套区间编号与扩展邻接表长期共同维护，保留原 struct。


=== 复杂度与下标

- 建图 `O(n)`；`p2r / r2p` 每次 `O(log n)` 条边；`r2r` 每次新增 `1` 个点和 `O(log n)` 条边。
- 原图节点 `1..n`，辅助节点 `n+1..tot`。


=== 接口说明

- `p2p(u,v,w)`：`u -> v (w)`
- `p2r(u,L,R,w)`：`u -> 每个 x∈[L,R] (w)`
- `r2p(L,R,v,w)`：`每个 x∈[L,R] -> v (w)`
- `r2r(L1,R1,L2,R2,w)`：`任意 x∈[L1,R1] -> 任意 y∈[L2,R2] (w)`


=== 调用方式

```cpp
SegGraph sg(4);
sg.p2r(1, 2, 3, 5); sg.r2p(2, 3, 4, 1);
sg.r2r(2, 3, 3, 4, 2);
cout << sg.tot << " " << sg.g[1].size() << endl;
```


=== 注意事项

- 区间为合法闭区间 1\<=L\<=R\<=n；叶点复用原点，初始 tot=3\*n-2（n\>0）。
- r2r 增加一个中间节点：源区间到中间点带 w，中间点到目标区间带 0，权值只算一次。
- 计算范围是 1..tot，输出只看 1..n。本模板只负责建图；另复制前文 Dijkstra 后，可调用 `dijkstra(sg.g,s)`，边权须非负。
- init 清空图；n=0 只能初始化，不调用区间方法。建树递归深度 O(log n)。


=== 现场记忆

out 由区间向原点，in 由原点向区间。点到区间接 out，区间到点接 in；区间到区间用一个中间点。


=== 模板引用

#graph_code("segment_graph.hpp")


== 倍增优化树上路径建图 <algo-graph-jump-graph>

*来源：* 原文 2.23。
*用途：* 大量维护“点到树上路径”或“树上路径到点”的依赖边，常配合拓扑排序 / 可达性。
*形式：* struct；维护倍增表、路径块编号与辅助图，保留 struct；原树预处理改成递归 DFS 以遵守个人风格。


=== 复杂度与下标

- 预处理 `O(n log n)`；`p2path / path2p` 每次 `O(log n)` 条边；辅助图规模 `O(n log n)`。
- 原树节点 `1..n`，辅助节点 `n+1..tot`。


=== 接口说明

- `add_tree(u,v)`：加原树无向边
- `build(root)`：建倍增和辅助图，之后再加依赖
- `p2p(u,v)`：`u -> v`
- `p2path(s,u,v)`：`s -> path(u,v)` 上所有点
- `path2p(u,v,t)`：`path(u,v)` 上所有点 `-> t`
- `kth(u,k)`：`u` 的第 `k` 个祖先
- `lca(u,v)`：最近公共祖先
- `next_on_path(u,v)`：从 `u` 朝 `v` 走一步
- `topo()`：检查整个辅助图并返回原节点拓扑序


=== 调用方式

```cpp
JumpGraph jg(4);
jg.add_tree(1, 2); jg.add_tree(2, 3); jg.add_tree(3, 4);
jg.build(1);
jg.p2path(1, 2, 4);
auto order = jg.topo();
for (int u : order) cout << u << " ";
```


=== 注意事项

- 原输入必须是 n\>=1 的连通树，root 合法。DFS 深度最坏 O(n)，不要对任意图直接 build。
- 顺序固定 add\_tree -\> build -\> 添加依赖 -\> topo。再次 build 清除旧依赖，可切换树根。
- 路径两个端点都包含，源 / 目标点在路径上会产生自环；排除端点可用 next\_on\_path，要求 u!=v。
- kth 的 k 为合法祖先距离；不存在祖先用 0 哨兵。topo 检查全辅助图，返回的只是原节点。


=== 现场记忆

倍增块覆盖向上连续 2^k 个点。out 块到原点，in 原点到块。路径拆为 u 到 LCA（含）和 v 到 LCA 儿子（不再含 LCA）。


=== 模板引用

#graph_code("jump_graph.hpp")


== 最大流 - Dinic <algo-graph-dinic>

*来源：* 原文 2.24。
*用途：* 求有向网络最大流，并可在最大流后取得对应最小割的源点侧。
*形式：* struct；残量网络、层次和当前弧必须共同维护，使用 struct。


=== 复杂度与下标

- 一般图最坏时间 `O(V^2E)`，空间 `O(V+E)`。
- 节点 `1-indexed`。


=== 接口说明

- init(n)、add(u,v,cap)、add\_undirected(u,v,cap)、add\_node()。
- maxflow(s,t,lim=LLONG\_MAX)：返回本次增广量。
- reachable(s)：残量可达标记；disable(u,idx)：清零边与反向边。


=== 调用方式

```cpp
Dinic dinic(2);
dinic.add(1, 2, 7);
cout << dinic.maxflow(1, 2, 3) << endl;
cout << dinic.maxflow(1, 2) << endl;
auto side = dinic.reachable(1);
```


=== 注意事项

- 有向容量必须非负。add 返回正向边在 g[u] 内的下标；反向索引和边下标均为 0-index。支持重边、自环。
- 无向容量用两个方向初始 cap；需保证 2\*cap 能由 ll 表示，所有待求流量也需能由 ll 表示。
- 默认 lim 为 LLONG\_MAX；指定 lim 只限制本次新增流量。重复 maxflow 返回残量图的新增流，不是历史总流。
- 仅在流已完全求尽时，reachable(s) 才代表对应最小割源点侧。init 重建原图；s=t 返回零；递归 DFS 深度 O(V)。
- add\_node 扩展节点。disable 将一对残量边都清零，仅用于明确删辅助边的步骤。


=== 现场记忆

BFS 走 cap\>0 分层，DFS 只走下一层并用当前弧。正减反加；割看残量可达点。


=== 模板引用

#graph_code("dinic.hpp")


== 最小费用最大流 - SPFA <algo-graph-min-cost-flow-spfa>

*来源：* 原文 2.25。
*用途：* 求最大流及其最小费用；实现短，适合规模较小或费用图不大的题。
*形式：* struct；需要维护残量边及增广路径；势能版本还维护 h，保留原 struct。


=== 复杂度与下标

- 设共增广 `A` 次，每次 SPFA 最坏 `O(VE)`，总计最坏 `O(AVE)`；空间 `O(V+E)`。
- 节点 `1-indexed`。


=== 接口说明

- add(u,v,cap,cost)：反向边费用为 -cost。
- flow(s,t,lim=LLONG\_MAX)：返回 {本次流量,本次最小费用}。


=== 调用方式

```cpp
MCMF flow(3);
flow.add(1, 2, 2, -1); flow.add(2, 3, 2, 3);
auto [sent, cost] = flow.flow(1, 3, 2);
cout << sent << " " << cost << endl;
```


=== 注意事项

- 容量非负，允许负费用边；要求残量网络没有负费用环，本算法不做消圈。
- 有限最短距离、势能及改权距离的绝对值须小于 `INF=2^60`；中间加减和反向费用 `-cost` 不能溢出。增广量乘路径费用及累积总费用必须能放入 ll。
- 默认 lim 改为 LLONG\_MAX，指定 K 后需检查 sent==K。重复调用返回本次新增流与费用；同一残量图继续增广应保持同一源汇。
- init 清空网络；s=t 返回 {0,0}。两版类型名不同，可以同时包含。


=== 现场记忆

每轮 SPFA 找最短增广路、找瓶颈、正减反加；费用加 f\*dis[t]。


=== 模板引用

#graph_code("min_cost_flow_spfa.hpp")


== 最小费用最大流 - 势能 Dijkstra / Johnson <algo-graph-min-cost-flow-potential>

*来源：* 原文 2.26。
*用途：* 用势能重标号把残量边费用变为非负，再用 Dijkstra 求最短增广路；适合较大的最小费用流。
*形式：* struct；需要维护残量边及增广路径；势能版本还维护 h，保留原 struct。


=== 复杂度与下标

- 初始势能 SPFA 最坏 `O(VE)`；之后每次增广 `O(E log V)`，共增广 `A` 次，总计 `O(VE + AE log V)`；空间 `O(V+E)`。
- 节点 `1-indexed`。


=== 接口说明

- add(u,v,cap,cost)：反向边费用为 -cost。
- flow(s,t,lim=LLONG\_MAX)：返回 {本次流量,本次最小费用}。
- h[u]：当前 Johnson 势能。


=== 调用方式

```cpp
MCMFPotential flow(3);
flow.add(1, 2, 2, -1); flow.add(2, 3, 2, 3);
auto [sent, cost] = flow.flow(1, 3, 2);
cout << sent << " " << cost << endl;
```


=== 注意事项

- 容量非负，允许负费用边；要求残量网络没有负费用环，本算法不做消圈。
- 有限最短距离、势能及改权距离的绝对值须小于 `INF=2^60`；中间加减和反向费用 `-cost` 不能溢出。增广量乘路径费用及累积总费用必须能放入 ll。
- 默认 lim 改为 LLONG\_MAX，指定 K 后需检查 sent==K。重复调用返回本次新增流与费用；同一残量图继续增广应保持同一源汇。
- init 清空网络；s=t 返回 {0,0}。两版类型名不同，可以同时包含。


=== 现场记忆

初始 SPFA 得 h；改权 cost+h[u]-h[v]。Dijkstra 后只给可达点 h+=dis，真实费用 h[t]-h[s]。


=== 模板引用

#graph_code("min_cost_flow_potential.hpp")


== 最小割树 - Gomory-Hu <algo-graph-gomory-hu>

*来源：* 原文 2.27。
*用途：* 用一棵树表示无向图任意两点之间的最小割；树上 `s-t` 路径最小边权就是原图 `s-t` 最小割。
*形式：* struct；多次最大流与割树需要共同管理，保留 struct，复用 Dinic。


=== 复杂度与下标

- 建树需要 `n-1` 次最大流，即 `O(n · T_maxflow)`；当前 `query()` 单次 `O(n)`；空间 `O(n+m)`。
- 节点 `1-indexed`。


=== 接口说明

- `add(u,v,w)`：加无向容量边
- `build()`：建 Gomory-Hu Tree
- `query(u,v)`：查询原图 `u-v` 最小割，要求 `u != v`
- `tree`：最小割树
- `fa[u]`：构造过程中 `u` 的父亲
- `cut[u]`：最终树边 `(u,fa[u])` 的权值


=== 调用方式

```cpp
GomoryHu gh(3);
gh.add(1, 2, 3); gh.add(2, 3, 2);
gh.build();
cout << gh.query(1, 3) << endl;
```


=== 注意事项

- 只用于无向非负容量图，忽略自环、允许重边。无向残量边与流量的 ll 限制同 Dinic。
- 先 build，再 query；要求两个合法点且 u!=v。不连通点对最小割为零。
- 每次 build 都从原边重新建立网络，可重复构造；添加原边后须重新 build。init 清图；PDF 已展开 Dinic。
- query 初始瓶颈改成 LLONG\_MAX，避免大割值被 2^60 截断。


=== 现场记忆

原图两点最小割等于割树路径的最小边权。做 n-1 次最大流，用残量可达侧改父亲。


=== 模板引用

#graph_code("gomory_hu.hpp", deps: ("dinic.hpp",))


== 全局最小割 - Stoer-Wagner <algo-graph-stoer-wagner>

*来源：* 原文 2.28。
*用途：* 求无向带权图所有非平凡割中的最小割权值，不需要指定源点和汇点。
*形式：* 函数式；每次求全局割时复制矩阵并逐轮收缩，使用函数。


=== 复杂度与下标

- 时间 `O(n^3)`，空间 `O(n^2)`。
- 节点 `1-indexed`，邻接矩阵为 `g[1..n][1..n]`。


=== 接口说明

- stoer\_wagner(g)：返回最小非平凡割权值；矩阵 0 行列保留。


=== 调用方式

```cpp
vector<vector<ll>> g(4, vector<ll>(4));
g[1][2] = g[2][1] = 3;
g[2][3] = g[3][2] = 2;
cout << stoer_wagner(g) << endl;
```


=== 注意事项

- 输入无向对称方阵，权非负，重边累加；自环不影响割。n\<=1 返回零。
- 图不连通时全局割为零；不是有向最小割。矩阵按值复制，重复调用不修改原矩阵。
- 收缩时会累加边权，所有累积值需能放入 ll；总非自环边权小于 LLONG\_MAX 是充分条件。初始答案改为 LLONG\_MAX。


=== 现场记忆

每轮不断选到当前集合连接权最大的点；最后两点 pre、sel，dis[sel] 更新割，然后把 sel 合并到 pre。


=== 模板引用

#graph_code("stoer_wagner.hpp")


== 0-1 BFS <algo-graph-zero-one-bfs>

*来源：* 原文 2.29。
*用途：* 求边权只有 `0/1` 的图上单源最短路。
*形式：* 函数式；deque 上的单次最短路，不建立 struct。


=== 复杂度与下标

- 时间 `O(n+m)`，空间 `O(n+m)`
- 节点 `1 ~ n`


=== 接口说明

- zeroOneBFS(g,s)：返回 1-index 的距离数组。


=== 调用方式

```cpp
vector<vector<pair<int, int>>> g(4);
g[1].push_back({2, 1}); g[1].push_back({3, 0});
g[3].push_back({2, 0});
auto dis = zeroOneBFS(g, 1);
cout << dis[2] << endl;
```


=== 注意事项

- 边权只允许 0/1，源点合法。无向边手动加两个方向。
- 不可达是 INF=2^60，函数每次独立初始化，不修改图。
- 0-1 BFS 按边权使用 deque；普通 BFS 仍使用 queue，不把一般权图套进本模板。


=== 现场记忆

零边放队首，一边放队尾；只在距离严格变小时入队。


=== 模板引用

#graph_code("zero_one_bfs.hpp")


== 二分图最大匹配 Hopcroft-Karp <algo-graph-hopcroft-karp>

*来源：* 原文 2.30。
*用途：* 求二分图最大匹配，比普通增广路适合较大的稀疏二分图。
*形式：* struct；匹配、分层与当前弧共享状态，使用 struct。


=== 复杂度与下标

- 令 V=nL+nR，时间 `O((V+m)√V)`，省略孤点初始化成本时常写 `O(m√V)`；空间 `O(V+m)`。
- 左部点 `1 ~ nL`，右部点 `1 ~ nR`


=== 接口说明

- `add(u,v)`：左部 `u` 连右部 `v`
- `work()`：最大匹配数
- `matchL[u]`：左部 `u` 匹配的右点
- `matchR[v]`：右部 `v` 匹配的左点


=== 调用方式

```cpp
HopcroftKarp hk(2, 2);
hk.add(1, 1); hk.add(1, 2); hk.add(2, 1);
cout << hk.work() << endl;
cout << hk.matchL[1] << endl;
```


=== 注意事项

- 只用于二分图，add 左点到右点；重边允许，未匹配为零。空左部或右部匹配数为零。
- BFS 使用 queue；保留原分层增广思路，补齐最短终层与 DFS 当前弧，使每轮沿最短增广路。
- work 每次清空匹配并重算，重复调用或追加边后调用均可；多测重新构造。增广 DFS 深度最坏 O(nL+nR)。


=== 现场记忆

所有未匹配左点同时分层；dis[0] 是最短未匹配右点层。DFS 只走下一层，匹配后同时更新 matchL / matchR。


=== 模板引用

#graph_code("hopcroft_karp.hpp")


== KM / Hungarian Assignment <algo-graph-hungarian>

*来源：* 原文 2.31。
*用途：* 解决带权二分图 Assignment Problem；代码求每个左点恰好匹配一个不同右点的最小总代价。
*形式：* 函数式；顶标与匹配都是求解过程的局部状态，去掉无成员外壳，保留原匈牙利流程。


=== 复杂度与下标

- 矩形 `n <= m` 时 `O(n²m)`，方阵为 `O(n³)`；额外空间 `O(n+m)`，输入矩阵空间 `O(nm)`。
- 输入矩阵 `0-indexed`


=== 接口说明

- hungarian(cost)：返回 {最小总代价,左点到右点的匹配数组}。


=== 调用方式

```cpp
vector<vector<ll>> cost{{4, 1, 3}, {2, 0, 5}};
auto [ans, match] = hungarian(cost);
cout << ans << " " << match[0] << endl;
```


=== 注意事项

- 输入是矩形完整代价矩阵，n\<=m；返回每个左点匹配的右点 0-index。空矩阵返回 {0,{}}。
- 最大权时先把权值取负，最后对答案取负；注意最小负数不能取负。n\>m 可转置，但结果语义要相应转换。
- 每个矩阵位置都视为可匹配；缺边用足够大的有限代价表示，随后检查是否选了缺边。
- 顶标、松弛量及总答案须在 ll 范围，建议 n\*max|cost| 远小于 INF=2^60。不改变输入，可重复调用。


=== 现场记忆

维护 u/v 顶标、p 匹配与 way 前驱；寻找最小 slack 调整顶标，再沿 way 增广。


=== 模板引用

#graph_code("hungarian.hpp")


== 上下界可行流 <algo-graph-lower-bound-flow>

*来源：* 原文 2.32。
*用途：* 判断每条边流量限制为 `[low, high]` 时，是否存在满足流量守恒的可行流，并恢复各边流量。
*形式：* struct；余额、原边恢复信息及求解状态共同维护，使用 struct，复用 Dinic。


=== 复杂度与下标

- 一次 Dinic，通常 `O(V²E)`；空间 `O(V+E)`
- 节点 `1 ~ n`


=== 接口说明

- `add(u,v,low,high)`：加上下界边，返回边编号
- `work()`：判断是否存在可行流
- `edgeFlow(id)`：可行流中原边实际流量


=== 调用方式

```cpp
LowerBoundFlow flow(2);
int id = flow.add(1, 2, 1, 3);
flow.add(2, 1, 1, 3);
if (flow.work()) cout << flow.edgeFlow(id) << endl;
```


=== 注意事项

- 0\<=low\<=high，high、余额与全部所需流量须能放入 ll。支持自环与重边。超级源 S=n+1，汇 T=n+2。
- 必须加完全部边后 work；成功后才能 edgeFlow。原边编号从 0 开始，返回的是实际流量。
- 重复 work 返回缓存，不重复加超级边或增广；求解后禁止再 add，多测重新构造。PDF 已展开修正自环反向编号的 Dinic。


=== 现场记忆

容量 high-low，bal[u]-=low、bal[v]+=low；正余额接 S，负余额接 T。可行当最大流等于 need；恢复 low+cap-剩余。


=== 模板引用

#graph_code("lower_bound_flow.hpp", deps: ("dinic.hpp",))


== 上下界最大流 <algo-graph-lower-bound-max-flow>

*来源：* 原文 2.33。
*用途：* 在每条边具有流量上下界时，求 `s -> t` 的最大可行流。
*形式：* struct；维护余额及两阶段残量网络，使用 struct，沿用 t-\>s 转循环流。


=== 复杂度与下标

- 两次 Dinic，通常 `O(V²E)`；空间 `O(V+E)`
- 节点 `1 ~ n`


=== 接口说明

- add(u,v,low,high)：加入上下界边。
- work(s,t)：返回最大非负可行流，或 -1；同源汇重复调用返回缓存。


=== 调用方式

```cpp
LowerBoundMaxFlow flow(2);
flow.add(1, 2, 1, 4);
cout << flow.work(1, 2) << endl;
```


=== 注意事项

- 求非负 s-\>t 可行流，源汇不同；若只有负的净流量可行，本模板仍视为无可行非负流。失败返回 -1。
- 0\<=low\<=high；余额、流量和累加值须能放入 ll。回边容量 INF=2^62，所有 high 之和小于 INF 是安全的充分条件。
- 超级源汇是 n+1、n+2。可行后必须同时清除每条辅助边及其反向边，再从 s 到 t 增广。
- 重复 work 只允许同一 s,t，返回缓存；求解后不能 add，换源汇或多测重新构造。


=== 现场记忆

加 t-\>s(INF)，跑超级源汇求可行流；回边已流量是 base。删除超级边与回边，两次都是正反一起删；答案 base+新增流。


=== 模板引用

#graph_code("lower_bound_max_flow.hpp", deps: ("dinic.hpp",))


== 最大权闭合子图 <algo-graph-max-weight-closure>

*来源：* 原文 2.34。
*用途：* 选择若干点最大化权值和，同时满足依赖关系 `选 u => 必须选 v`。
*形式：* struct；保留点权与依赖的建模接口，局部重建流网络，使用 struct。


=== 复杂度与下标

- 一次最大流；Dinic 通常 `O(V²E)`
- 节点 `1 ~ n`


=== 接口说明

- `addWeight(u,w)`：给点 `u` 增加权值
- `addDependency(u,v)`：`选 u => 必须选 v`
- `work()`：返回 `{最大权值, 选择的点}`


=== 调用方式

```cpp
MaxWeightClosure closure(2);
closure.addWeight(1, 5); closure.addWeight(2, -2);
closure.addDependency(1, 2);
auto [ans, chosen] = closure.work();
cout << ans << endl;
```


=== 注意事项

- 正权总和必须严格小于 INF=2^62，保证依赖边不会被最优割切断；点权累加及负权取负须能放入 ll。
- 原点 1..n，超级源为 0，超级汇 n+1；空图、全负权、依赖成环均允许。
- work 每次用原始权值与依赖重建网络，可重复运行，也可修改建模数据后重算；多测重新构造。
- chosen 从最小割源点侧恢复，可能存在多个最优集合，不应只比较唯一点集。


=== 现场记忆

正权 S-\>u，负权 u-\>T，依赖 u-\>v(INF)。答案正权和减最小割，残量从 S 可达的原点就是选择集合。


=== 模板引用

#graph_code("max_weight_closure.hpp", deps: ("dinic.hpp",))
