= 数据结构 <chapter-data-structure>

#text("完整保留 raw/04_data_structure.md 的 39 个原始条目及其重要变体，按原顺序排列。原稿附项按当前位置参与算法自动编号；线段树组另补充 Beats 和 zkw 两种用途的独立模板。")

#text("统一约定：输入规模、节点号、版本号和查询区间须合法；空区间仅在接口明确支持时使用。递归实现需评估栈空间；整数中间值及累计值必须在对应类型范围内。多测重新调用 init / build / clear，普通函数的外部维护状态由调用者清空。各模板独立选用，同名 Info / Tag / Seg / Query 不应同时粘贴。")

#import "../styles/source.typ": source_code


=== 公共代码与复制顺序

本章源码块仅展示算法自身实现；公共骨架只打印一次。按依赖链接复制所需的公共代码，再复制本算法与调用示例，比赛时不需要仓库头文件。已有基础算法被复用时，直接复制其原条目，不在每个代码块里重贴。

*依赖索引：* #ref(<algo-data-structure-mo-rollback-add>, supplement: none)。

== #text("单调队列：滑动窗口最小值 / 最大值") <algo-data-structure-sliding-window>

#text("原稿来源：4.1 单调队列：滑动窗口最小值 / 最大值")


#strong[#text("用途")]#text("：")#raw("O(n)")#text(" 求每个长度为 ")#raw("k")#text(" 的滑动窗口最小值 / 最大值。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n)")#text("，空间 ")#raw("O(k)")
#strong[#text("下标")]#text("：")#raw("1-indexed")
#strong[#text("限制")]#text("：")#raw("1 <= k <= n")


=== #text("现场记忆")

#raw("a[1..n]，队列存下标，队首是当前窗口答案。\n\n先弹过期位置：q.front() <= i - k\n再从队尾维护单调性。\n\n最小值：值递增，>= a[i] 弹。\n最大值：值递减，<= a[i] 弹。\n\ni >= k 时产生第一个答案。",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#raw("window_min(a,k)")#text(" / ")#raw("window_max(a,k)")#text(" 返回按窗口右端点顺序的结果，输入 a[1..n]，输出 0-indexed。k 不在 1..n 时返回空。两函数均无外部状态。")

=== 最小调用示例

```cpp
vector<int> a{0, 3, 1, 2};
assert((window_min(a, 2) == vector<int>{1, 1}));
assert((window_max(a, 2) == vector<int>{3, 2}));
```

=== 模板引用

#source_code(("data_structure/sliding_window.hpp",))


== #text("单调栈：左右最近严格更小元素") <algo-data-structure-monotonic-stack>

#text("原稿来源：4.2 单调栈：左右最近严格更小元素")


#strong[#text("用途")]#text("：")#raw("O(n)")#text(" 求每个位置左 / 右侧第一个满足大小关系的位置。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n)")#text("，空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：")#raw("1-indexed")


=== #text("现场记忆")

#raw("栈存下标。\n\n严格小于：弹 >=\n小于等于：弹 >\n严格大于：弹 <=\n大于等于：弹 <\n\n找左边：1 -> n\n找右边：n -> 1",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#raw("nearest_smaller(a)")#text(" 返回 res[1..n]，不存在为 0；比较 >= 弹栈，保证严格更小。当前源码求左侧；原稿所述右侧版本可将扫描方向反转。a[0] 为占位。")

=== 最小调用示例

```cpp
assert((nearest_smaller(vector<int>{0, 2, 2, 1, 3}) ==
        vector<int>{0, 0, 0, 0, 3}));
```

=== 模板引用

#source_code(("data_structure/monotonic_stack.hpp",))


== #text("笛卡尔树 - 单调栈建树") <algo-data-structure-cartesian-tree>

#text("原稿来源：笛卡尔树 - 单调栈建树")


#strong[#text("用途")]#text("：给定 ")#raw("a[1..n]")#text("，构造下标满足 BST、权值满足小根堆性质的笛卡尔树。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n)")#text("，空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：")#raw("1-indexed")


=== #text("现场记忆")

#raw("维护权值单调递增栈。\n\n插入 i：\n弹出所有 > a[i] 的点\n最后弹出的点 -> ls[i]\n弹完后的栈顶 -> 它的 rs = i\n\n最后 stk[0] 是根。\n大根堆反转比较符号。",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#raw("cartesian_tree(a,ls,rs)")#text(" 返回根编号，ls / rs 为输出儿子数组。输入 a[1..n]，同值不弹栈，较早位置优先成为祖先；n=0 使用 a={0}，返回 0。每次重建清空儿子数组。")

=== 最小调用示例

```cpp
vector<int> ls, rs;
vector<int> a{0, 3, 1, 2};
int rt = cartesian_tree(a, ls, rs);
assert(rt == 2 && ls[2] == 1 && rs[2] == 3);
```

=== 模板引用

#source_code(("data_structure/cartesian_tree.hpp",))


== #text("ST 表：区间最大值") <algo-data-structure-sparse-table>

#text("原稿来源：4.3 ST 表：区间最大值")


#strong[#text("用途")]#text("：静态数组预处理后，")#raw("O(1)")#text(" 查询区间最大值。")
#strong[#text("复杂度")]#text("：预处理 ")#raw("O(n log n)")#text("，单次查询 ")#raw("O(1)")#text("，空间 ")#raw("O(n log n)")
#strong[#text("下标")]#text("：")#raw("1-indexed")
#strong[#text("限制")]#text("：数组静态不修改；")#raw("1 <= l <= r <= n")





=== #text("现场记忆")

#raw("st[k][i]")#text("：从 ")#raw("i")#text(" 开始、长度 ")#raw("2^k")#text(" 的最大值。")

#text("查询长度 ")#raw("len")#text("，取 ")#raw("k = floor(log2(len))")#text("。")

#text("两个长度 ")#raw("2^k")#text(" 的块允许重叠：")
#raw("[l, l+2^k-1]")#text(" 和 ")#raw("[r-2^k+1, r]")#text("。")


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
STMax st(vector<int>{0, -3, -1, -2});
assert(st.query(1, 3) == -1);
```

=== 模板引用

#source_code(("data_structure/sparse_table.hpp",))


== #text("Fenwick 树：动态集合第 k 小") <algo-data-structure-fenwick-kth>

#text("原稿来源：4.4 Fenwick 树：动态集合第 k 小")


#strong[#text("用途")]#text("：离散值域上动态插入 / 删除，并查询当前集合第 ")#raw("k")#text(" 小。")
#strong[#text("复杂度")]#text("：修改、前缀和、第 ")#raw("k")#text(" 小均为 ")#raw("O(log n)")#text("，空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：值域 ")#raw("1-indexed")
#strong[#text("限制")]#text("：树中维护的是非负频次；调用 ")#raw("kth(k)")#text(" 时需满足 ")#raw("1 <= k <= total()")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("init(n)")],[#text("初始化值域 ")#raw("[1,n]")],
[#raw("add(x,v)")],[#raw("x")#text(" 的出现次数增加 ")#raw("v")],
[#raw("sum(x)")],[#raw("[1,x]")#text(" 中元素数量")],
[#raw("total()")],[#text("当前元素总数")],
[#raw("kth(k)")],[#text("第 ")#raw("k")#text(" 小对应的值域位置")],
)])



=== #text("现场记忆")

#raw("kth")#text(" 找最小的 ")#raw("pos")#text("，使前缀和 ")#raw("sum(pos) >= k")#text("。")

#text("二进制下降维护：")
#raw("cur = sum(pos)")#text("。")

#text("若 ")#raw("cur + tree[nxt] < k")#text("，说明答案还在右边，跳到 ")#raw("nxt")#text("。")

#text("删除后必须保证每个位置频次非负，否则前缀和不再单调，")#raw("kth")#text(" 失效。")



=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#raw("add(x,delta)")#text(" 仅允许 1<=x<=n；x=0 会使 lowbit 循环不前进。第 k 小要求 1<=k<=total 且频次非负；频次总和须在 int 范围内。多测 init(n) 清空。")

=== 最小调用示例

```cpp
BITKth fw(5);
fw.add(2, 2);
fw.add(5, 1);
assert(fw.kth(2) == 2 && fw.kth(3) == 5);
```

=== 模板引用

#source_code(("data_structure/fenwick_kth.hpp",))


== 树状数组：区间加与区间和 <algo-data-structure-fenwick-range>

*用途：* 两棵 Fenwick 维护差分，支持区间加、单点查询和区间和；只需要区间加单点查时可以只保留差分树 t。

*复杂度：* 初始化 `O(n)`，修改和查询 `O(log n)`，空间 `O(n)`。

*下标与接口：* `1..n`，`BITRange b(n)` 初始全 0；`add(l,r,v)` 为闭区间加，`sum(p)` 返回前 p 项和，允许 p=0；`query(l,r)` 返回区间和，单点用 `query(p,p)`。`init(n)` 清空两棵树。

*注意事项：* 不支持区间赋值；输入非零数组可逐点 add，构建耗时 `O(n log n)`。差分取负、树内累计、`v*(p-1)`、前缀和乘法以及相减的中间值都必须在 ll 内；不能传 0 作为修改左端点。

*现场记忆：* 差分 d 的前缀为 a；前缀和是 `p*Σd[i] - Σ(i-1)*d[i]`。在 l 加 v、r+1 减 v；r=n 时末端自然不入树。

=== 最小调用示例

```cpp
BITRange b(5);
b.add(2, 5, 3);
b.add(3, 3, -1);
assert(b.query(1, 5) == 11);
assert(b.query(3, 3) == 2);
b.init(2);
assert(b.sum(2) == 0);
```

=== 模板引用

#source_code(("data_structure/fenwick_range.hpp",))

== #text("线性基") <algo-data-structure-linear-basis>

#text("原稿来源：4.5 线性基")


#strong[#text("用途")]#text("：维护一组数的异或线性空间，支持插入、可表示性、最大异或、第 ")#raw("k")#text(" 小异或值等操作。")
#strong[#text("复杂度")]#text("：插入 / 查询 ")#raw("O(B)")#text("；")#raw("rebuild()")#text(" 为 ")#raw("O(B^2)")#text("；空间 ")#raw("O(B)")#text("，其中 ")#raw("B = 63")
#strong[#text("下标")]#text("：二进制位 ")#raw("0 ~ 62")
#strong[#text("限制")]#text("：默认处理 ")#raw("[0, 2^63)")#text(" 内的非负 ")#raw("long long")
#strong[#text("注意")]#text("：")#raw("kth()")#text(" / ")#raw("rank()")#text(" 使用前必须 ")#raw("rebuild()")#text("；之后若再次 ")#raw("insert()")#text(" / ")#raw("merge()")#text("，需要重新 ")#raw("rebuild()")#text("。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口 / 变量")],[#text("含义")]),
[#raw("init()")],[#text("清空线性基")],
[#raw("insert(x)")],[#text("插入；独立返回 ")#raw("true")#text("，相关返回 ")#raw("false")],
[#raw("contains(x)")],[#text("是否能异或表示出 ")#raw("x")],
[#raw("max_xor()")],[#text("可表示出的最大异或值")],
[#raw("min_nonzero()")],[#text("最小非零异或值；空基返回 ")#raw("0")],
[#raw("merge(other)")],[#text("合并另一个线性基")],
[#raw("rebuild()")],[#text("转为 ")#raw("kth / rank")#text(" 所需的规范基")],
[#raw("distinct()")],[#text("不同异或值数量")],
[#raw("kth(k)")],[#text("第 ")#raw("k")#text(" 小不同异或值，")#raw("k")#text(" 从 ")#raw("0")#text(" 开始")],
[#raw("rank(x)")],[#raw("x")#text(" 在不同异或值中的 ")#raw("0-indexed")#text(" 排名")],
[#raw("cnt")],[#text("线性基维数")],
)])



=== #text("现场记忆")

#text("插入从最高位向下消元：")
#text("该最高位没有基就放进去，否则 ")#raw("x ^= basis[i]")#text("。")

#raw("insert(x) == false")#text(" ⇔ ")#raw("x")#text(" 与现有基线性相关。")

#text("最大异或：从高位到低位，只要 ")#raw("res ^ basis[i]")#text(" 更大就异或。")

#raw("rebuild()")#text(" 消掉每个基向量中的低位主元，再按主元从低到高放入 ")#raw("vec")#text("。")

#text("规范化后，")#raw("k")#text(" 的二进制位直接决定选哪些 ")#raw("vec[i]")#text("，因此可得到排序后的第 ")#raw("k")#text(" 小不同异或值。")

#text("任何后续 ")#raw("insert / merge")#text(" 都会破坏规范基，")#raw("kth / rank")#text(" 前重新 ")#raw("rebuild()")#text("。")



=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("无数组下标；支持 [0,2^63) 的 long long。kth 与 rank 均从 0 开始，kth(0)=0；调用前 rebuild，insert / merge 后重新 rebuild。distinct 使用 unsigned long long，最多 2^63。")

=== 最小调用示例

```cpp
Basis b;
b.insert(3);
b.insert(5);
b.rebuild();
assert(b.max_xor() == 6 && b.kth(0) == 0 && b.kth(3) == 6);
```

=== 模板引用

#source_code(("data_structure/linear_basis.hpp",))


== #text("普通莫队") <algo-data-structure-mo>

#text("原稿来源：4.6 普通莫队")


#strong[#text("用途")]#text("：离线处理静态区间询问，适合加入 / 删除一个位置能快速维护答案的问题。")
#strong[#text("复杂度")]#text("：块长为 ")#raw("B")#text(" 时约 ")#raw("O(mB + n^2/B)")#text(" 次指针移动；")#raw("m≈n")#text(" 时为 ")#raw("O((n+m)√n)")
#strong[#text("下标")]#text("：数组、区间均 ")#raw("1-indexed")


=== #text("现场记忆")

#text("莫队只负责移动 ")#raw("[L,R]")#text("，题意全部塞进 ")#raw("add / del")#text("。")

#text("左端点分块，同块 ")#raw("r")#text(" 奇偶反向排序减少折返。")

#text("块长可取 ")#raw("B≈n/√m")#text("；")#raw("m≈n")#text(" 时就是 ")#raw("√n")#text("。")



=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

#text("莫队引擎只负责指针、时间或 Euler 序的移动。now 按引用传入，回调实际维护题目状态；每次调用前清空。Query.id 为 0..m-1 的排列。带修模板的 Change 从下标 1 开始，t 为已生效修改数，pre 由按时间顺序的临时数组计算；回调接收当前值，调用者需要时自行离散化。回滚的 undo 必须严格撤销最近操作，clear / init 同时清空回滚栈和 now。二次离线须传入真实 pre / suf，且 w(i,j) 在 i>=j 时视为 0。骨架的应用维护逻辑仍须针对题目验证。")

=== 最小调用示例

```cpp
ll now = 0;
vector<Query> q{{1, 2, 0}, {2, 3, 1}};
vector<int> a{0, 1, 2, 1};
auto add = [&](int p) { now += a[p]; };
auto del = [&](int p) { now -= a[p]; };
auto ans = mo(3, q, now, add, del);
assert((ans == vector<ll>{3, 3}));
```

=== 模板引用

#source_code(("data_structure/mo.hpp",))


== #text("带修莫队") <algo-data-structure-mo-updates>

#text("原稿来源：4.7 带修莫队")


#strong[#text("用途")]#text("：离线处理区间询问 + 单点修改。")
#strong[#text("复杂度")]#text("：询问、修改与 ")#raw("n")#text(" 同阶时，取 ")#raw("B≈n^(2/3)")#text("，约 ")#raw("O(n^(5/3))")#text(" 次状态转移")
#strong[#text("下标")]#text("：数组、区间、修改均 ")#raw("1-indexed")
#strong[#text("注意")]#text("：修改点在当前区间内时，必须 ")#raw("del -> 改值 -> add")#text("。")


=== #text("现场记忆")

#text("状态从 ")#raw("(L,R)")#text(" 变成 ")#raw("(L,R,T)")#text("。")

#text("第 ")#raw("t")#text(" 次修改记录 ")#raw("pre -> now")#text("；同一位置连续修改时，")#raw("pre")#text(" 必须用读入时的临时数组维护。")

#text("修改点在 ")#raw("[L,R]")#text("：")
#raw("del(pos) -> cur[pos]=新值 -> add(pos)")#text("。")

#text("撤销完全相反，只把新值换回 ")#raw("pre")#text("。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

#text("莫队引擎只负责指针、时间或 Euler 序的移动。now 按引用传入，回调实际维护题目状态；每次调用前清空。Query.id 为 0..m-1 的排列。带修模板的 Change 从下标 1 开始，t 为已生效修改数，pre 由按时间顺序的临时数组计算；回调接收当前值，调用者需要时自行离散化。回滚的 undo 必须严格撤销最近操作，clear / init 同时清空回滚栈和 now。二次离线须传入真实 pre / suf，且 w(i,j) 在 i>=j 时视为 0。骨架的应用维护逻辑仍须针对题目验证。")

=== 最小调用示例

```cpp
vector<int> a{0, 1, 2};
vector<Change> c{{}, {1, 1, 5}};
vector<Query> q{{1, 2, 0, 0}, {1, 2, 1, 1}};
ll now = 0;
auto ans = mo_updates(
    a, c, q, now, [&](int v) { now += v; }, [&](int v) { now -= v; });
