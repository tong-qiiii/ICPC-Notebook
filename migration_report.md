# 第 4～10 章迁移记录

> 本文是原始迁移记录。后续已按用户要求引入全项目自动编号；下文保留原稿编号 / 无编号附项的描述是迁移时的历史策略，现以 [TYPST_STYLE.md](TYPST_STYLE.md#自动编号与稳定引用) 为准。算法内容与模板文件仍保留，最新编号验证见 [numbering_report.md](numbering_report.md)。

本次按原稿顺序迁移，保留全部 121 个二级条目及重要变体，新增 140 份 C++17 模板、7 个章节及对应测试。原始 Markdown 永久只读；前三章源码、章节和测试保持原样。逐项文件对应见文末清单和 `test/migration_manifest.json`。

| 章节 | 原稿条目 | 模板文件 | 主要验证 |
| --- | ---: | ---: | --- |
| 04 数据结构 | 39 | 40 | 区间操作、莫队、历史版本、可重集与暴力对照；重复初始化 |
| 05 区间贪心 / 双指针 | 5 | 9 | 子集枚举最优解、覆盖 / 分组、窗口与两端指针对拍 |
| 06 数学 | 18 | 22 | 组合数、数论穷举、消元解集、分解乘积、离散对数、积分解析值 |
| 07 计算几何 | 18 | 18 | 点包含、最近 / 最远点对、矩形、半平面交独立裁剪、圆与切线、MEC、Minkowski |
| 08 多项式 | 11 | 18 | 朴素卷积、FPS 关系与系数递推、求值 / 插值、递推、NTT 模数容量 |
| 09 字符串 | 13 | 13 | 朴素匹配、全子串统计、回文计数、后缀暴力排序、最小旋转 |
| 10 动态规划 | 17 | 20 | 背包枚举、树上枚举、数位穷举、铺砖 / 回路枚举、优化 DP、CDQ 八种取等 |
| 合计 | 121 | 140 | 新增 140 个对应 C++ 测试单元 |

## 编号、拆分及接口选择

- 第 4 章未编号的笛卡尔树及六份主席树继续保持无编号；不填造原稿缺失的 4.21 / 4.22。4.23 及后续编号保持原样。4.32 为合并相邻区间的区间并；4.33 为不允许重叠的活动区间 ID 集合，两者独立保留。
- 几何 7.7 / 7.8 的代码原本重复，保留两个条目与两份模板，不自行删并。
- 多项式 8.3 的 `poly998::Poly` 成员版、8.9 的独立 `Multipoint` 和 8.10 的独立 `ProductTree` 保持不同实现。8.2 的 FPS 成员因相互依赖集中在同一头文件，保留全部逆、对数、指数、幂、平方根、三角和反三角接口。8.6 拆出三种递推接口。
- 数学 6.18 原标题包含 Cipolla，但原稿实际只实现 Tonelli-Shanks；本次保留实际实现，没有声称迁移不存在的 Cipolla 代码。
- DP 章末未编号 CDQ 单独保留，支持三个布尔模板参数的全部八种 `< / <=` 配置；普通最长链仍可令点权为 1。Hamilton 路径后的 TSP 回起点公式、单调栈反向扫描说明也保留。
- 多次查询、版本或递归共用状态的数据结构保留轻量 `struct`。静态扫描、数论、字符串基础和单次 DP 使用普通函数；原稿中的莫队、双指针、数位及 WQS 骨架以简短回调参数表达题意，不再保留无功能的 TODO 或无法编译的条件。没有添加工程化图类、策略层或 getter / setter。

## 修复和必要调整

### 数据结构

- 滑窗参数无效时返回空，避免负长度转为巨大 `reserve`；笛卡尔树空数组返回根 0，避免 `stk[0]` 越界。
- Fenwick 倍增上界改为除法比较，避免左移比较的潜在溢出；零规模构造也初始化哨兵。`add(0,...)` 仍是非法调用，已明确限制。
- 值域分块的 `count_lt(INT_MIN)` 不再计算溢出的 `x-1`；空值域的前驱查询返回 -1。
- 两种普通线段树空构建时跳过递归；查询仍只接受合法非空区间。
- 合并线段树删除至全零时，原 pos 会残留最后一个删除的权值；现在叶子与合并结果的零计数统一令 pos=0，表示没有出现的值，增加随机增删及删除至空检查。正计数并列时仍取较小权值。
- 莫队保留原排序及指针移动，TODO 维护转为外部回调。带修版接受时间顺序的 `Change{pos,pre,now}`，回调接收当前值；原有离散化与临时数组要求在说明中保留。二次离线仍要求调用者提供实际前缀 / 后缀贡献，不能用零占位冒充实现。
- 只增回滚的最大相同值下标差变体独立保留并补齐清空、调用及答案类型。原 4.29 的 FHQ 增量片段整合为完整可复制版本；值、建树、插入、查询和导出都使用 `long long`，只读区间和不增加节点。
- 移除与个人 `ll` 宏冲突的类型别名。Boost 头文件读入时临时隔离并恢复 `ll / x / y / db / endl / debug` 宏，避免宏污染 Boost 内部代码。没有更换高精度实现。

### 区间、数学

- 大值域合法二元组计数不再用 `n+1` 作哨兵，可处理答案仍合法的 `n=LLONG_MAX` 边界。
- 双指针两端逼近原片段在找到答案后没有规定移动，直接照抄会无限循环；改由回调返回 0 结束、正值推进左端或负值后退右端，并写明语义。
- 实数 / 模素数高斯支持空矩阵，约定为零变量系统；不能从空矩阵推知额外变量数。
- 一般模逆归一化避免 `x%mod+mod` 溢出；扩展欧几里得的乘减先提升到 i128；CRT 的剩余差值提升到 i128，并检查正模数、互质和总模数范围。CRT 不是广义 CRT。
- Pollard-Rho 的模加先提升到 u128，修复 uint64_t 上限附近先回绕再取模的问题。BSGS / exBSGS 补齐负底数、负目标归一化。
- `factorize(1)` 返回空；0 无有限质因数分解，接口返回空以阻止递归失控，文档明确不能将其解释为 0 的分解。
- 组合数、Lucas、矩阵和模高斯的 Mint 依赖原文误指 6.5，说明更正为 6.1。

### 几何、多项式、字符串

- 保持几何原算法及整数 / 浮点两类接口。7.7 / 7.8 每次先扫描 `polygon_area2`，真实总复杂度为 O(n)，文档已纠正，扇形二分部分仍为 O(log n)。没有为复杂度标签偷偷重写算法。
- 头文件中的自由函数使用 inline；最终多翻译单元链接检查发现二维转换辅助函数 `toD(PointI)` 遗漏该标记，已补齐后复测，运算内容不变。
- 原独立多点求值 / 插值版补上 NTT 长度、非零求逆、合法除数检查；所有 NTT 仍固定 998244353、长度不超过 2^23。实际运行了恰好 2^23 长度的正逆变换测试。
- 连续点值平移先归一化大偏移量 `c`，避免 `c+j` 在取模前溢出；独立快速插值补齐负 `y` 归一化、输入长度和重复横坐标检查。拉格朗日原查询逐点求逆，复杂度更正为 O(n log MOD)。
- 后缀数组与 LCP 共享同一份维护实现，PDF 中展开；Kasai 遇最小排名时显式清零 `h`。
- AC 只接受非空模式，明确拒绝原来统计含义不完整的空模式；SAM 空串次数统一为 n+1。KMP / Z 保留原来空模式返回空列表的约定；Trie01 限制最高位 0..30。

### 动态规划

- 数位 DP 原稿终止条件 `started && /* st 合法 */` 无法编译，改为 `next / accept` 回调，保留前导零和非 tight 记忆化思路。每次调用独立清空状态，用 i128 容纳完整 `[0,LLONG_MAX]` 的数量，并避免 `l-1` 下溢。
- 多重背包将可用数量限制到 C/w，拆分倍增和包重量使用 ll，避免 int 乘法 / 左移溢出；四类背包保留不超过容量模式，并提供原稿所述恰好装满初始化。
- 分治优化没有有限候选时不再向递归传入 -1 决策位置。仍要求应用满足决策单调，有限值远离 INF。
- Li Chao 最小值版用 i128 计算跨 ll 全域中点，移除原来 ±2^62 截断，避免合法 long long 答案被改值；空树返回 LLONG_MAX。
- CDQ 原 `int ll` 与个人宏冲突，局部名改为 `left`；工作状态放入轻量 struct，逐次重置 BIT，八种取等都与 O(n²) 对拍。NEG 改为 LLONG_MIN，合法权和仍须避开该哨兵并不溢出。
- 骨牌 / 插头状态宽度增加移位范围检查；WQS 二分中点及惩罚恢复使用 i128 中间量。未改变原状态转移思路。

## 验证范围及人工复核事项

最终统一验收已全部通过；命令、范围和结果见文末。首轮发现的问题均保留修复记录，没有把失败项当成通过。

测试覆盖的是文档约束内的实现，以下内容仍需按比赛题目人工确认，不能把编译或随机对拍当成这些条件的证明：

- **待人工复核（应用条件）**：莫队 / 双指针 / 数位的具体状态与回调；分治、Knuth 的决策单调性；WQS 的离散凸性与惩罚搜索界。
- **待人工复核（数值条件）**：病态实数方程、近共线 / 近相切及极端尺度几何的绝对 EPS；有奇点或强振荡函数的辛普森误差；整数和 i128 的题目范围。并未宣称穷尽全部浮点退化输入。
- **待维护者决定**：是否在将来的独立整理任务中去除 7.7 / 7.8 重复项；本次按要求完整保留。非 ASCII 的 Duval / 最小表示法仍需确认原 char 比较顺序，文档将跨平台一致范围限定为 ASCII。
- 哈希有碰撞风险；Treap、Rho、MEC 等随机实现不提供最坏时间保证。通用模板的外部输入合法性按各节约定，不增加昂贵的全量校验。
- 没有修改 GitHub Actions、权限、发布机制；没有执行 commit、push、Tag 或 Release。PDF 和字体缓存由现有 Git 忽略规则排除。

## 排版与文件检查

`main.typ` 仅启用七条章节导入，全部全局样式与此前一致。封面、目录、字体、A4 双栏、5.7 pt 代码及背景没有重新设计；六列 CDQ 取等表使用既有跨栏页顶 figure。PDF 中所有依赖通过 read 展开，不含仓库本地 include。完整原稿说明、接口速查、调用、边界和现场记忆保留在章节中。

逐项清单如下。

### raw/04_data_structure.md

| 原条目 | 模板文件（code/data_structure/） |
| --- | --- |
| 4.1 单调队列：滑动窗口最小值 / 最大值 | `sliding_window.hpp` |
| 4.2 单调栈：左右最近严格更小元素 | `monotonic_stack.hpp` |
| 笛卡尔树 - 单调栈建树 | `cartesian_tree.hpp` |
| 4.3 ST 表：区间最大值 | `sparse_table.hpp` |
| 4.4 Fenwick 树：动态集合第 k 小 | `fenwick_kth.hpp` |
| 4.5 线性基 | `linear_basis.hpp` |
| 4.6 普通莫队 | `mo.hpp` |
| 4.7 带修莫队 | `mo_updates.hpp` |
| 4.8 只增回滚莫队 | `mo_rollback_add.hpp`, `mo_max_span.hpp` |
| 4.9 只删回滚莫队 | `mo_rollback_delete.hpp` |
| 4.10 树上莫队 | `mo_tree.hpp` |
| 4.11 二次离线莫队：二元组贡献 | `mo_second_offline.hpp` |
| 4.12 值域分块：频次 / 第 k 小 / 前驱后继 | `value_block.hpp` |
| 4.13 序列分块：区间加 / 区间和 | `sequence_block.hpp` |
| 4.14 InfoTag 线段树 | `segment_tree_lazy.hpp` |
| 4.15 Info 线段树：单点修改 | `segment_tree_point.hpp` |
| 4.16 李超线段树：整数最大值 | `li_chao_integer.hpp` |
| 4.17 李超线段树：浮点系数最大值（整数横坐标） | `li_chao_double.hpp` |
| 4.18 栈维护凸包最值 | `line_hull.hpp` |
| 4.19 动态开点线段树：区间赋值 + 剩余数量 | `dynamic_segment_tree.hpp` |
| 4.20 线段树合并：点计数 + 区间最值 | `segment_tree_merge.hpp` |
| 主席树：Info 单点修改 | `persistent_segment_tree_point.hpp` |
| 主席树：InfoTag 区间修改 | `persistent_segment_tree_lazy.hpp` |
| 主席树：值域计数 | `persistent_segment_tree_count.hpp` |
| 主席树：区间不同颜色数量 | `persistent_distinct.hpp` |
| 主席树：区间 mex | `persistent_mex.hpp` |
| 主席树：树上第 k 小 | `persistent_tree_kth.hpp` |
| 4.23 可持久化 01 Trie | `persistent_trie.hpp` |
| 4.24 前缀线性基：区间异或最大值 | `prefix_linear_basis.hpp` |
| 4.25 FHQ Treap：按值平衡树 | `fhq_set.hpp` |
| 4.26 FHQ Treap：隐式序列 | `fhq_sequence.hpp` |
| 4.27 可持久化 FHQ Treap：按值平衡树 | `persistent_fhq_set.hpp` |
| 4.28 可持久化 FHQ Treap：隐式序列 | `persistent_fhq_sequence.hpp` |
| 4.29 可持久化隐式 FHQ：区间和扩展 | `persistent_fhq_sum.hpp` |
| 4.30 树状数组：动态第 k 小 / 前 k 小之和 | `fenwick_k_sum.hpp` |
| 4.31 双 multiset 维护前 k 小之和 | `multiset_k_sum.hpp` |
| 4.32 set 维护不相交区间集合 | `interval_union.hpp` |
| 4.33 set 维护不相交区间集合 | `interval_ids.hpp` |
| 4.34 高精度整数 `cpp_int` | `big_integer.hpp` |

### raw/05_interval_greedy.md

| 原条目 | 模板文件（code/interval_greedy/） |
| --- | --- |
| 5.1 区间集合合法二元组计数 | `count_pairs_small.hpp`, `count_pairs_large.hpp` |
| 5.2 区间最小点覆盖 / 最大不相交区间数 | `min_points.hpp`, `max_disjoint.hpp` |
| 5.3 区间分组 | `interval_group.hpp` |
| 5.4 区间覆盖目标区间 | `min_cover.hpp` |
| 5.5 双指针模板 | `window_longest.hpp`, `window_shortest.hpp`, `two_ends.hpp` |

### raw/06_math.md

| 原条目 | 模板文件（code/math/） |
| --- | --- |
| 6.1 ModInt / 快速幂 / 逆元 / 扩展欧几里得 | `modint.hpp`, `qpow.hpp`, `extended_gcd.hpp`, `inverse_mod.hpp` |
| 6.2 组合数预处理：阶乘 / 逆阶乘 | `combination.hpp` |
| 6.3 Lucas 定理 | `lucas.hpp` |
| 6.4 整除分块 | `division_blocks.hpp` |
| 6.5 线性筛 | `linear_sieve.hpp` |
| 6.6 欧拉函数 / 莫比乌斯函数：线性筛 | `multi_sieve.hpp` |
| 6.7 莫比乌斯反演：互质对数 | `mobius_pairs.hpp` |
| 6.8 矩阵快速幂 | `matrix_power.hpp` |
| 6.9 高斯消元：实数线性方程组 | `gauss_double.hpp` |
| 6.10 高斯消元：模素数线性方程组 | `gauss_mod.hpp` |
| 6.11 容斥原理：子集枚举 | `subset_ie.hpp`, `div_any.hpp` |
| 6.12 自适应辛普森积分 | `adaptive_simpson.hpp` |
| 6.13 CRT 中国剩余定理 | `crt.hpp` |
| 6.14 Miller-Rabin 素性测试 | `miller_rabin.hpp` |
| 6.15 Pollard-Rho 大整数分解 | `pollard_rho.hpp` |
| 6.16 BSGS / exBSGS 离散对数 | `bsgs.hpp` |
| 6.17 floor_sum | `floor_sum.hpp` |
| 6.18 Cipolla / Tonelli-Shanks 模平方根 | `mod_sqrt.hpp` |

### raw/07_geometry.md

| 原条目 | 模板文件（code/geometry/） |
| --- | --- |
| 7.1 二维几何基础 | `geometry_base.hpp` |
| 7.2 直线与线段 | `line_segment.hpp` |
| 7.3 三角形常用工具 | `triangle.hpp` |
| 7.4 多边形基础 | `polygon.hpp` |
| 7.5 格点多边形：Pick 定理 | `pick.hpp` |
| 7.6 二维凸包 Andrew | `convex_hull.hpp` |
| 7.7 凸多边形点包含 O(log n) | `convex_contains.hpp` |
| 7.8 凸多边形点包含 O(log n) | `convex_contains_variant.hpp` |
| 7.9 旋转卡壳：凸包直径 | `convex_diameter.hpp` |
| 7.10 旋转卡壳：最小面积外接矩形 | `bounding_rectangle.hpp` |
| 7.11 最近点对 | `closest_pair.hpp` |
| 7.12 半平面交 | `halfplane_intersection.hpp` |
| 7.13 圆基础 | `circle.hpp` |
| 7.14 圆的切线 | `circle_tangents.hpp` |
| 7.15 最小圆覆盖 | `minimum_enclosing_circle.hpp` |
| 7.16 Minkowski Sum | `minkowski_sum.hpp` |
| 7.17 Convex Hull Trick：单调直线凸包 | `monotone_cht.hpp` |
| 7.18 三维几何基础 | `geometry_3d.hpp` |

### raw/08_Poly.md

| 原条目 | 模板文件（code/poly/） |
| --- | --- |
| 8.1 多项式核心 - NTT / Poly | `poly_core.hpp` |
| 8.2 形式幂级数 FPS - 逆 / ln / exp / 幂 / sqrt / 三角函数 | `fps.hpp` |
| 8.3 多项式除法 / 多点求值 / 插值 | `division_evaluation.hpp` |
| 8.4 多项式平移 / Chirp-Z / 连续点值平移 | `taylor_shift.hpp`, `chirp_z.hpp`, `consecutive_shift.hpp` |
| 8.5 位运算卷积 - FWT XOR / OR / AND | `fwt_xor.hpp`, `fwt_or.hpp`, `fwt_and.hpp` |
| 8.6 线性递推 - Bostan-Mori / Berlekamp-Massey | `bostan_mori.hpp`, `linear_recurrence.hpp`, `berlekamp_massey.hpp` |
| 8.7 多项式复合 / 拉格朗日反演 | `composition.hpp` |
| 8.8 生成函数 OGF / EGF | `ogf.hpp`, `egf.hpp` |
| 8.9 多项式多点求值 | `multipoint.hpp` |
| 8.10 多项式快速插值 | `fast_interpolation.hpp` |
| 8.11 拉格朗日插值 | `lagrange.hpp` |

### raw/09_string.md

| 原条目 | 模板文件（code/string/） |
| --- | --- |
| 9.1 KMP / Prefix Function | `kmp.hpp` |
| 9.2 Z 函数 | `z_function.hpp` |
| 9.3 Manacher：回文半径 | `manacher.hpp` |
| 9.4 字符串哈希：双模数 | `hash_double.hpp` |
| 9.5 Trie：字符串插入 / 前缀计数 | `trie.hpp` |
| 9.6 01 Trie：最大异或 / 最小异或 | `trie01.hpp` |
| 9.7 AC 自动机：多模式匹配 | `aho_corasick.hpp` |
| 9.8 回文自动机 PAM | `palindromic_tree.hpp` |
| 9.9 后缀自动机 SAM | `suffix_automaton.hpp` |
| 9.10 后缀数组 SA：O(n log n) | `suffix_array.hpp` |
| 9.11 LCP / Height + RMQ | `lcp_rmq.hpp` |
| 9.12 Duval：Lyndon 分解 | `duval.hpp` |
| 9.13 字符串最小表示法 | `minimum_rotation.hpp` |

### raw/10_dp.md

| 原条目 | 模板文件（code/dp/） |
| --- | --- |
| 10.1 0/1、完全、多重、分组背包 | `knapsack01.hpp`, `knapsack_complete.hpp`, `knapsack_bounded.hpp`, `knapsack_group.hpp` |
| 10.2 区间 DP（石子合并骨架） | `interval_merge.hpp` |
| 10.3 树形 DP（最大权独立集骨架） | `tree_independent_set.hpp` |
| 10.4 换根 DP（全点距离和） | `reroot_distance.hpp` |
| 10.5 数位 DP（记忆化通用骨架） | `digit_dp.hpp` |
| 10.6 状压 DP（Hamilton 路径 / TSP 骨架） | `hamilton_path.hpp` |
| 10.7 轮廓线 DP（多米诺骨牌铺满） | `domino_tilings.hpp` |
| 10.8 插头 DP（网格 Hamilton 单回路） | `plug_hamilton_cycle.hpp` |
| 10.9 SOS DP（子集 / 超集和） | `sos_dp.hpp` |
| 10.10 单调队列优化 DP | `monotone_queue_dp.hpp` |
| 10.11 斜率优化 CHT | `cht_min.hpp` |
| 10.12 分治优化 DP | `divide_conquer_dp.hpp` |
| 10.13 Knuth 优化（区间 DP） | `knuth_merge.hpp` |
| 10.14 Li Chao Tree 优化 DP | `li_chao_min.hpp` |
| 10.15 Bitset 优化 DP（0/1 子集和） | `bitset_subset_sum.hpp` |
| 10.16 WQS 二分 / Aliens Trick | `wqs.hpp` |
| CDQ 优化三维偏序 DP | `cdq_dominance.hpp` |

每份模板对应 `test/<topic>/<algorithm>_test.cpp`；各章包含 `run_tests.py`。共享入口为 `test/run_all.py`、`test/check_headers.py`、`test/run_chapter.py`、`test/check_examples.py`、`test/check_manifest.py`，另有 `test/migration_manifest.json`。章节为 `sections/04_data_structure.typ` 至 `sections/10_dp.typ`。维护说明变更为 `AGENTS.md`、`README.md`、`.gitignore` 与本记录。

## 最终验收结果

- `python test/run_all.py` 完整执行结束，退出码 0：**PASS ALL 10 CHAPTERS**。
- 项目全部 196 份模板通过各章独立编译；图论、树论及新增章节通过个人基础模板兼容检查，工具章通过其原有提交 / LOCAL 两种模式。新增 140 份头文件另通过多翻译单元链接；新增 140 个对应运行测试单元全部通过。前三章原测试、样例与多文件链接全部通过。
- 新增章节在 PDF 中展开依赖后的 140 份调用示例全部编译并运行：**PASS ALL 140 EXPANDED PDF EXAMPLES**；前三章原有 56 份示例也通过原测试。
- 实际覆盖 NTT 长度 2^23 的正逆变换、八种 CDQ 取等、合并线段树删除至空、半平面交与独立裁剪对拍、重复初始化和历史版本不变性等边界。
- 核对全部十份原稿：前三章 10 / 34 / 9 个原始条目对应既有 10 / 37 / 9 个 PDF 条目（原图论已拆 Floyd / 闭包及三种带权 DSU）；新增七章 121 个原始条目均有清单映射，全部模板均有测试和 PDF 源码引用，未发现遗漏。
- Typst **0.15.1** 编译完整十章 PDF：**194 页**、187 个目录 / 书签条目。487 个块状代码与现场记忆内容均能在 PDF 文本中完整匹配；未检测到代码越栏、本地仓库 include、替代字符或 NUL。全页渲染总览及封面、目录、各章正文、长代码、跨栏表和末页均已检查。没有通过裁切、缩小字体或额外强制分页处理溢出。
- 封面与迁移前基准像素一致；前三章正文文本完整一致。固定的三份中文字体均通过原脚本 SHA-256 校验。
- 迁移前保护的 85 个既有文件（包含全部 raw 及前三章代码、章节和测试）SHA-256 全部未变。`git diff --check` 通过，301 个新增文本文件的行尾空白检查通过。
- 最终 Git：`main...origin/main`；4 个已有文件修改（`.gitignore`、`AGENTS.md`、`README.md`、`main.typ`），301 个新增文件未跟踪，没有已暂存项、删除项或 PDF 跟踪项。未执行 commit、push、Tag、Release 或远程变更。

本机验证使用 GCC 15.1.0、C++17、断言、标准库边界检查及运行测试的 `-ftrapv`。Boost.Multiprecision 使用现有本机头文件搜索路径。Windows 临时进程启动曾出现瞬时拒绝访问 / CreateProcess 失败，测试工具增加有限重试并降低并发；最终完整验收重新运行成功，没有跳过这些测试。
