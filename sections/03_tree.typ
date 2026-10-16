= 树论 <chapter-tree>

本章完整迁移 `raw/03_tree.md` 的 9 项算法，显示编号由当前位置自动生成。
普通最小生成树参见 #ref(<algo-graph-kruskal>, supplement: [算法])，重构树保留在本章。
均保留轻量 struct：它们需要跨多次查询或递归维护关联状态，不额外添加 getter / setter、通用图类或嵌套封装。
节点默认 `1..n`，`0` 为哨兵；询问编号、重构树辅助节点等例外在各节注明。n 非负，n+1 及数组长度须能由 int 表示并满足内存限制。标准头文件及 ll 兜底宏随源码展示，复制个人基础模板和所需实现即可使用，不依赖仓库头文件。

*统一约定：* 除 KRT 的一般无向图和 LCT 的动态森林外，输入须是合法连通树（无环、无重边），不做额外合法性扫描。递归 DFS 保持原习惯，链形大树须评估栈空间；LCT 保持原有迭代 Splay。n=0 仅支持初始化及注明的空构建 / 求解，不代表可查询哨兵。所有节点编号、树边和根参数须合法。

#let tree_code(file) = {
  let source = read("../code/tree/" + file)
  source = source.replace(regex("(?m)^#pragma once\r?\n"), "")
  raw(source, lang: "cpp", block: true)
}


== 倍增 LCA <algo-tree-lca>

*来源：* 原文 3.1。

*用途*：在线查询树上两点最近公共祖先，也可查询节点的第 `k` 级祖先。
*复杂度*：预处理 `O(n log n)`，单次查询 `O(log n)`，空间 `O(n log n)`。
*下标*：节点 `1-indexed`，`0` 表示不存在的祖先。
*限制*：输入必须是一棵树。
*注意*：递归 DFS 在极深链上可能爆栈。

*形式：* 维护邻接表、深度和倍增表，保留 struct LCA。

=== 核心接口

#figure(align(center)[#table(
  columns: 2,
  inset: 6pt,
  table.header([接口 / 变量], [含义]),
  [`init(n)`], [初始化],
  [`add(u, v)`], [加无向树边],
  [`build(root)`], [以 `root` 为根预处理],
  [`lca(u, v)`], [最近公共祖先],
  [`kth_up(u, k)`], [向上跳 `k` 条边，负数或越过根返回 `0`],
  [`dep[u]`], [`u` 的深度],
  [`fa[k][u]`], [`u` 的 `2^k` 级祖先],
)])

=== 最小调用示例

```cpp
LCA lca(3);
lca.add(1, 2); lca.add(1, 3);
lca.build(1);
assert(lca.lca(2, 3) == 1);
assert(lca.kth_up(2, 1) == 1);
assert(lca.kth_up(2, 2) == 0);
```

=== 注意事项与修正

构造和 init 即分配哨兵。build 可重复换根；init 清空旧边。n=0 时 build 不做 DFS，不能查询节点。kth_up 的 k 使用 int；负数或跳过根返回 0，k=0 返回原节点。所有查询须在非空树 build 后进行。

=== 现场记忆

`fa[k][u] = fa[k-1][fa[k-1][u]]`

先把深度大的点跳到同一深度。

若已经相等，当前点就是 LCA。

否则从大到小枚举 `k`，只在 `fa[k][u] != fa[k][v]` 时一起跳。

最后答案是 `fa[0][u]`。

=== 模板引用

#tree_code("lca.hpp")


== 树剖 LCA <algo-tree-lca-hld>

*来源：* 原文 3.2。

*用途*：用重链剖分预处理树，在线查询两点最近公共祖先。
*复杂度*：预处理 `O(n)`，单次查询 `O(log n)`，空间 `O(n)`。
*下标*：节点 `1-indexed`。
*限制*：输入必须是一棵树。
*注意*：递归 DFS 在极深链上可能爆栈。

*形式：* 两次 DFS 共用 fa / dep / siz / son / top，保留 struct LCAHLD。

=== 核心接口