assert((ans == vector<ll>{3, 7}));
```

=== 模板引用

#source_code(("data_structure/mo_updates.hpp",))


== #text("只增回滚莫队") <algo-data-structure-mo-rollback-add>

#text("原稿来源：4.8 只增回滚莫队")


#strong[#text("用途")]#text("：删除困难，但加入容易、临时加入可以回滚时处理静态区间询问。")
#strong[#text("复杂度")]#text("：约 ")#raw("O(mB + n^2/B)")#text(" 次状态操作；")#raw("m≈n")#text(" 时取 ")#raw("B≈√n")
#strong[#text("下标")]#text("：数组、区间均 ")#raw("1-indexed")
#strong[#text("注意")]#text("：右扩是永久状态，左扩是临时状态；两者可以分开维护以减少回滚常数。")


=== #text("常用维护：区间相同值最大下标差")

#raw("a[i]")#text(" 先离散化到 ")#raw("[0,V)")#text("。")


#text("复杂维护优先记快照：")
#text("临时左扩前保存 now 与回滚栈大小；add_left 把修改前状态入栈，rollback_left 严格逆序恢复，最后还原 now。此引擎已按加入次数调用撤销回调，不要再在回调中一次撤销整段。")

=== #text("现场记忆")

#text("每个左端点块重新开始；同块询问按 ")#raw("r")#text(" 升序，右指针只增。")

#text("右边 ")#raw("[block_r+1,R]")#text(" 是永久状态，不必记录左扩所需的回滚信息。")

#text("左边 ")#raw("[l,block_r]")#text(" 临时加入，查询后回滚。")

#text("临时加入顺序 ")#raw("block_r -> l")#text("，撤销顺序必须 ")#raw("l -> block_r")#text("。")

#text("复杂状态直接 ")#raw("snapshot = stk.size()")#text("，回滚到 snapshot 最稳。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

#text("莫队引擎只负责指针、时间或 Euler 序的移动。now 按引用传入，回调实际维护题目状态；每次调用前清空。Query.id 为 0..m-1 的排列。带修模板的 Change 从下标 1 开始，t 为已生效修改数，pre 由按时间顺序的临时数组计算；回调接收当前值，调用者需要时自行离散化。回滚的 undo 必须严格撤销最近操作，clear / init 同时清空回滚栈和 now。二次离线须传入真实 pre / suf，且 w(i,j) 在 i>=j 时视为 0。骨架的应用维护逻辑仍须针对题目验证。")

=== 最小调用示例

```cpp
ll now = 0;
auto add = [&](int p) { now += p; };
auto undo = [&](int p) { now -= p; };
auto ans = mo_rollback_add(3, vector<Query>{{1, 3, 0}}, now, add, add,
                           undo, [&]() { now = 0; });
assert(ans[0] == 6);
```

=== 模板引用

#source_code(("data_structure/mo_rollback_add.hpp",))

==== #text("mo_max_span")

```cpp
assert(mo_max_span(vector<int>{0, 1, 2, 1}, vector<Query>{{1, 3, 0}},
                   3)[0] == 2);
```

*代码依赖：* 先复制 #ref(<algo-data-structure-mo-rollback-add>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("data_structure/mo_max_span.hpp",))

== #text("只删回滚莫队") <algo-data-structure-mo-rollback-delete>

#text("原稿来源：4.9 只删回滚莫队")


#strong[#text("用途")]#text("：加入困难，但删除容易、最近的删除可以回滚时处理静态区间询问。")
#strong[#text("复杂度")]#text("：约 ")#raw("O(mB + n^2/B)")#text(" 次状态操作；")#raw("m≈n")#text(" 时取 ")#raw("B≈√n")
#strong[#text("下标")]#text("：数组、区间均 ")#raw("1-indexed")
#strong[#text("注意")]#text("：保留块间状态转移；")#raw("L")#text(" 全程只向右推进，不为每个块重新建整个状态。")


=== #text("现场记忆")

#text("初始维护完整 ")#raw("[1,n]")#text("。")

#text("每块基准状态是 ")#raw("[block_l,n]")#text("；")#raw("L")#text(" 跨块只向右，因此块间不用重新建状态。")

#text("同块 ")#raw("r")#text(" 降序，右侧删除顺序：")
#raw("n, n-1, ...")#text("。")

#text("换块前按相反顺序 ")#raw("R+1, R+2, ..., n")#text(" 回滚右侧，再恢复 ")#raw("block_now")#text("。")

#text("左侧临时删除后必须严格逆序回滚；回滚函数若不维护答案，直接用 ")#raw("backup")#text(" 恢复 ")#raw("now")#text("。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

#text("莫队引擎只负责指针、时间或 Euler 序的移动。now 按引用传入，回调实际维护题目状态；每次调用前清空。Query.id 为 0..m-1 的排列。带修模板的 Change 从下标 1 开始，t 为已生效修改数，pre 由按时间顺序的临时数组计算；回调接收当前值，调用者需要时自行离散化。回滚的 undo 必须严格撤销最近操作，clear / init 同时清空回滚栈和 now。二次离线须传入真实 pre / suf，且 w(i,j) 在 i>=j 时视为 0。骨架的应用维护逻辑仍须针对题目验证。")

=== 最小调用示例

```cpp
ll now = 0;
auto ans = mo_rollback_delete(
    3, vector<Query>{{1, 3, 0}}, now, [&]() { now = 6; },
    [&](int p) { now -= p; }, [&](int p) { now += p; });
assert(ans[0] == 6);
```

=== 模板引用

#source_code(("data_structure/mo_rollback_delete.hpp",))


== #text("树上莫队") <algo-data-structure-mo-tree>

#text("原稿来源：4.10 树上莫队")


#strong[#text("用途")]#text("：通过双 Euler 序把树上路径询问转成莫队区间询问。")
#strong[#text("复杂度")]#text("：Euler / LCA 预处理 ")#raw("O(n log n)")#text("；莫队约 ")#raw("O((n+m)√n)")#text(" 次节点切换")
#strong[#text("下标")]#text("：树节点 ")#raw("1-indexed")
#strong[#text("注意")]#text("：非祖先路径使用 ")#raw("[out[u],in[v]]")#text("，LCA 需要额外加入一次。")


=== #text("现场记忆")

#text("Euler 序每个节点出现两次；当前区间中出现奇数次的节点就是路径节点。")

#text("先保证 ")#raw("in[u] <= in[v]")#text("。")

#text("若 ")#raw("lca(u,v)==u")#text("：")
#raw("[in[u],in[v]]")#text("。")

#text("否则：")
#raw("[out[u],in[v]] + lca(u,v)")#text("。")

#text("额外 LCA 不在莫队区间中，记录答案时临时 ")#raw("add_node")#text(" 再 ")#raw("del_node")#text("。")

#text("树可能退化成链且 ")#raw("n")#text(" 很大时，递归 DFS 注意爆栈。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

#text("莫队引擎只负责指针、时间或 Euler 序的移动。now 按引用传入，回调实际维护题目状态；每次调用前清空。Query.id 为 0..m-1 的排列。带修模板的 Change 从下标 1 开始，t 为已生效修改数，pre 由按时间顺序的临时数组计算；回调接收当前值，调用者需要时自行离散化。回滚的 undo 必须严格撤销最近操作，clear / init 同时清空回滚栈和 now。二次离线须传入真实 pre / suf，且 w(i,j) 在 i>=j 时视为 0。骨架的应用维护逻辑仍须针对题目验证。")

=== 最小调用示例

```cpp
vector<vector<int>> g{{}, {2, 3}, {1}, {1}};
ll now = 0;
auto ans = mo_tree(
    g, vector<pair<int, int>>{{2, 3}}, now, [&](int u) { now += u; },
    [&](int u) { now -= u; });
assert(ans[0] == 6);
```

=== 模板引用

#source_code(("data_structure/mo_tree.hpp",))


== #text("二次离线莫队：二元组贡献") <algo-data-structure-mo-second-offline>

#text("原稿来源：4.11 二次离线莫队：二元组贡献")


#strong[#text("用途")]#text("：处理区间答案可写成 ")#raw("ans(l,r)=Σw(i,j)")#text("（")#raw("l<=i<j<=r")#text("）的二元组贡献问题。")
#strong[#text("复杂度")]#text("：事件总扫描长度与莫队指针移动量同阶，约 ")#raw("O(mB+n^2/B)")#text(" 次题意查询")
#strong[#text("下标")]#text("：数组、区间均 ")#raw("1-indexed")
#strong[#text("限制")]#text("：需要能快速维护 ")#raw("Σ_{j<=x}w(j,i)")#text(" 与 ")#raw("Σ_{j>=x}w(i,j)")#text("。")
#strong[#text("注意")]#text("：")#raw("query_pre / query_suf")#text(" 应尽量做到 ")#raw("O(1)")#text(" 或很小的复杂度，否则二次离线仍可能超时。")


=== #text("现场记忆")

#text("设：")

#raw("pre[i] = ans(1,i)")#text("，")#raw("suf[i] = ans(i,n)")#text("。")

#text("右扩：")
#raw("pre[r]-pre[R]")#text("，再减去左边界外的贡献。")

#text("右缩：")
#raw("-(pre[R]-pre[r])")#text("，再补回左边界外的贡献。")

#text("左扩 / 左缩同理使用 ")#raw("suf")#text("，修正右边界外的贡献。")

#text("修正项不当场算，只挂事件；第二次扫描统一处理：")

#raw("query_pre(i) = Σ_{j<=x} w(j,i)")

#raw("query_suf(i) = Σ_{j>=x} w(i,j)")

#text("事件区间总长度与莫队移动量同阶。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

#text("莫队引擎只负责指针、时间或 Euler 序的移动。now 按引用传入，回调实际维护题目状态；每次调用前清空。Query.id 为 0..m-1 的排列。带修模板的 Change 从下标 1 开始，t 为已生效修改数，pre 由按时间顺序的临时数组计算；回调接收当前值，调用者需要时自行离散化。回滚的 undo 必须严格撤销最近操作，clear / init 同时清空回滚栈和 now。二次离线须传入真实 pre / suf，且 w(i,j) 在 i>=j 时视为 0。骨架的应用维护逻辑仍须针对题目验证。")

=== 最小调用示例

```cpp
int n = 3;
vector<ll> pre{0, 0, 1, 3}, suf{0, 3, 1, 0, 0};
int p = 0, s = 4;
auto ans = mo_second_offline(
    n, vector<Query>{{1, 3, 0}}, pre, suf, [&]() { p = 0; },
    [&](int x) { p = x; }, [&](int i) -> ll { return min(p, i - 1); },
    [&]() { s = n + 1; }, [&](int x) { s = x; },
    [&](int i) -> ll { return n - max(s, i + 1) + 1; });
assert(ans[0] == 3);
```

=== 模板引用

#source_code(("data_structure/mo_second_offline.hpp",))


== #text("值域分块：频次 / 第 k 小 / 前驱后继") <algo-data-structure-value-block>

#text("原稿来源：4.12 值域分块：频次 / 第 k 小 / 前驱后继")


#strong[#text("用途")]#text("：在离散值域上维护动态多重集合，支持频次修改、排名、第 ")#raw("k")#text(" 小、前驱和后继。")
#strong[#text("复杂度")]#text("：修改 ")#raw("O(1)")#text("；查询 ")#raw("O(√V)")#text("；空间 ")#raw("O(V)")
#strong[#text("下标")]#text("：值域 ")#raw("1 ~ n")
#strong[#text("限制")]#text("：需要保证每个值的频次始终非负；值域过大时先离散化。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口 / 变量")],[#text("含义")]),
[#raw("init(n)")],[#text("初始化值域 ")#raw("[1,n]")],
[#raw("add(x,v)")],[#raw("x")#text(" 的出现次数增加 ")#raw("v")],
[#raw("count_le(x)")],[#raw("<= x")#text(" 的元素数量")],
[#raw("count_lt(x)")],[#raw("< x")#text(" 的元素数量")],
[#raw("kth(k)")],[#text("第 ")#raw("k")#text(" 小，")#raw("k")#text(" 从 ")#raw("1")#text(" 开始")],
[#raw("prev(x)")],[#text("严格小于 ")#raw("x")#text(" 的最大值")],
[#raw("next(x)")],[#text("严格大于 ")#raw("x")#text(" 的最小值")],
[#raw("tot")],[#text("当前元素总数")],
)])



=== #text("现场记忆")

#text("按“值”分块，不是按数组位置分块。")

#raw("freq[x]")#text(" 维护单值频次，")#raw("sum[b]")#text(" 维护整块元素数量。")

#text("第 ")#raw("k")#text(" 小：")
#text("先整块减 ")#raw("sum[b]")#text("，定位块后再逐值减 ")#raw("freq[x]")#text("。")

#text("排名：")
#raw("rank(x) = count_lt(x) + 1")#text("。")

#text("前驱 / 后继：")
#text("先扫当前残块，再跳过 ")#raw("sum[b] == 0")#text(" 的整块。")

#text("值域太大先离散化；此时 ")#raw("kth()")#text(" 返回离散后的下标。")


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("值域 1..n；add 需要合法值和非负频次，频次总和在 int 内。kth 不合法和不存在的前驱后继返回 -1。count_lt 对 INT_MIN 不做 x-1，空值域 prev 安全返回 -1。")

=== 最小调用示例

```cpp
ValueBlock b(5);
b.add(2, 2);
b.add(5, 1);
assert(b.kth(3) == 5 && b.prev(5) == 2 && b.next(2) == 5);
```

=== 模板引用

#source_code(("data_structure/value_block.hpp",))


== #text("序列分块：区间加 / 区间和") <algo-data-structure-sequence-block>

#text("原稿来源：4.13 序列分块：区间加 / 区间和")


#strong[#text("用途")]#text("：按数组位置平方分块，维护区间加、区间和，并支持单点赋值。")
#strong[#text("复杂度")]#text("：建块 ")#raw("O(n)")#text("；修改 / 查询 ")#raw("O(√n)")#text("；空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：数组、区间均 ")#raw("1-indexed")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("build(a)")],[#text("按 ")#raw("a[1..n]")#text(" 重新建块")],
[#raw("change(pos,v)")],[#text("单点赋值 ")#raw("a[pos] = v")],
[#raw("modify(l,r,v)")],[#text("区间 ")#raw("[l,r] += v")],
[#raw("query(l,r)")],[#text("查询 ")#raw("[l,r]")#text(" 区间和")],
)])



=== #text("现场记忆")

#text("按“位置”分块，")#raw("bel[i]")#text(" 表示下标 ")#raw("i")#text(" 所属块。")

#raw("a[i]")#text(" 不含尚未下放的 ")#raw("tag[b]")#text("；")
#raw("sum[b]")#text(" 始终保存整块真实总和。")

#text("整块加：")

#raw("tag[b] += v")

#raw("sum[b] += 块长 * v")

#text("需要直接修改残块时：")

#raw("pushdown -> 修改 a[i] -> rebuild")

#text("查询残块用 ")#raw("a[i] + tag[b]")#text("；")
#text("查询整块直接用 ")#raw("sum[b]")#text("。")

#text("以后维护块内排序、最大值、最小值等，主要修改 ")#raw("rebuild()")#text(" 和整块查询逻辑。")


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("输入 a[1..n]，空构建传 {0}；非空闭区间才可查询。sum 与 tag 的累计量、长度乘增量均需在 long long 内。build 重新初始化所有块。")

=== 最小调用示例

```cpp
SeqBlock b(vector<ll>{0, 1, 2, 3});
b.modify(1, 3, 2);
b.change(2, 0);
assert(b.query(1, 3) == 8);
```

=== 模板引用

#source_code(("data_structure/sequence_block.hpp",))


== #text("InfoTag 线段树") <algo-data-structure-segment-tree-lazy>

#text("原稿来源：4.14 InfoTag 线段树")


#strong[#text("用途")]#text("：通用懒标记线段树框架；默认实现区间加、区间和，换题时主要修改 ")#raw("Tag / Info / operator+")#text("。")
#strong[#text("复杂度")]#text("：建树 ")#raw("O(n)")#text("；区间修改 / 查询 ")#raw("O(log n)")#text("；空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：数组、区间均 ")#raw("1-indexed")
#strong[#text("注意")]#text("：")#raw("Tag::apply(t)")#text(" 表示在已有旧标记之后，再执行新标记 ")#raw("t")#text("；非交换操作时合并顺序不能写反。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("build(a)")],[#text("按 ")#raw("a[1..n]")#text(" 重新建树")],
[#raw("modify(l,r,t)")],[#text("对 ")#raw("[l,r]")#text(" 应用标记 ")#raw("t")],
[#raw("query(l,r)")],[#text("返回 ")#raw("[l,r]")#text(" 的 ")#raw("Info")],
[#raw("Tag::apply(t)")],[#text("合并懒标记")],
[#raw("Info::apply(t)")],[#text("标记作用到区间信息")],
[#raw("operator+")],[#text("合并左右儿子信息")],
)])



=== #text("现场记忆")

#raw("Info")#text(" 保存当前区间的真实信息，")#raw("Tag")#text(" 保存尚未下传给儿子的操作。")

#text("整段修改一定同时做：")

#raw("info[p].apply(t)")
#raw("tag[p].apply(t)")

#text("即答案先更新，标记只负责以后下传。")

#raw("Tag::apply(t)")#text(" 的语义：")
#strong[#text("旧操作之后执行新操作 `t`")]#text("。")
#text("若把标记看成函数，应合成为 ")#raw("t ∘ old")#text("。")

#text("下探儿子前必须 ")#raw("pushdown()")#text("，修改儿子后必须 ")#raw("pushup()")#text("。")

#text("换题通常只改三处：")

#raw("Tag")#text("：标记内容 + 标记合并")
#raw("Info")#text("：区间信息 + 标记作用")
#raw("operator+")#text("：左右区间合并")



=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
Seg seg(vector<ll>{0, 1, 2, 3});
assert(seg.query(1, 3).sum == 6);
```

=== 模板引用

#source_code(("data_structure/segment_tree_lazy.hpp",))


== #text("Info 线段树：单点修改") <algo-data-structure-segment-tree-point>

#text("原稿来源：4.15 Info 线段树：单点修改")


#strong[#text("用途")]#text("：通用单点修改线段树框架；默认实现单点赋值、区间和，换题时主要修改 ")#raw("Info / operator+")#text("。")
#strong[#text("复杂度")]#text("：建树 ")#raw("O(n)")#text("；单点修改 / 区间查询 ")#raw("O(log n)")#text("；空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：数组、区间均 ")#raw("1-indexed")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("build(a)")],[#text("按 ")#raw("a[1..n]")#text(" 重新建树")],
[#raw("modify(x,v)")],[#text("将位置 ")#raw("x")#text(" 的信息改为 ")#raw("v")],
[#raw("query(l,r)")],[#text("返回 ")#raw("[l,r]")#text(" 的 ")#raw("Info")],
[#raw("operator+")],[#text("合并左右区间信息")],
)])



=== #text("现场记忆")

#text("单点修改一路递归到叶子，直接 ")#raw("info[p] = v")#text("。")