#figure(align(center)[#table(
  columns: 2,
  inset: 6pt,
  table.header([接口 / 变量], [含义]),
  [`init(n)` / `add(u,v)`], [清空状态 / 加无向树边],
  [`build(root)`], [重算深度、子树大小、重儿子和链顶],
  [`lca(u,v)`], [沿链顶跳转，返回 LCA],
  [`fa / dep / siz / son / top`], [父节点 / 深度 / 子树大小 / 重儿子 / 链顶],
)])

=== 最小调用示例

```cpp
LCAHLD lca(3);
lca.add(1, 2); lca.add(1, 3);
lca.build(1);
assert(lca.lca(2, 3) == 1);
lca.build(2);
assert(lca.lca(2, 3) == 2);
```

=== 注意事项与修正

build 每次先清空 son，支持在同一棵树上换根重算；init 清空旧边。n=0 允许空 build，不允许节点查询。深度根为 1，fa[root]=0；siz[0]=0 用于比较重儿子。

=== 现场记忆

`son[u]` = 子树最大的儿子，重儿子和父亲共处一条重链。

不同链时，让 `top` 更深的一边跳：

`u = fa[top[u]]`

两点进入同一条链后，深度更浅的点就是 LCA。

重新 `build()` 时记得清空 `son`。

=== 模板引用

#tree_code("lca_hld.hpp")


== Tarjan 离线 LCA <algo-tree-lca-tarjan>

*来源：* 原文 3.3。

*用途*：一次性求出树上一批已知询问的最近公共祖先。
*复杂度*：时间 `O((n+q) α(n))`，空间 `O(n+q)`。
*下标*：节点 `1-indexed`，询问编号 `0-indexed`。
*限制*：询问必须在执行 `work()` 前加入；输入必须是一棵树。
*注意*：递归 DFS 在极深链上可能爆栈。

*形式：* 同时维护离线询问、并查集和访问标记，保留 struct LCATarjan。

=== 核心接口

#figure(align(center)[#table(
  columns: 2,
  inset: 6pt,
  table.header([接口 / 变量], [含义]),
  [`init(n)`], [初始化],
  [`add(u, v)`], [加无向树边],
  [`ask(u, v)`], [加入询问并返回询问编号],
  [`work(root)`], [执行全部离线询问],
  [`ans[id]`], [第 `id` 个询问的 LCA],
)])

=== 最小调用示例

```cpp
LCATarjan lca(3);
lca.add(1, 2); lca.add(1, 3);
int id = lca.ask(2, 3);
const auto &ans = lca.work(1);
assert(ans[id] == 1);
assert(lca.work(2)[id] == 2);
```

=== 注意事项与修正

work 每次重置 DSU、祖先、vis 和答案，已有询问会保留并重新计算；新增询问后可再次 work。init 同时清空边和询问。空结构返回仅含已有答案槽位的 ans；n=0 不应 ask。返回引用在下一次 ask / init 后可能失效；要保留旧结果应复制。

=== 现场记忆

DFS 完一个儿子后：

`merge(u, v)`，再令 `anc[find(u)] = u`。

`vis[v] = 1` 表示 `v` 的整棵子树已经处理完成。

处理询问 `(u, v)` 时，只有 `v` 已完成才回答：

`ans = anc[find(v)]`

查询挂在两个端点各一份，但只会在合法时机得到同一个答案。

=== 模板引用

#tree_code("lca_tarjan.hpp")


== 重链剖分 HLD <algo-tree-hld>

*来源：* 原文 3.4。

*用途*：将树上路径拆成若干个连续 DFS 序区间，并将子树映射为一个连续区间；具体区间操作交给线段树、Fenwick 等数据结构处理。

*复杂度*：预处理 `O(n)`；`path()` / `path_edge()` 产生 `O(log n)` 个区间；`lca()` `O(log n)`；`subtree()` `O(1)`；空间 `O(n)`。

*下标*：节点、DFS 序均为 `1-indexed`。

*注意*：
- `path()` 只保证完整覆盖路径，不保证回调顺序等于 `u -> v` 的路径顺序；非交换信息需要额外处理方向。
- `path_edge()` 默认将边权放在较深端点的 `dfn` 上，因此会排除 LCA。
- 两次 DFS 使用递归，极深链上可能爆栈。

*形式：* 剖分状态长期用于路径和子树回调，保留 struct HLD；不内置线段树。

=== 核心接口