#text("回溯时不断 ")#raw("pushup()")#text(" 重算祖先。")

#text("无懒标记，因此没有 ")#raw("Tag / apply / pushdown")#text("。")

#text("换题通常只改两处：")

#raw("Info")#text("：节点维护的信息")
#raw("operator+")#text("：左右区间如何合并")



=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
Seg seg(vector<ll>{0, 1, 2, 3});
assert(seg.query(1, 3).sum == 6);
```

=== 模板引用

#source_code(("data_structure/segment_tree_point.hpp",))


== Segment Tree Beats：区间最值与加法 <algo-data-structure-segment-tree-beats>

*用途：* 区间 `chmin`、`chmax`、加法，查询区间和、最小值和最大值。用 `Info` 保存信息、`Seg` 管树与下传；变量采用 `mn / mx / mn2 / mx2 / nmn / nmx`。

=== 接口、复杂度与下标

- `Seg seg(a)` 或 `build(a)`：从 1-index 的 `vector<ll>` 建树，多测会清空全部状态；空数组可建，只允许空区间操作。
- `add(l,r,v)`：加 `v`；`chmin(l,r,v)`：逐个取 `min(a[i],v)`；`chmax(l,r,v)`：逐个取 `max(a[i],v)`。
- `query(l,r)`：返回 `Info`，取 `.sum / .mn / .mx`；区间均为闭区间。`l>r` 时更新无效，查询返回单位元，`len=0`，不要把空区间极值当作有效答案。
- 建树 `O(n)`，空间 `O(n)`，递归深度 `O(log n)`，查询 `O(log n)`；本组最值与加法操作使用总计 `O((n+q) log^2 n)` 的均摊界，单次修改最坏可达 `O(n)`。

=== 分类与赛场修改位置

- `Info`：`sum`、长度 `len`，严格第一 / 第二小值 `mn,mn2`，严格第一 / 第二大值 `mx,mx2`，最小 / 最大值出现次数 `nmn,nmx`。`operator+` 合并左右信息，`Info()` 是空区间单位元。
- `Info::apply(v,op)`：`op=0/1/2` 对应加、取小、取大。能整段处理返回 true；不能时返回 false，且不能提前改动任何字段。
- `Seg::modify`：先检查操作无效果的退出条件，再尝试整段处理；失败就 `push`、递归、`pull`。`Seg::apply` 同步维护加标记，`push` 先下传加法，再用父节点最大 / 最小值限制儿子。
- 扩展维护量时，修改节点字段、叶子构造、合并和三种操作的更新公式；若增加新的操作，必须同时证明“何时不变 / 何时可整段更新 / 如何下传”，重新分析均摊复杂度，不能照搬普通 InfoTag 任意替换标记。
- 只需要 `chmin` 时可裁掉最小值与 `chmax` 分支，但应重新对拍；本版不含历史最值、区间取模等额外变体。

=== 注意事项

`chmin(v)` 只有 `mx2 < v < mx` 才能只修改最大值组；`v==mx2` 必须递归，否则合并后的极值计数会错。`chmax` 对称。改最大值时还需同步可能相等的 `mn/mn2`，改最小值时同步 `mx/mx2`。

`INF=2^60` 只作不存在的次值哨兵。初值、修改参数以及所有实际或延迟状态中的有限极值（包括 push 时先加后截断的中间状态）必须严格处于 `(-INF,INF)`；哨兵不能参与加法。所有节点和、查询和、累计加标记须在 `ll` 内。更新和用 GCC `__int128` 避免乘加的中间溢出，最终结果仍需装进 `ll`。查询可能下传状态，不是 const。

=== 最小调用示例

```cpp
Seg seg(vector<ll>{0, 1, 7, 3, 9});
seg.chmin(1, 4, 5);
seg.chmax(1, 3, 2);
seg.add(2, 4, 1);
Info ans = seg.query(1, 4);
assert(ans.sum == 18 && ans.mn == 2 && ans.mx == 6);
```

=== 现场记忆

先判无效，再判能否整段，失败才递归。严格次值不能写成非严格；上下极值互相同步；下传顺序是“加 → 取小 → 取大”。复杂度是均摊，不是每次严格对数。

原理参考：#link("https://codeforces.com/blog/entry/57319")[jiry_2 的 Segment Tree Beats 原文]。

=== 模板引用

#source_code(("data_structure/segment_tree_beats.hpp",))

== zkw 线段树：单点修改 <algo-data-structure-zkw-segment-tree-point>

*用途：* 迭代线段树，默认单点赋值、区间和。只用 `Seg`、数值数组 `tr` 和短函数 `merge`，无 Info/Tag 层。

=== 接口、复杂度与下标

- `Seg seg(a)` / `build(a)`：输入 1-index 数组，向上补齐二次幂 `m`，第 `i` 项在 `m+i-1`，填充叶子为单位元 `E`；重建清空旧数据。
- `modify(p,v)`：单点赋值；加法可先在外部维护当前值，再赋新值，或把叶子赋值改成 `+=`。
- `query(l,r)`：闭区间查询；`l>r` 返回 `E`。非空区间要求 `1<=l<=r<=n`。
- 建树 `O(n)`，修改 / 查询 `O(log n)`，空间 `O(n)`；无递归、无懒标记。空数组仅支持空查询。

=== 分类与赛场修改位置

- 求和：`merge(a,b)=a+b`、`E=0`；最大值：`merge=max`、`E=LLONG_MIN`；最小值：`merge=min`、`E=LLONG_MAX`；GCD：`merge=gcd`、`E=0`（注意符号与范围）。
- 合并要满足结合律，`E` 是双侧单位元；修改 `merge` 时必须同步改 `E`。查询分别累积左右答案，因此不假设交换律。
- 多字段复杂信息优先使用已有 @algo-data-structure-segment-tree-point；此版保持数值数组，便于竞赛现场裁改。所有合并中间值和答案须能放进 `ll`。

=== 最小调用示例

```cpp
Seg seg(vector<ll>{0, 1, 2, 3, 4, 5});
seg.modify(3, 10);
assert(seg.query(2, 4) == 16);
assert(seg.query(4, 3) == 0);
```

=== 现场记忆

叶子赋值后一路向上合并。外部闭区间，内部转 `[l,r)`；左答案往右接，右答案往左接。填充位置必须使用单位元。

=== 模板引用

#source_code(("data_structure/zkw_segment_tree_point.hpp",))

== zkw 线段树：区间加与区间和 <algo-data-structure-zkw-segment-tree-lazy>

*用途：* 专用高性能迭代版。直接维护 `sum / tag` 两个数值数组，永久加标记，不递归、不下传，不使用 Info/Tag 抽象。

=== 接口、复杂度与下标

- `Seg seg(a)` / `build(a)`：1-index 数组；`m` 为不小于 `n+2` 的最小二次幂，实际叶子为 `m+i`，两边保留哨兵。多测重建清空和与标记。
- `add(l,r,v)`：闭区间加；`query(l,r)`：闭区间和，const 查询，不改变树。空区间修改无效、查询为零；非空区间必须合法。
- 建树 `O(n)`，每次修改 / 查询 `O(log n)`，空间 `O(n)`，无递归栈。

=== 分类与赛场修改位置

- `sum[p]` 包含节点自身和所有后代的修改，不包含祖先尚未计入的永久标记；`tag[p]` 是直接加在整个节点上的量。
- 外部 `[l,r]` 转成叶子两旁的开区间 `(l+m-1,r+m+1)`，上行时选完整兄弟块。`k` 是当前层块长，`nl/nr` 是左右边界子树中已经覆盖的真实长度。
- 修改时完整兄弟同时改 `sum/tag`，边界及公共祖先按覆盖长度补和；查询时拿完整块的和，并把边界与公共祖先的标记乘覆盖长度补齐到根。
- 需要单点加、单点查时令 `l=r` 即可。若只改数值类型，同时修改数组、参数、返回值与中间乘积。
- 此实现依赖加法可叠加和区间和的线性关系。不能只把加号换成 max/min，也不能直接加入覆盖、乘法或 Beats 操作；这些场景选择 @algo-data-structure-segment-tree-lazy 或 @algo-data-structure-segment-tree-beats。

=== 注意事项

永久标记不能再额外 push；也不能用普通 `sum[p]=sum[lc]+sum[rc]` 回拉，否则丢掉节点自身标记。查询爬到相邻边界后仍须补公共祖先标记，不能提前返回。数组必须给两侧哨兵留位置；这里与单点 zkw 的叶子偏移不同。

累计标记、`tag*覆盖长度`、节点和及查询累加的每一步均须在 `ll` 范围内。两版的树下标用 int，规模须保证补齐二次幂及 `2*m` 不溢出且内存足够。是否比递归版更快取决于数据与编译器，不给出未经基准测试的性能倍数。

=== 最小调用示例

```cpp
Seg seg(vector<ll>{0, 1, 2, 3, 4, 5});
seg.add(1, 5, 3);
seg.add(2, 4, -2);
assert(seg.query(1, 5) == 24);
assert(seg.query(2, 4) == 12);
```

=== 现场记忆

两旁开区间夹答案，左右覆盖长度逐层累加。完整块更新和与标记；查询补齐祖先贡献。永久标记不下传，不做普通 pull。

=== 模板引用

#source_code(("data_structure/zkw_segment_tree_lazy.hpp",))

== #text("李超线段树：整数最大值") <algo-data-structure-li-chao-integer>

#text("原稿来源：4.16 李超线段树：整数最大值")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：动态加入直线 ")#raw("y = kx + b")#text("，查询整数横坐标处的最大值；支持只在指定横坐标区间生效的线段。")
#strong[#text("复杂度")]#text("：加整条直线 ")#raw("O(log X)")#text("，查询 ")#raw("O(log X)")#text("，加线段最坏 ")#raw("O(log^2 X)")#text("，")#raw("X = R-L+1")
#strong[#text("横坐标")]#text("：整数闭区间 ")#raw("[L, R]")
#strong[#text("限制")]#text("：查询点必须在 ")#raw("[L, R]")#text("；最终答案需落在 ")#raw("long long")#text(" 范围内。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("init(L, R)")],[#text("清空并设置整数横坐标范围")],
[#raw("add_line(k, b)")],[#text("全区间加入 ")#raw("y = kx + b")],
[#raw("add_seg(k, b, l, r)")],[#text("仅在 ")#raw("[l, r]")#text(" 加入直线")],
[#raw("query(x)")],[#text("查询 ")#raw("x")#text(" 处最大值")],
)])



=== #text("现场记忆")

#raw("每个节点保留“在 mid 更优”的直线。\n\n插入新线：\n先记录它在 l、mid 是否更优；\n若 mid 更优，交换新线和节点线。\n\nl 与 mid 的优劣关系不同 -> 新线去左边；\n否则 -> 去右边。\n\n区间加线 = 线段树区间分解 + 李超插线，所以最坏 O(log^2 X)。",block:true)




=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
LiChao t(-5, 5);
t.add_line(2, 1);
t.add_seg(-1, 5, -2, 3);
assert(t.query(2) == 5);
```

=== 模板引用

#source_code(("data_structure/li_chao_integer.hpp",))


== #text("李超线段树：浮点系数最大值（整数横坐标）") <algo-data-structure-li-chao-double>

#text("原稿来源：4.17 李超线段树：浮点系数最大值（整数横坐标）")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：动态加入浮点系数直线 ")#raw("y = kx + b")#text("，查询整数横坐标处的最大值；支持区间生效的线段。")
#strong[#text("复杂度")]#text("：加整条直线 ")#raw("O(log X)")#text("，查询 ")#raw("O(log X)")#text("，加线段最坏 ")#raw("O(log^2 X)")#text("，")#raw("X = R-L+1")
#strong[#text("横坐标")]#text("：整数闭区间 ")#raw("[L, R]")
#strong[#text("注意")]#text("：比较优劣直接使用 ")#raw(">")#text("，不要用 ")#raw("EPS")#text(" 改变李超树的递归判断；精度要求高时可整体改为 ")#raw("long double")#text("。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("init(L, R)")],[#text("清空并设置整数横坐标范围")],
[#raw("add_line(k, b)")],[#text("全区间加入 ")#raw("y = kx + b")],
[#raw("add_seg(k, b, l, r)")],[#text("仅在 ")#raw("[l, r]")#text(" 加入直线")],
[#raw("query(x)")],[#text("查询 ")#raw("x")#text(" 处最大值")],
)])



=== #text("现场记忆")

#raw("结构与整数李超树完全相同，只是 k、b、答案改成浮点。\n\n节点仍保存 mid 处更优的线：\nmid 更优先 swap；\nl / mid 优劣不同走左，否则走右。\n\n这里 x 仍是整数离散坐标，不是连续实数域。\n不要用 EPS 改写“谁更优”的严格比较。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
LiChaoD t(-5, 5);
t.add_line(2, 1);
t.add_seg(-1, 5, -2, 3);
assert(t.query(2) == 5);
```

=== 模板引用

#source_code(("data_structure/li_chao_double.hpp",))


== #text("栈维护凸包最值") <algo-data-structure-line-hull>

#text("原稿来源：4.18 栈维护凸包最值")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：斜率按非降序加入直线，维护 ")#raw("y = kx + b")#text(" 的最大值；任意查询点可二分，查询点非降时可用单调指针。")
#strong[#text("复杂度")]#text("：加线均摊 ")#raw("O(1)")#text("；")#raw("query(x)")#text(" 为 ")#raw("O(log n)")#text("；")#raw("query_mono(x)")#text(" 在 ")#raw("x")#text(" 非降时均摊 ")#raw("O(1)")
#strong[#text("限制")]#text("：加入直线的斜率 ")#raw("k")#text(" 必须非降；")#raw("query_mono()")#text(" 的查询横坐标必须非降；最终答案需落在 ")#raw("long long")#text(" 范围内。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("clear()")],[#text("清空凸包和单调指针")],
[#raw("add_line(k, b)")],[#text("按非降斜率加入直线")],
[#raw("query(x)")],[#text("任意 ")#raw("x")#text(" 二分查询最大值")],
[#raw("query_mono(x)")],[#raw("x")#text(" 非降时单调指针查询")],
)])



=== #text("现场记忆")

#raw("最大值 + 斜率递增。\n\n同斜率只保留 b 更大的线。\n\n三条线 a,b,c：\n若交点 x(a,b) >= x(b,c)，则 b 永远无用，弹掉。\n交叉乘必须用 __int128。\n\n任意 x：比较相邻函数值二分峰值。\nx 非降：while next(x) >= cur(x)，指针右移。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("add_line 的 k 按非降序，query_mono 的 x 按非降序；query 支持任意 x。容器 hull 为 0-indexed，查询是标量坐标。空凸包返回 LLONG_MIN；clear 重置指针。中间交叉乘也须在 i128 内，不能据此支持无限精度的任意 ll 极值组合。")

=== 最小调用示例

```cpp
LineHull h;
h.add_line(-1, 2);
h.add_line(1, 0);
assert(h.query(3) == 3);
```

=== 模板引用

#source_code(("data_structure/line_hull.hpp",))


== #text("动态开点线段树：区间赋值 + 剩余数量") <algo-data-structure-dynamic-segment-tree>

#text("原稿来源：4.19 动态开点线段树：区间赋值 + 剩余数量")


#strong[#text("用途")]#text("：在巨大整数值域上维护 ")#raw("0/1")#text(" 状态，支持区间赋值，并快速查询全局剩余数量。")
#strong[#text("复杂度")]#text("：区间赋值 ")#raw("O(log U)")#text("，查询总剩余数量 ")#raw("O(1)")#text("；空间 ")#raw("O(q log U)")
#strong[#text("下标")]#text("：")#raw("1-indexed")#text("，值域 ")#raw("[1, n]")
#strong[#text("约定")]#text("：初始所有位置均为 ")#raw("1")#text("；")#raw("1")#text(" 表示剩余，")#raw("0")#text(" 表示占用。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("init(n)")],[#text("重置为 ")#raw("[1,n]")#text(" 全部剩余")],
[#raw("assign(l,r,v)")],[#text("将 ")#raw("[l,r]")#text(" 全部赋为 ")#raw("v")],
[#raw("remain()")],[#text("当前剩余数量")],
[#raw("occupied()")],[#text("当前占用数量")],
)])



=== #text("现场记忆")

#raw("节点新建时代表整段全为 1，不是“未知”。\n\ntag = 0/1：整段统一；\ntag = -1：已经下传、左右状态不同。\n\n部分修改前必须 push_down：\n未创建的儿子要继承父亲当前 tag。\n\n不要把 tr[p].lc / rc 按引用传进会 push_back 的递归函数。",block:true)



=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
DynSeg t(10);
t.assign(3, 7, 0);
assert(t.remain() == 5);
t.assign(5, 6, 1);
assert(t.remain() == 7);
```

=== 模板引用

#source_code(("data_structure/dynamic_segment_tree.hpp",))


== #text("线段树合并：点计数 + 区间最值") <algo-data-structure-segment-tree-merge>

#text("原稿来源：4.20 线段树合并：点计数 + 区间最值")