#figure(align(center)[#table(
  columns: 2,
  inset: 6pt,
  table.header([接口 / 变量], [含义]),
  [`init(n)`], [初始化],
  [`add(u,v)`], [加无向树边],
  [`build(root)`], [完成重链剖分],
  [`path(u,v,f)`], [将点路径拆成若干 `[l,r]`，调用 `f(l,r)`],
  [`path_edge(u,v,f)`], [将边路径拆成若干 `[l,r]`],
  [`subtree(u)`], [返回 `u` 子树的 DFS 序区间],
  [`lca(u,v)`], [最近公共祖先],
  [`fa[u]`], [父节点],
  [`dep[u]`], [深度],
  [`siz[u]`], [子树大小],
  [`son[u]`], [重儿子],
  [`top[u]`], [所在重链链顶],
  [`dfn[u]`], [DFS 序],
)])

=== 最小调用示例

```cpp
HLD hld(3);
hld.add(1, 2); hld.add(1, 3);
hld.build(1);
vector<ll> val = {0, 10, 20, 30}, a(4);
for (int u = 1; u <= 3; u++)
{
    a[hld.dfn[u]] = val[u];
}
ll ans = 0;
hld.path(2, 3, [&](int l, int r)
{
    // 演示接区间；正式查询可换为外部 seg.query(l, r)。
    for (int i = l; i <= r; i++)
    {
        ans += a[i];
    }
});
assert(ans == 60);
int edges = 0;
hld.path_edge(2, 3, [&](int l, int r)
{
    edges += r - l + 1;
});
assert(edges == 2);
auto [l, r] = hld.subtree(1);
assert(l == 1 && r == 3 && hld.lca(2, 3) == 1);
```

=== 注意事项与修正

节点和闭区间均从 1 开始。build 重置 timer / son，可重复换根；init 清空旧边。n=0 只允许空 build。回调内部的区间操作开销另算：接 O(log n) 的 seg / Fenwick 后，路径操作通常为 O(log^2 n)。示例中的数组扫描仅说明接口，不代表此复杂度。点权按 a[dfn[u]] 排列；边权放在较深端点。子树接口可接 seg.modify(l,r,w)；Fenwick 区间和可接 bit.query(r)-bit.query(l-1)。

=== 现场记忆

```text
dfs2 重儿子优先 -> 一条重链连续，整棵子树也连续。

path：
处理 [dfn[top[u]], dfn[u]]
然后 u = fa[top[u]]

同链后处理 [dfn[u], dfn[v]]

subtree(u) = [dfn[u], dfn[u] + siz[u] - 1]

边权放到深端点，path_edge 最后一段排除 LCA。

path() 不保证 u -> v 顺序；非交换合并要单独处理方向。
```

=== 模板引用

#tree_code("hld.hpp")


== 虚树 <algo-tree-virtual-tree>

*来源：* 原文 3.5。

*用途*：把一批关键点及其必要 LCA 压缩成一棵 `O(k)` 大小的树，方便只在关键结构上做树形 DP。
*复杂度*：预处理 `O(n log n)`；设关键点数为 `k`，单次构建详见下方重复点与清理开销约定，虚树节点数不超过 `2k - 1`。
*下标*：原树节点 `1-indexed`。
*注意*：`vt` 中的边为祖先到儿子的有向边；每次 `build()` 会自动清空上一棵虚树。递归 DFS 在极深链上可能爆栈。

*形式：* 同时维护原树预处理和多次构建的虚树，保留 struct VTree。

=== 核心接口

#figure(align(center)[#table(
  columns: 2,
  inset: 6pt,
  table.header([接口 / 变量], [含义]),
  [`init(n)`], [初始化],
  [`add(u, v)`], [加原树无向边],
  [`prep(root)`], [预处理 DFS 序和倍增 LCA],
  [`lca(u, v)`], [原树上的最近公共祖先],
  [`build(key)`], [构建虚树并返回虚树根，空集返回 `0`],
  [`vt[u]`], [虚树中 `u` 的儿子],
  [`nodes`], [当前虚树全部节点],
  [`dep[u]`], [原树深度],
  [`dfn[u]`], [原树 DFS 序],
)])

=== 最小调用示例

```cpp
VTree vt(3);
vt.add(1, 2); vt.add(1, 3);
vt.prep(1);
int root = vt.build({2, 3, 2});
assert(root == 1 && vt.nodes.size() == 3);
int cnt = 0;
function<void(int)> dfs = [&](int u)
{
    cnt++;
    for (int v : vt.vt[u])
    {
        dfs(v);
    }
};
if (root)
{
    dfs(root);
}
assert(cnt == 3);
assert(vt.build({}) == 0);
```

=== 注意事项与修正

prep 可重复换根，并清掉旧虚树；原树改变后必须重新 prep。build 会清除上一轮 O(k_prev) 个虚树节点的边。设传入数组长度为 s（可重复），构建为 O(s log s + s log n)，另加旧树清理；去重后的关键点数 k>0 时节点数至多 2k-1。空 key 返回 0，调用方不要 dfs(0)。vt[u] 仅存向下边；压缩边长度可由 dep[v]-dep[u] 得到。

=== 现场记忆

关键点先按 `dfn` 排序，只需补相邻关键点的 LCA。

补完 LCA 再排序去重，虚树节点数最多 `2k - 1`。

祖先判断：

`dfn[u] <= dfn[v] < dfn[u] + siz[u]`

节点按 DFS 序扫描，栈顶不是当前点祖先就弹栈。

最终 `vt[parent].push_back(child)`，直接按原树祖先方向做 DP。

=== 模板引用

#tree_code("virtual_tree.hpp")


== 启发式合并 / DSU on Tree：子树不同颜色数 <algo-tree-dsu-on-tree>

*来源：* 原文 3.6。

*用途*：统计以每个节点为根的子树中不同颜色的数量。
*复杂度*：时间 `O(n log n)`，空间 `O(n)`。
*下标*：节点 `1-indexed`。
*注意*：颜色值可为任意 `int`，`work()` 内部自动离散化；递归在链形树上可能爆栈。

*形式：* 子树大小、重儿子、颜色频次及清理流程共享状态，保留 struct DSUOnTree。

=== 核心接口

#figure(align(center)[#table(
  columns: 2,
  inset: 6pt,
  table.header([接口 / 变量], [含义]),
  [`init(n)` / `add(u,v)`], [清空状态 / 加无向树边],
  [`set_color(u,c)`], [设置原始 int 颜色，可重复设置],
  [`work(root)`], [重做离散化并返回全部答案的引用],
  [`color[u]` / `ans[u]`], [原始颜色 / 子树不同颜色数],
  [`diff`], [当前临时统计，work 结束时已清零],
)])

=== 最小调用示例

```cpp
DSUOnTree dsu(3);
dsu.add(1, 2); dsu.add(1, 3);
dsu.set_color(1, -7);
dsu.set_color(2, 9); dsu.set_color(3, 9);
const auto &ans = dsu.work(1);
assert(ans[1] == 2 && ans[2] == 1);
dsu.set_color(1, 9);
assert(dsu.work(1)[1] == 1);
```

=== 注意事项与修正

每次 work 重做颜色离散化，支持负颜色、INT_MIN / INT_MAX、重复颜色和换根；color 保留原始值。work 完成时临时计数已全部删除，diff=0，便于再次调用。init 清空边和颜色；n=0 返回大小为 1 的哨兵答案数组。保留答案请复制，返回引用会随后续 work 更新。

=== 现场记忆

先算 `siz / son`。

轻儿子：递归求完后清空。

重儿子：最后递归，并保留统计结果。

再把所有轻儿子整棵加入，最后加入 `u`：

`ans[u] = 当前统计答案`

`keep = false` 时离开前把整棵 `u` 子树清空。

=== 模板引用

#tree_code("dsu_on_tree.hpp")


== 点分治：统计距离不超过 k 的点对数 <algo-tree-centroid>

*来源：* 原文 3.7。

*用途*：统计加权树中距离不超过 `k` 的无序点对数量。
*复杂度*：时间 `O(n log^2 n)`，空间 `O(n)`。
*下标*：节点 `1-indexed`。
*注意*：距离和 `k` 使用 `long long`；递归在极深树上可能爆栈。

*形式：* 分治过程共享删除标记、子树大小和答案，保留 struct Centroid。