#strong[#text("用途")]#text("：维护多棵动态权值线段树，支持单点计数，并将两棵树直接合并；常用于子树颜色统计等问题。")
#strong[#text("复杂度")]#text("：单点修改 ")#raw("O(log V)")#text("；一次合并与实际同时存在的节点数有关，若每棵树只被破坏性合并一次，总合并复杂度 ")#raw("O(总节点数)")
#strong[#text("下标")]#text("：权值域 ")#raw("[1, V]")
#strong[#text("限制")]#text("：默认计数保持非负；最大值相同时返回较小的权值。")
#strong[#text("注意")]#text("：")#raw("merge(x,y)")#text(" 为破坏性合并，返回的新根可能直接复用 ")#raw("y")#text(" 的节点，合并后不要再独立使用旧树 ")#raw("y")#text("。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口 / 变量")],[#text("含义")]),
[#raw("init(n,V)")],[#text("初始化 ")#raw("n")#text(" 个根，权值域 ")#raw("[1,V]")],
[#raw("add(id,pos,delta)")],[#text("第 ")#raw("id")#text(" 棵树的 ")#raw("pos")#text(" 增加 ")#raw("delta")],
[#raw("merge(x,y)")],[#text("合并两个根并返回新根")],
[#raw("root[id]")],[#text("第 ")#raw("id")#text(" 棵线段树的根")],
[#raw("tr[p].mx")],[#text("当前区间最大计数")],
[#raw("tr[p].pos")],[#text("最大计数对应的最小权值")],
)])



=== #text("现场记忆")

#raw("单点修改一路动态开点，叶子保存该权值出现次数。\n\nmerge(x,y)：\n一边为空 -> 直接复用另一边；\n叶子 -> 次数相加；\n否则左右儿子分别递归合并，再 pull。\n\npull 保存区间最大次数；\n相同次数优先左儿子 -> 权值更小。\n\nmerge 是破坏性的，旧的 y 不要再单独修改或使用。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("计数非负，存在正计数时最大频次相同取较小权值；频次全部为 0 时 mx=0、pos=0，表示没有出现的权值，包含删除至空和两棵空树合并。破坏性 merge 后将被吸收 root 清零，禁止把共享节点再当独立树修改。")

=== 最小调用示例

```cpp
SegMerge s(2, 5);
s.add(1, 2, 1);
s.add(2, 3, 2);
s.root[1] = s.merge(s.root[1], s.root[2]);
s.root[2] = 0;
assert(s.tr[s.root[1]].pos == 3);
```

=== 模板引用

#source_code(("data_structure/segment_tree_merge.hpp",))


== #text("主席树：Info 单点修改") <algo-data-structure-persistent-segment-tree-point>

#text("原稿来源：主席树：Info 单点修改")


#strong[#text("用途")]#text("：维护序列的多个历史版本，支持单点修改和区间查询；适合可持久化数组、历史区间和、最值等问题。")

#strong[#text("复杂度")]#text("：建树 ")#raw("O(n)")#text("；单次修改、查询 ")#raw("O(log n)")#text("；每次修改新增 ")#raw("O(log n)")#text(" 个节点。")

#strong[#text("下标")]#text("：序列位置默认 ")#raw("1-indexed")#text("。")

#strong[#text("限制")]#text("：")#raw("Info()")#text(" 必须是信息合并的单位元，")#raw("operator +")#text(" 定义左右区间信息合并。")

#strong[#text("注意")]#text("：")
#text("• ")#raw("modify()")#text(" 是单点赋值。")
#text("• 初始序列全部为 ")#raw("Info()")#text(" 时，可以直接令 ")#raw("root[0] = 0")#text("，无需建树。")


#text("有初始数组：")
#raw("PST pst;\nvector<int> root(q + 1);\nroot[0] = pst.build(a, 1, n);\nroot[1] = pst.modify(root[0], 1, n, pos, Info(x));\nInfo ans = pst.query(root[1], 1, n, l, r);",lang:"cpp",block:true)

#text("初始全部为单位元：")
#raw("root[0] = 0;\nroot[i] = pst.modify(root[ver], 1, n, pos, Info(x));",lang:"cpp",block:true)

=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("build(a,l,r)")],[#text("建立初始版本")],
[#raw("modify(pre,l,r,pos,v)")],[#text("基于 ")#raw("pre")#text(" 单点赋值，返回新根")],
[#raw("query(root,l,r,L,R)")],[#text("查询某个版本区间信息")],
[#raw("Info()")],[#text("合并单位元")],
[#raw("operator +")],[#text("合并左右区间信息")],
)])



=== #text("现场记忆")

#raw("位置域主席树。\n\nmodify：\nclone 当前节点，只复制根到叶子的路径。\n\n叶子直接赋值，回溯 pull。\n\nroot[new] = modify(root[old], ...)\n旧版本完全不变。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
PST p;
int r = p.modify(0, 1, 3, 2, Info(5));
assert(p.query(r, 1, 3, 1, 3).sum == 5 &&
       p.query(0, 1, 3, 1, 3).sum == 0);
```

=== 模板引用

#source_code(("data_structure/persistent_segment_tree_point.hpp",))


== #text("主席树：InfoTag 区间修改") <algo-data-structure-persistent-segment-tree-lazy>

#text("原稿来源：主席树：InfoTag 区间修改")


#strong[#text("用途")]#text("：维护序列的多个历史版本，支持区间修改、区间查询和历史版本访问。")

#strong[#text("复杂度")]#text("：建树 ")#raw("O(n)")#text("；单次区间修改、查询 ")#raw("O(log n)")#text("；每次修改新增 ")#raw("O(log n)")#text(" 个节点。")

#strong[#text("下标")]#text("：序列位置默认 ")#raw("1-indexed")#text("。")

#strong[#text("限制")]#text("：")
#text("• ")#raw("Info()")#text(" 必须是信息合并单位元。")
#text("• ")#raw("Tag()")#text(" 必须是空标记。")
#text("• 修改 ")#raw("Tag")#text(" 时需要同步修改 ")#raw("Info::apply()")#text("、")#raw("Tag::apply()")#text(" 和 ")#raw("Tag::empty()")#text("。")

#strong[#text("注意")]#text("：")
#text("• ")#raw("push()")#text(" 时不能直接修改旧儿子，必须先 ")#raw("clone()")#text("。")
#text("• ")#raw("query()")#text(" 不下传、不创建节点，而是携带祖先懒标记。")
#text("• 如果 Tag 不满足交换律，")#raw("tag.apply(t)")#text(" 的语义固定为：在当前 Tag 之后再执行 ")#raw("t")#text("。")


#text("有初始数组：")
#raw("PST pst;\nvector<int> root(q + 1);\nroot[0] = pst.build(a, 1, n);\nroot[1] = pst.modify(root[0], 1, n, l, r, Tag(x));\nInfo ans = pst.query(root[1], 1, n, l, r);",lang:"cpp",block:true)

#text("初始全部为单位元：")
#raw("root[0] = 0;\nroot[i] = pst.modify(root[ver], 1, n, l, r, Tag(x));",lang:"cpp",block:true)

=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("build(a,l,r)")],[#text("建立初始版本")],
[#raw("modify(pre,l,r,L,R,tag)")],[#text("基于旧版本区间修改")],
[#raw("query(root,l,r,L,R)")],[#text("查询某个版本区间信息")],
[#raw("Info::apply()")],[#text("标记作用到区间信息")],
[#raw("Tag::apply()")],[#text("合并两个懒标记")],
[#raw("Tag::empty()")],[#text("判断是否为空标记")],
)])



=== #text("现场记忆")

#raw("InfoTag 主席树 = Lazy Segment Tree + 路径复制。\n\nmodify 先 clone 当前节点。\n\npush 时：\n两个儿子都先 clone，再 apply。\n\nquery 不 push，不产生新节点。\n\n非交换 Tag：\n当前节点 tag 先执行，祖先携带 tag 后执行。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
PST p;
int r = p.modify(0, 1, 3, 1, 3, Tag(2));
assert(p.query(r, 1, 3, 2, 3).sum == 4 &&
       p.query(0, 1, 3, 1, 3).sum == 0);
```

=== 模板引用

#source_code(("data_structure/persistent_segment_tree_lazy.hpp",))


== #text("主席树：值域计数") <algo-data-structure-persistent-segment-tree-count>

#text("原稿来源：主席树：值域计数")


#strong[#text("用途")]#text("：维护前缀值域频率，支持区间第 k 小、排名、频率、前驱后继等查询。")

#strong[#text("复杂度")]#text("：建树 ")#raw("O(n log V)")#text("；单次修改、查询、第 k 小 ")#raw("O(log V)")#text("；空间 ")#raw("O(n log V)")#text("。")

#strong[#text("下标")]#text("：原数组默认 ")#raw("1-indexed")#text("；线段树维护离散化值域 ")#raw("[1,m]")#text("。")

#strong[#text("限制")]#text("：使用前需要把所有可能出现的值加入离散化。")

#strong[#text("注意")]#text("：")
#text("• ")#raw("root[r] - root[l - 1]")#text(" 表示数组区间 ")#raw("[l,r]")#text(" 的值域多重集。")
#text("• ")#raw("kth()")#text(" 要保证 ")#raw("1 <= k <= r-l+1")#text("。")



#text("建立前缀版本（a[1..n]，n>=1）：")
#raw("vector<int> b(a.begin() + 1, a.end());\nsort(b.begin(), b.end());\nb.erase(unique(b.begin(), b.end()), b.end());\nint m = b.size();\nPST pst;\nvector<int> root(n + 1);\nfor (int i = 1; i <= n; i++)\n{\n    int pos = lower_bound(b.begin(), b.end(), a[i]) - b.begin() + 1;\n    root[i] = pst.modify(root[i - 1], 1, m, pos, 1);\n}\nint id = pst.kth(root[l - 1], root[r], 1, m, k);\nint kth_value = b[id - 1];",lang:"cpp",block:true)

#text("查询 ")#raw("<= x")#text(" 的数量：")
#raw("int p = upper_bound(b.begin(), b.end(), x) - b.begin();\nint ans = p ? pst.query(root[l - 1], root[r], 1, m, 1, p) : 0;",lang:"cpp",block:true)

=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("modify(pre,l,r,pos,v)")],[#text("某个值的出现次数加 ")#raw("v")],
[#raw("query(u,v,l,r,L,R)")],[#text("查询版本差中值域 ")#raw("[L,R]")#text(" 的数量")],
[#raw("kth(u,v,l,r,k)")],[#text("查询版本差中的第 ")#raw("k")#text(" 小")],
[#raw("count(u,v)")],[#text("两版本计数总差")],
)])



=== #text("常见查询")

#raw("区间 [l,r]：\nu = root[l-1]\nv = root[r]\n\n第 k 小：\nkth(u,v,...,k)\n\n<= x：\nupper_bound 得到最后一个 <= x 的离散位置，再 query 前缀。\n\n[x,y] 出现次数：\nquery 对应离散值域。\n\n严格前驱 < x：\nc = 区间中 < x 的数量\n答案 = kth(c)\n\n严格后继 > x：\nc = 区间中 <= x 的数量\n答案 = kth(c+1)",block:true)

=== #text("现场记忆")

#raw("值域主席树的核心是版本差。\n\nroot[r] - root[l-1] = [l,r] 的值域频率。\n\nkth：\n看左儿子的计数差。\n\n左边够 k -> 左走。\n否则右走，k -= 左边数量。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
PST p;
int a = p.modify(0, 1, 5, 2, 1), b = p.modify(a, 1, 5, 4, 1);
assert(p.kth(0, b, 1, 5, 2) == 4 && p.query(a, b, 1, 5, 1, 5) == 1);
```

=== 模板引用

#source_code(("data_structure/persistent_segment_tree_count.hpp",))


== #text("主席树：区间不同颜色数量") <algo-data-structure-persistent-distinct>

#text("原稿来源：主席树：区间不同颜色数量")


#strong[#text("用途")]#text("：在线查询静态数组区间 ")#raw("[l,r]")#text(" 中不同值的数量。")

#strong[#text("复杂度")]#text("：建树 ")#raw("O(n log n)")#text("，单次查询 ")#raw("O(log n)")#text("，空间 ")#raw("O(n log n)")#text("。")

#strong[#text("下标")]#text("：数组位置和线段树位置均为 ")#raw("1-indexed")#text("。")

#strong[#text("注意")]#text("：")
#text("• 这是位置域主席树，不是值域计数主席树。")
#text("• 对于版本 ")#raw("root[r]")#text("，每种颜色只在最后出现位置保留一个 ")#raw("1")#text("。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("build(a)")],[#text("建立所有前缀版本")],
[#raw("query(l,r)")],[#text("查询 ")#raw("[l,r]")#text(" 不同值数量")],
[#raw("root[r]")],[#text("处理完前缀 ")#raw("[1,r]")#text(" 的版本")],
)])



=== #text("现场记忆")

#raw("位置 i 是否为某个颜色当前最后出现位置。\n\n加入 a[i]：\n旧 last 位置 -1\n当前位置 +1\n\nroot[r] 中每种颜色只留下最后一次出现。\n\n查询 [l,r] 内的 1 的数量即可。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
PSTColor p;
p.build(vector<int>{0, 0, 1, 0});
assert(p.query(1, 3) == 2);
```

=== 模板引用

#source_code(("data_structure/persistent_distinct.hpp",))


== #text("主席树：区间 mex") <algo-data-structure-persistent-mex>

#text("原稿来源：主席树：区间 mex")


#strong[#text("用途")]#text("：查询静态数组区间 ")#raw("[l,r]")#text(" 的 mex。")

#strong[#text("复杂度")]#text("：建树 ")#raw("O(n log n)")#text("，单次查询 ")#raw("O(log n)")#text("，空间 ")#raw("O(n log n)")#text("。")

#strong[#text("下标")]#text("：数组位置 ")#raw("1-indexed")#text("；值域自然维护 ")#raw("[0,n]")#text("。")

#strong[#text("限制")]#text("：值 ")#raw("< 0")#text(" 或 ")#raw("> n")#text(" 不影响长度不超过 ")#raw("n")#text(" 的区间 mex，可以直接忽略。")

#strong[#text("注意")]#text("：")
#text("• 叶子 ")#raw("x")#text(" 保存 ")#raw("last[x]")#text("，即值 ")#raw("x")#text(" 最近出现的位置。")
#text("• 节点保存区间内最小的 ")#raw("last")#text("。")
#text("• 空节点的 ")#raw("mn = 0")#text("，因此不需要预建完整值域树。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("build(a)")],[#text("建立所有前缀版本")],
[#raw("modify()")],[#text("修改某个值的最后出现位置")],
[#raw("mex(root,l,r,L)")],[#text("找最小满足 ")#raw("last[x] < L")#text(" 的值")],
[#raw("query(l,r)")],[#text("查询数组 ")#raw("[l,r]")#text(" 的 mex")],
)])



=== #text("现场记忆")

#raw("值域叶子 x 保存 last[x]。\n\n查询 [L,R]：\n直接使用 root[R]。\n\nlast[x] < L\n说明 x 没在 [L,R] 中出现。\n\n左儿子 mn < L -> 左走。\n否则右走。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
PSTMex p;
p.build(vector<int>{0, 0, 1, 0});
assert(p.query(1, 3) == 2);
```

=== 模板引用

#source_code(("data_structure/persistent_mex.hpp",))


== #text("主席树：树上第 k 小") <algo-data-structure-persistent-tree-kth>

#text("原稿来源：主席树：树上第 k 小")


#strong[#text("用途")]#text("：查询树上路径 ")#raw("u -> v")#text(" 中第 ")#raw("k")#text(" 小的点权。")

#strong[#text("复杂度")]#text("：建主席树 ")#raw("O(n log V)")#text("；单次查询 ")#raw("O(log V)")#text("，另需 LCA 查询复杂度。")

#strong[#text("下标")]#text("：树节点默认 ")#raw("1-indexed")#text("；值域使用离散化后的 ")#raw("[1,m]")#text("。")

#strong[#text("限制")]#text("：需要配合 LCA；点权需要预先离散化。")

#strong[#text("注意")]#text("：")
#text("• ")#raw("root[u]")#text(" 表示根节点到 ")#raw("u")#text(" 路径上的值域计数。")
#text("• 根节点父亲设为 ")#raw("0")#text("，并令 ")#raw("root[0] = 0")#text("。")


#text("DFS 建版本时：")
#raw("int pos = lower_bound(b.begin(), b.end(), a[u]) - b.begin() + 1;\nroot[u] = pst.modify(root[fa[u][0]], 1, m, pos);",lang:"cpp",block:true)

#text("查询路径：")
#raw("int w = lca(u, v);\nint p = fa[w][0];\nint id = pst.kth(root[u], root[v], root[w], root[p], 1, m, k);\nint ans = b[id - 1];",lang:"cpp",block:true)

=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("modify(pre,l,r,pos)")],[#text("根到当前点的版本加入一个点权")],
[#raw("kth(a,b,c,d,l,r,k)")],[#text("查询 ")#raw("a+b-c-d")#text(" 多重集中的第 ")#raw("k")#text(" 小")],
)])



=== #text("现场记忆")

#raw("root[u] = 根 -> u 的值域计数。\n\nu -> v 路径：\n\nroot[u] + root[v]\n- root[lca]\n- root[fa[lca]]\n\n之后和普通值域 kth 完全一样。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
PSTTreeKth p;
int r1 = p.modify(0, 1, 5, 2), r2 = p.modify(r1, 1, 5, 4),
    r3 = p.modify(r1, 1, 5, 1);
assert(p.kth(r2, r3, r1, 0, 1, 5, 2) == 2);
```

=== 模板引用

#source_code(("data_structure/persistent_tree_kth.hpp",))


== #text("可持久化 01 Trie") <algo-data-structure-persistent-trie>

#text("原稿来源：4.23 可持久化 01 Trie")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：维护多个二进制 Trie 版本，查询两个版本计数差中与 ")#raw("x")#text(" 异或的最大值或最小值；前缀建版本后可直接查询数组区间。")
#strong[#text("复杂度")]#text("：单次修改 / 查询 ")#raw("O(B)")#text("；空间每次修改新增 ")#raw("O(B)")#text(" 个节点")
#strong[#text("位数")]#text("：维护 ")#raw("[B,0]")#text(" 共 ")#raw("B+1")#text(" 位，默认 ")#raw("B=30")
#strong[#text("限制")]#text("：查询的两个版本计数差必须构成非空合法多重集；删除时必须保证对应元素存在。")
#strong[#text("注意")]#text("：")#raw("modify(from,from,...)")#text(" 会覆盖版本槽位，但其他历史版本不受影响；旧节点不会回收。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口 / 变量")],[#text("含义")]),
[#raw("init(versions,B)")],[#text("初始化版本槽位与最高位")],
[#raw("build(a)")],[#text("建立数组前缀版本")],
[#raw("modify(from,to,x,delta)")],[#text("修改 ")#raw("x")#text(" 出现次数")],
[#raw("insert(from,to,x)")],[#text("插入一个 ")#raw("x")],
[#raw("max_xor(vl,vr,x)")],[#text("查询版本差中的最大异或值")],
[#raw("min_xor(vl,vr,x)")],[#text("查询版本差中的最小异或值")],
[#raw("range_max_xor(l,r,x)")],[#text("查询数组区间最大异或值")],
[#raw("range_min_xor(l,r,x)")],[#text("查询数组区间最小异或值")],
[#raw("root[v]")],[#text("第 ")#raw("v")#text(" 个版本根")],
)])



=== #text("现场记忆")

#raw("可持久化 Trie = 每次只复制根到叶子的二进制路径。\n\n版本区间仍然是：\nroot[R] - root[L-1]。\n\n最大异或：\n优先走 x 当前位的反位，前提是版本差中该分支 cnt > 0。\n\n最小异或：\n优先走同位，不存在才走反位。\n\nmodify(from, to, ...)，from == to 可直接覆盖版本槽位。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
PTrie01 p(0, 5);
p.build(vector<unsigned long long>{0, 1, 4, 7});
assert(p.range_max_xor(1, 3, 2) == 6 && p.range_min_xor(1, 3, 2) == 3);
```

=== 模板引用

#source_code(("data_structure/persistent_trie.hpp",))


== #text("前缀线性基：区间异或最大值") <algo-data-structure-prefix-linear-basis>

#text("原稿来源：4.24 前缀线性基：区间异或最大值")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：维护数组每个前缀的线性基，查询区间 ")#raw("[l,r]")#text(" 子集异或最大值。")
#strong[#text("复杂度")]#text("：插入 / 查询 ")#raw("O(B)")#text("，建立全部前缀 ")#raw("O(nB)")#text("，空间 ")#raw("O(nB)")
#strong[#text("限制")]#text("：默认维护非负 ")#raw("int")#text("，最高位为 ")#raw("LOG=30")#text("。")





=== #text("现场记忆")

#raw("pre[i] = pre[i-1]，再插入 (a[i], i)。\n\n每个最高位优先保留 pos 更大的基向量：\n若新向量 id 更大，先 swap，再继续消元。\n\n查询 [l,r] 只看 pre[r]：\n仅允许 pos[i] >= l 的基向量参与贪心。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("a[1..n]，pre[0] 为空基。insert(x,id) 要求非负 int，维护位 30..0；query(l) 在 pre[r] 上求区间 [l,r] 最大子集异或。多测重新建立 pre 数组。")

=== 最小调用示例

```cpp
LinearBasis b;
b.insert(3, 1);
b.insert(5, 2);
assert(b.query(1) == 6 && b.query(2) == 5);
```

=== 模板引用

#source_code(("data_structure/prefix_linear_basis.hpp",))


== #text("FHQ Treap：按值平衡树") <algo-data-structure-fhq-set>

#text("原稿来源：4.25 FHQ Treap：按值平衡树")


#strong[#text("用途")]#text("：维护可重集，支持插入、删除一个值、排名、第 ")#raw("k")#text(" 小、严格前驱 / 后继。")
#strong[#text("复杂度")]#text("：单次操作期望 ")#raw("O(log n)")#text("，最坏 ")#raw("O(n)")#text("；空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：")#raw("kth(k)")#text(" 中 ")#raw("k")#text(" 从 ")#raw("1")#text(" 开始。")
#strong[#text("注意")]#text("：允许重复值；")#raw("pre / nxt")#text(" 不存在时的返回值按题意修改。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("clear()")],[#text("清空")],
[#raw("insert(x)")],[#text("插入一个 ")#raw("x")],
[#raw("erase(x)")],[#text("删除一个 ")#raw("x")],
[#raw("rank_of(x)")],[#raw("x")#text(" 的排名，即 ")#raw("< x")#text(" 的数量 ")#raw("+1")],
[#raw("kth(k)")],[#text("第 ")#raw("k")#text(" 小")],
[#raw("pre(x)")],[#text("严格前驱")],
[#raw("nxt(x)")],[#text("严格后继")],
[#raw("contains(x)")],[#text("是否存在")],
)])



=== #text("现场记忆")

#raw("按值 split：\n左树 <= val，右树 > val。\n\ninsert：\nsplit -> 中间塞新节点 -> merge。\n\nerase 一个 val：\n先拆 <=val / >val，\n再拆 <val / ==val，\n把 ==val 那棵树的根删掉一个。\n\n排名 = 小于 val 的数量 + 1。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("可重复值；kth 仅接受 1..size(root)，不存在的前驱 / 后继以 INT_MIN / INT_MAX 表示，会与合法极值混淆，应用层应先判断是否存在。删除节点不回收，空间按累计插入量而非当前 size 估算。")

=== 最小调用示例

```cpp
FHQSet s;
s.insert(3);
s.insert(1);
s.insert(3);
s.erase(3);
assert(s.kth(2) == 3 && s.rank_of(3) == 2);
```

=== 模板引用

#source_code(("data_structure/fhq_set.hpp",))


== #text("FHQ Treap：隐式序列") <algo-data-structure-fhq-sequence>

#text("原稿来源：4.26 FHQ Treap：隐式序列")


#strong[#text("用途")]#text("：按序列位置维护动态序列，支持插入、删除区间、区间翻转，并方便扩展区间信息和 lazy。")
#strong[#text("复杂度")]#text("：单次 ")#raw("split / merge / 区间操作")#text(" 期望 ")#raw("O(log n)")#text("，最坏 ")#raw("O(n)")#text("；空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：序列位置 ")#raw("1-indexed")
#strong[#text("注意")]#text("：")#raw("split(root,k)")#text(" 的左树恰好包含前 ")#raw("k")#text(" 个元素。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("build(a)")],[#text("按 ")#raw("a")#text(" 的顺序建树")],
[#raw("insert(pos,x)")],[#text("插入后成为第 ")#raw("pos")#text(" 个")],
[#raw("erase(l,r)")],[#text("删除 ")#raw("[l,r]")],
[#raw("reverse(l,r)")],[#text("翻转 ")#raw("[l,r]")],
[#raw("to_vector()")],[#text("导出当前序列")],
[#raw("split(root,k)")],[#text("拆成前 ")#raw("k")#text(" 个 / 剩余部分")],
[#raw("merge(x,y)")],[#text("拼接两段序列")],
)])



=== #text("现场记忆")

#raw("隐式 FHQ 不按 val split，而按 siz split。\n\nsplit(root,k)：\n左边前 k 个，右边剩余。\n\n区间 [l,r]：\nsplit(root,l-1)\nsplit(mid,r-l+1)\n操作中间树\n再 merge 回去。\n\n加 lazy 时只改 Node / apply_xxx / push_down / push_up。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("build 的输入是 0-indexed 容器，树上位置为 1-indexed；to_vector 输出 0-indexed。clear / build 清空当前树，但不回收其他对象。随机 Treap 最坏可能退化，空间按累计插入量计算。")

=== 最小调用示例

```cpp
FHQSeq s;
s.build(vector<int>{1, 2, 3});
s.reverse(1, 3);
assert((s.to_vector() == vector<int>{3, 2, 1}));
```

=== 模板引用

#source_code(("data_structure/fhq_sequence.hpp",))


== #text("可持久化 FHQ Treap：按值平衡树") <algo-data-structure-persistent-fhq-set>

#text("原稿来源：4.27 可持久化 FHQ Treap：按值平衡树")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：维护多个历史版本的可重集，支持插入、删除、排名、第 ")#raw("k")#text(" 小、前驱 / 后继。")
#strong[#text("复杂度")]#text("：单次修改期望 ")#raw("O(log n)")#text("，新增期望 ")#raw("O(log n)")#text(" 个节点；查询期望 ")#raw("O(log n)")
#strong[#text("版本")]#text("：")#raw("modify(from,to,...)")#text(" 风格，")#raw("from == to")#text(" 可直接覆盖版本槽位。")
#strong[#text("注意")]#text("：所有修改路径都必须复制；覆盖版本槽位不会影响其他历史版本，但旧节点不会回收。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口 / 变量")],[#text("含义")]),
[#raw("init(versions)")],[#text("初始化版本槽位")],
[#raw("insert(from,to,x)")],[#text("基于 ")#raw("from")#text(" 插入，写入 ")#raw("to")],
[#raw("erase(from,to,x)")],[#text("基于 ")#raw("from")#text(" 删除一个 ")#raw("x")],
[#raw("rank_of(ver,x)")],[#text("版本 ")#raw("ver")#text(" 中 ")#raw("x")#text(" 的排名")],
[#raw("kth(ver,k)")],[#text("版本 ")#raw("ver")#text(" 中第 ")#raw("k")#text(" 小")],
[#raw("pre(ver,x)")],[#text("严格前驱")],
[#raw("nxt(ver,x)")],[#text("严格后继")],
[#raw("root[v]")],[#text("第 ")#raw("v")#text(" 个版本根")],
)])



=== #text("现场记忆")

#raw("可持久化 FHQ：\nsplit / merge 只要准备修改节点，就先 clone。\n\ninsert(from,to,x)：\n对 root[from] split，最后结果写 root[to]。\n\nfrom == to 也安全：\n先算完整新根，再覆盖版本槽位。\n\nerase 一个重复值仍然是：\n<val / ==val / >val，删除 ==val 树根一个。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("版本从 0 开始，初始为空；from 必须存在，to 可自动扩容或覆盖。kth 为 1-indexed。不存在的前驱 / 后继为 INT_MIN / INT_MAX。init 重置所有版本，已分配历史节点不在单次删除时回收。")

=== 最小调用示例

```cpp
PersistentFHQSet s;
s.insert(0, 1, 3);
s.insert(1, 2, 1);
s.erase(2, 2, 3);
assert(s.kth(1, 1) == 3 && s.kth(2, 1) == 1);
```

=== 模板引用

#source_code(("data_structure/persistent_fhq_set.hpp",))


== #text("可持久化 FHQ Treap：隐式序列") <algo-data-structure-persistent-fhq-sequence>

#text("原稿来源：4.28 可持久化 FHQ Treap：隐式序列")


#strong[#text("用途")]#text("：维护多个历史版本的动态序列，支持任意版本插入、删除区间、区间翻转和只读查询。")
#strong[#text("复杂度")]#text("：单次修改期望 ")#raw("O(log n)")#text("，新增期望 ")#raw("O(log n)")#text(" 个节点；")#raw("kth")#text(" 查询期望 ")#raw("O(log n)")
#strong[#text("下标")]#text("：序列位置 ")#raw("1-indexed")
#strong[#text("版本")]#text("：")#raw("modify(from,to,...)")#text(" 风格，")#raw("from == to")#text(" 可覆盖当前版本槽位。")
#strong[#text("注意")]#text("：")#raw("push_down")#text(" 若要修改儿子，必须先复制儿子；纯查询不要 ")#raw("push_down")#text("。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口 / 变量")],[#text("含义")]),
[#raw("build(ver,a)")],[#text("在版本 ")#raw("ver")#text(" 建初始序列")],
[#raw("insert(from,to,pos,x)")],[#text("插入后成为第 ")#raw("pos")#text(" 个")],
[#raw("erase(from,to,l,r)")],[#text("删除 ")#raw("[l,r]")],
[#raw("reverse(from,to,l,r)")],[#text("翻转 ")#raw("[l,r]")],
[#raw("kth(ver,k)")],[#text("查询第 ")#raw("k")#text(" 个元素")],
[#raw("to_vector(ver)")],[#text("导出指定版本")],
[#raw("root[v]")],[#text("第 ")#raw("v")#text(" 个版本根")],
)])



=== #text("现场记忆")

#raw("持久化隐式 FHQ 有两层要求：\n\nsplit / merge：\n当前要修改的节点先 clone。\n\npush_down：\n若 lazy 会修改儿子，\n两个儿子也必须先 clone。\n\n区间操作仍是两次 split。\n\n纯查询不能 push_down；\n用 flip 记录祖先尚未实际下传的 reverse。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("build(ver,a) 的输入 a 为 0-indexed 容器，树上位置和 kth 为 1-indexed；插入位置 1..size+1，删除和翻转必须是合法非空闭区间。init 清空全部版本；build 单个版本不会清空其他版本。")

=== 最小调用示例

```cpp
PersistentFHQSeq s;
s.build(0, vector<int>{1, 2, 3});
s.reverse(0, 1, 1, 3);
assert(s.kth(0, 1) == 1 && s.kth(1, 1) == 3);
```

=== 模板引用

#source_code(("data_structure/persistent_fhq_sequence.hpp",))


== #text("可持久化隐式 FHQ：区间和扩展") <algo-data-structure-persistent-fhq-sum>

#text("原稿来源：4.29 可持久化隐式 FHQ：区间和扩展")

#strong[#text("用途")]#text("：可持久化隐式序列的区间和，查询只读，不分配节点。")
#strong[#text("复杂度")]#text("：修改及查询期望 O(log n)，修改新增期望 O(log n) 节点。")
#strong[#text("下标")]#text("：序列位置及闭区间 1-indexed；输入 build 数组 0-indexed。")
#strong[#text("限制")]#text("：lazy 仅 rev；值和区间和须能放入 long long。")
=== #text("现场记忆")
#text("sum 对翻转不敏感；查询携带 flip，不 split、不 push_down。")

=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

#text("原稿给出的是增量片段；此处已整合为完整独立版本，build / insert / kth / to_vector 全部使用 long long，不会在导出时截断。range_sum 纯只读，只支持 rev 标记。")

=== 接口、下标与适用边界补充

#text("与序列版相同，build 输入 0-indexed，操作区间 1-indexed。区间和查询不增加 tr.size()，即使祖先有 rev 也不修改历史节点。")

=== 最小调用示例

```cpp
PersistentFHQSum s;
s.build(0, vector<ll>{1, 2, 3});
s.reverse(0, 1, 1, 3);
assert(s.kth(0, 1) == 1 && s.kth(1, 1) == 3);
```

=== 模板引用

#source_code(("data_structure/persistent_fhq_sum.hpp",))


== #text("树状数组：动态第 k 小 / 前 k 小之和") <algo-data-structure-fenwick-k-sum>

#text("原稿来源：4.30 树状数组：动态第 k 小 / 前 k 小之和")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：值域可离散化时，动态维护多重集合，支持插入 / 删除、第 ")#raw("k")#text(" 小、前 ")#raw("k")#text(" 小之和，以及 ")#raw("<=x")#text(" 的数量 / 总和。")
#strong[#text("复杂度")]#text("：预处理离散化 ")#raw("O(V log V)")#text("；修改 / 查询均 ")#raw("O(log V)")#text("；空间 ")#raw("O(V)")
#strong[#text("重复值")]#text("：支持。")
#strong[#text("限制")]#text("：所有可能修改的值必须提前加入离散化值域；删除时保证该值当前存在；总和保证不超过 ")#raw("long long")#text("。")
#strong[#text("优势")]#text("：")#raw("k")#text(" 不需要固定，每次查询可以不同。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("init(values)")],[#text("离散化所有可能出现的值并清空")],
[#raw("insert(x)")],[#text("插入一个 ")#raw("x")],
[#raw("erase(x)")],[#text("删除一个 ")#raw("x")],
[#raw("modify(x,delta)")],[#raw("x")#text(" 的出现次数增加 ")#raw("delta")],
[#raw("size()")],[#text("当前元素总数")],
[#raw("sum()")],[#text("当前所有元素之和")],
[#raw("kth(k)")],[#text("第 ")#raw("k")#text(" 小，")#raw("1 <= k <= size()")],
[#raw("sum_k(k)")],[#text("前 ")#raw("k")#text(" 小之和，")#raw("0 <= k <= size()")],
[#raw("count_le(x)")],[#raw("<=x")#text(" 的元素个数")],
[#raw("sum_le(x)")],[#raw("<=x")#text(" 的元素之和")],
)])



=== #text("现场记忆")

#raw("离散化后开两棵 BIT：\ncnt 维护出现次数，sum 维护实际值之和。\n\n第 k 小：\n在 cnt 上 Fenwick 倍增，O(log V)。\n\n前 k 小和：\n倍增时同时累加完整跳过部分的 cnt / sum，\n最后补 (k-cnt) * 第 k 小值。\n\n删除不能让某个值的出现次数变成负数。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("值容器 0-indexed，内部离散值域 1-indexed。修改值必须在预收集值域中，不能删除不存在的值；kth 需 1<=k<=size，sum_k 需 0<=k<=size。计数、总和和 delta*x 均须在 long long 内。")

=== 最小调用示例

```cpp
FenwickKSum s(vector<ll>{-1, 2, 5});
s.insert(-1);
s.insert(5);
assert(s.sum_k(1) == -1 && s.kth(2) == 5);
```

=== 模板引用

#source_code(("data_structure/fenwick_k_sum.hpp",))


== #text("双 multiset 维护前 k 小之和") <algo-data-structure-multiset-k-sum>

#text("原稿来源：4.31 双 multiset 维护前 k 小之和")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：动态维护多重集合中最小的 ")#raw("k")#text(" 个元素及其和，同时可得到第 ")#raw("k")#text(" 小；常用于滑动窗口。")
#strong[#text("复杂度")]#text("：固定 ")#raw("k")#text(" 时插入 / 删除 ")#raw("O(log n)")#text("，查询和 ")#raw("O(1)")
#strong[#text("重复值")]#text("：支持。")
#strong[#text("注意")]#text("：元素不足 ")#raw("k")#text(" 时，")#raw("sum()")#text(" 为当前所有元素之和，")#raw("full()")#text(" 为 ")#raw("false")#text("；")#raw("kth()")#text(" 仅在 ")#raw("k>0 && full()")#text(" 时调用。")
#strong[#text("限制")]#text("：假设前 ")#raw("k")#text(" 小元素之和可以放入 ")#raw("long long")#text("。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("init(k)")],[#text("清空并重新设置 ")#raw("k")],
[#raw("insert(x)")],[#text("插入一个 ")#raw("x")],
[#raw("erase(x)")],[#text("删除一个 ")#raw("x")#text("，不存在则忽略")],
[#raw("sum()")],[#text("当前最小 ")#raw("min(k,size)")#text(" 个元素之和")],
[#raw("full()")],[#text("当前元素数是否至少为 ")#raw("k")],
[#raw("kth()")],[#text("第 ")#raw("k")#text(" 小，仅在 ")#raw("k>0 && full()")#text(" 时调用")],
)])



=== #text("现场记忆")

#raw("small 保存最小的 min(k,n) 个数，\nlarge 保存其余数。\n\n始终维护：\nsmall.size() <= k\nmax(small) <= min(large)\n\nsum_small 只统计 small。\n\n第 k 小 = max(small)，\n调用前保证 k > 0 且 full()。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("init(k) 要求 k>=0。full 在 k=0 时为真，但 kth 仍不允许调用。init 清空两集合。")

=== 最小调用示例

```cpp
KSum s(2);
s.insert(3);
s.insert(1);
s.insert(2);
assert(s.sum() == 3 && s.kth() == 2);
```

=== 模板引用

#source_code(("data_structure/multiset_k_sum.hpp",))


== #text("set 维护不相交区间集合") <algo-data-structure-interval-union>

#text("原稿来源：4.32 set 维护不相交区间集合")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：动态维护若干区间的并，插入新区间时自动合并所有重叠或相邻区间；也支持删除一段区间。")
#strong[#text("复杂度")]#text("：插入 / 删除 ")#raw("O((k+1)\\log s)")#text("，")#raw("k")#text(" 为被合并或切割影响的区间数")
#strong[#text("区间")]#text("：闭区间 ")#raw("[l,r]")
#strong[#text("性质")]#text("：集合中任意两个区间始终不相交且不相邻。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口 / 变量")],[#text("含义")]),
[#raw("clear()")],[#text("清空")],
[#raw("add(l,r)")],[#text("加入区间并自动合并重叠 / 相邻区间")],
[#raw("erase(l,r)")],[#text("删除区间 ")#raw("[l,r]")#text("，必要时切割")],
[#raw("contains(x)")],[#text("查询点 ")#raw("x")#text(" 是否被覆盖")],
[#raw("seg")],[#text("当前所有极大不相交区间")],
)])



=== #text("现场记忆")

#raw("set 按左端点排序，始终保存极大区间。\n\nadd [l,r]：\nlower_bound(l)，先检查前驱；\n然后一路删除所有 first <= r+1 的区间并扩展 [l,r]。\n\nerase [l,r]：\n删除所有相交区间，\n若原区间伸出删除范围，则把左右残段重新插回。\n\n相邻区间也合并，所以判断是 r1 + 1 >= l2。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
IntSet s;
s.add(1, 3);
s.add(4, 6);
s.erase(2, 5);
assert(s.contains(1) && s.contains(6) && !s.contains(3));
```

=== 模板引用

#source_code(("data_structure/interval_union.hpp",))


== #text("set 维护不相交区间集合") <algo-data-structure-interval-ids>

#text("原稿来源：4.33 set 维护不相交区间集合")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：维护一组两两不相交的区间 ID，支持判断新区间是否冲突、插入 / 删除，以及找出或删除所有与 ")#raw("[l,r]")#text(" 相交的区间。")
#strong[#text("复杂度")]#text("：插入 / 删除 ")#raw("O(log s)")#text("；找出 ")#raw("k")#text(" 个相交区间 ")#raw("O(log s+k)")#text("；删除 ")#raw("k")#text(" 个相交区间 ")#raw("O(log s+k)")
#strong[#text("区间")]#text("：闭区间 ")#raw("[l,r]")#text("；相邻区间如 ")#raw("[1,2]")#text("、")#raw("[3,4]")#text(" 不算相交。")
#strong[#text("限制")]#text("：")#raw("active")#text(" 中的区间必须始终两两不相交；区间加入 ")#raw("active")#text(" 后，不要修改其左右端点。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("init(seg)")],[#text("绑定区间数组并清空集合")],
[#raw("insert(id)")],[#text("无相交区间时插入，成功返回 ")#raw("true")],
[#raw("erase(id)")],[#text("删除指定 ID")],
[#raw("overlap(l,r)")],[#text("是否存在与 ")#raw("[l,r]")#text(" 相交的活动区间")],
[#raw("ids(l,r)")],[#text("返回所有与 ")#raw("[l,r]")#text(" 相交的区间 ID")],
[#raw("erase_cross(l,r)")],[#text("删除并返回所有相交区间 ID")],
)])



=== #text("现场记忆")

#raw("active 按左端点排序，并保证区间两两不相交。\n\n找与 [l,r] 相交的第一个区间：\nlower_bound(l)，再检查前驱是否延伸到了 l。\n\n找到起点后一直向右扫，\n直到当前区间左端 > r。\n\n不要直接强插重叠区间，否则 first_cross 的性质失效。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
IntSet s(vector<IntSet::Seg>{{}, {1, 3}, {4, 6}, {2, 5}});
assert(s.insert(1) && s.insert(2) && !s.insert(3));
assert((s.ids(3, 4) == vector<int>{1, 2}));
```

=== 模板引用

#source_code(("data_structure/interval_ids.hpp",))


== #text("高精度整数 ")#raw("cpp_int") <algo-data-structure-big-integer>

#text("原稿来源：4.34 高精度整数 ")#raw("cpp_int")

#strong[#text("下标")]#text("：标量接口无数组下标；辅助容器按 C++ 原生 0-indexed，特定值域、版本及 k 的约定见下方接口。")


#strong[#text("用途")]#text("：处理超出内置整数范围的整数加减乘除、取模和比较。")
#strong[#text("复杂度")]#text("：设整数长度为 ")#raw("L")#text(" 个机器字块；加减 / 比较 ")#raw("O(L)")#text("，乘除复杂度依位数和 Boost 内部实现而定。")
#strong[#text("支持")]#text("：正数、负数、")#raw("0")#text("，以及 ")#raw("+ - * / %")#text("、比较、位运算、输入输出。")


=== #text("现场记忆")

#raw("GNU++17/20 直接用 boost::multiprecision::cpp_int。\n\n支持负数、除法、取模、比较和 cin / cout，\n普通整数写法基本可以直接套用。\n\n构造大位移要写：\nBigInt(1) << k\n不要先用 1LL << k 再赋值，否则可能已经溢出。",block:true)

=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

#text("依赖 Boost.Multiprecision 头文件；本地编译器未自带时需提供 Boost include 路径。除数非零；向内置整数转换前确认范围。")

=== 最小调用示例

```cpp
BigInt a = BigInt(1) << 200;
assert((a * a / a) == a);
assert((a + 123) % a == 123);
```

=== 模板引用

#source_code(("data_structure/big_integer.hpp",))