=== 核心接口

#figure(align(center)[#table(
  columns: 2,
  inset: 6pt,
  table.header([接口 / 变量], [含义]),
  [`init(n,k)` / `add(u,v,w)`], [清空状态 / 加无向带权树边],
  [`work()`], [重置删除标记并返回无序异点对数],
  [`k`], [距离上限，可改后再次 work],
  [`g`], [简单带权邻接表；边权为 ll],
)])

=== 最小调用示例

```cpp
Centroid cd(3, 2);
cd.add(1, 2, 1); cd.add(2, 3, 2);
assert(cd.work() == 2);
cd.k = 3;
assert(cd.work() == 3);
```

=== 注意事项与修正

只统计 u!=v 的无序点对；单点和空树答案为 0。保留排序双指针与容斥思路，无距离剪枝，允许负边和负 k。work 清空 removed / ans，支持重算及修改 k；init 清空旧树。原代码只在双指针比较时转宽，collect 中 dis+w 仍可溢出；本版内部距离及比较统一用 `__int128`，依赖支持该扩展的 GCC 目标（GNU++17）。返回点对数用 ll，int 范围内节点数的最大点对数可容纳。

=== 现场记忆

对当前重心 `c`：

`所有经过 c 的合法点对 = count(all) - Σ count(subtree)`

其中距离都表示节点到 `c` 的距离，`all` 额外加入重心自身的距离 `0`。

`count()`：排序后双指针；若 `d[l] + d[r] <= k`，则当前 `l` 能和 `l+1 ... r` 全部配对。

统计完删除重心，再递归每个剩余连通块。

本实现每层需要排序距离数组，因此总复杂度 `O(n log^2 n)`。

=== 模板引用

#tree_code("centroid.hpp")


== Kruskal 重构树 <algo-tree-kruskal-tree>

*来源：* 原文 3.8。

*用途*：处理最小瓶颈、边权阈值下的连通块，将问题转成重构树上的 LCA / 祖先 / 子树问题。
*复杂度*：构树 `O(m log m)`，倍增 `O(n log n)`；`lca / climb` 单次 `O(log n)`，空间 `O(n log n + m)`。
*下标*：原图节点 `1..n`，重构节点 `n+1..tot`。
*注意*：边权从小到大构造；原图不连通时得到森林，`bottleneck(u,v)` 要求两点连通。

*形式：* 构树、森林归属、DFS 序和倍增查询共享状态，保留 struct KRT。

=== 核心接口

#figure(align(center)[#table(
  columns: 2,
  inset: 6pt,
  table.header([接口 / 变量], [含义]),
  [`add(u,v,w)`], [加无向边],
  [`build()`], [建 Kruskal 重构森林并预处理倍增],
  [`connected(u,v)`], [判断两点最终是否连通],
  [`lca(u,v)`], [重构树 LCA；不连通返回 `0`],
  [`bottleneck(u,v)`], [两点最小瓶颈值],
  [`climb(u,x)`], [最高的 `val <= x` 祖先],
  [`val[u]`], [重构点对应边权],
  [`siz[u]`], [子树中的原图节点数],
  [`dfn[u]`], [原图节点 DFS 序区间左端点],
  [`bel[u]`], [所在重构树的根],
  [`roots`], [重构森林所有根],
)])

=== 最小调用示例

```cpp
KRT krt(4);
krt.add(1, 2, -3); krt.add(2, 3, 5);
krt.build();
assert(krt.bottleneck(1, 3) == 5);
assert(!krt.connected(1, 4) && krt.lca(1, 4) == 0);
int p = krt.climb(1, -3);
assert(krt.siz[p] == 2);
int l = krt.dfn[p], r = l + krt.siz[p] - 1;
assert(r - l + 1 == 2);
```

=== 注意事项与修正

build 可以重复执行，init 清空旧边和 roots；查询须在 build 后进行。支持自环、重边、不连通、等权及负权；不同连通块 lca 返回 0，bottleneck 仅对已连通节点调用。同点瓶颈约定为 0（空路径），不能把它当作负边的最大值单位元。climb 从原图节点出发，只检查重构祖先的阈值，叶子 val=0 不限制负阈值查询。总点数至多 2n-1；须保证 2n+5 能由 int 表示，且倍增表内存足够。递归 DFS 与 DSU find 在退化合并链上可能深达 O(n)，原稿提到的按编号迭代下传是另一种可选实现，本版保留递归。

=== 现场记忆

Kruskal 从小到大合并两个连通块根 `fu, fv`，新建点 `p`，`val[p] = w`。

新点编号大于儿子，可另写按编号迭代下传；本模板实际保留递归 DFS，须注意退化链栈深。

`val[lca(u,v)]` = 两点连通所需的最小边权阈值 = 最小瓶颈值。

限制边权 `<= x`：从原点倍增向上跳到最高的 `val <= x` 祖先。

该祖先对应当前连通块，大小为 `siz[p]`，原图点区间为 `[dfn[p], dfn[p] + siz[p] - 1]`。

=== 模板引用

#tree_code("kruskal_tree.hpp")


== LCT：动态森林 / 路径加 / 路径和 <algo-tree-link-cut-tree>

*来源：* 原文 3.9。

*用途*：维护动态森林，支持动态连边、删边，以及树链上的点权加、点权和。
*复杂度*：`link / cut / connected / set_val / path_add / path_sum` 均摊 `O(log n)`，空间 `O(n)`。
*下标*：节点 `1-indexed`，`0` 为哨兵空节点。
*注意*：只能维护森林，`link(u,v)` 不允许形成环；`cut(u,v)` 只删除原树中的直接边；`split / path_add / path_sum` 要求 `u,v` 已连通。

*形式：* Splay、懒标记和动态森林操作共享数组，保留 struct LCT；Splay 原有迭代实现不变。

=== 核心接口

#figure(align(center)[#table(
  columns: 2,
  inset: 6pt,
  table.header([接口], [含义]),
  [`init(n)`], [初始化 / 清空],
  [`set_val(u, x)`], [将节点 `u` 权值设为 `x`],
  [`connected(u, v)`], [判断两点是否连通],
  [`link(u, v)`], [连边，已连通返回 `false`],
  [`cut(u, v)`], [删除直接边 `(u,v)`，不存在返回 `false`],
  [`makeroot(u)`], [将 `u` 设为所在原树的根],
  [`split(u, v)`], [提取路径 `u -> v`，之后 `v` 的 Splay 即整条路径],
  [`path_add(u, v, x)`], [路径所有节点加 `x`],
  [`path_sum(u, v)`], [查询路径节点权值和],
)])

=== 最小调用示例

```cpp
LCT lct(3);
lct.set_val(1, 1); lct.set_val(2, 2); lct.set_val(3, 3);
assert(lct.link(1, 2) && lct.link(2, 3));
assert(!lct.link(1, 3));
if (lct.connected(1, 3))
{
    lct.path_add(1, 3, 4);
    assert(lct.path_sum(1, 3) == 18);
}
assert(!lct.cut(1, 3));
assert(lct.cut(2, 3) && !lct.connected(1, 3));
```

=== 注意事项与修正

保留原版迭代 Splay / access。`init` 清空森林及懒标记，`siz[0]=0`；空结构仅可初始化，合法节点为 `1..n`。`link` 拒绝已连通节点（含自环）；`cut` 仅删直接边，对自环、不同树及不存在的边返回 `false`。`split / path_add / path_sum` 前须检查 `connected`。`set_val` 经 access / splay 下传旧标记后覆盖单点。点权、懒标记、辅助树和及乘加中间值均须在 `ll` 范围内，仅最终答案不溢出并不足够。

=== 现场记忆

`access(x) + splay(x)`：把根到 `x` 的路径变成 `x` 的 Splay。

`makeroot(x)`：`access + splay + rev`。

`split(x,y)`：`makeroot(x)` 后 `access(y), splay(y)`，此时 `y` 的 Splay 就是 `x -> y`。

所以路径修改 / 查询直接改或读 `y`。

`link`：先 `makeroot(x)`，确认不同树，再 `fa[x] = y`。

`cut`：`split(x,y)` 后，直接边满足 `ch[y][0] == x && ch[x][1] == 0`。

Splay 前必须把祖先到当前点的懒标记依次 `push_down`。

=== 模板引用

#tree_code("link_cut_tree.hpp")

