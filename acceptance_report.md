# ICPC Notebook 十章最终验收报告

> 历史快照：本文保留自动编号机制引入前的验收结论，正文中的“暂缓提交”和“编号不一致”不是当前结论。后续用户已确认按实际位置自动编号；编号成因、最新全量测试、未跟踪文件逐项分类及提交建议以 [final_acceptance_report.md](final_acceptance_report.md) 为准。机制说明另见 [numbering_report.md](numbering_report.md)。本文的历史编号、页数、哈希和 Git 数量仅代表当时状态。

本报告审查当前本地工作区，包含此前第 4～10 章尚未提交的迁移成果。未增加算法，未重排编号，未大规模重构；只修复本次复现的一个算术边界错误和确定的展示 / 说明遗漏。

**结论：技术构建与现有测试通过，但原稿编号一致性尚未全部通过。建议先确定并处理图论编号口径，再作为“最终验收完成”的版本提交 main。当前不建议直接提交为最终定稿。** 本轮没有 commit、push、Tag、Release 或远程权限修改。

## 已通过

### 内容、模板数量与页数

逐章读取十份原稿，核对二级条目、实现及重要变体、接口 / 调用 / 现场记忆，并建立原稿 → 章节 → 维护头文件对应。没有发现整个模板或主要实现变体缺失；审查中发现若干辅助调用片段被漏掉，已补回，见“本轮修复”。完整清单在报告后半部分，每份头文件恰好计数一次。

“算法条目”按原稿二级标题计数；“模板文件”按 `.hpp` 计数，不把 struct 的每个成员函数或重复展开的依赖另算一个模板。

| 章 | 章节 | 原稿条目 | 模板文件 | 运行测试单元 | PDF 物理页范围 | 涉及页数 |
| --- | --- | ---: | ---: | ---: | --- | ---: |
| 01 | 基础工具 | 10 | 10 | 1 | 3–6 | 4 |
| 02 | 图论 | 34 | 37 | 4 | 6–28 | 23 |
| 03 | 树论 | 9 | 9 | 4 | 28–35 | 8 |
| 04 | 数据结构 | 39 | 40 | 40 | 35–64 | 30 |
| 05 | 区间贪心 / 双指针 | 5 | 9 | 9 | 64–66 | 3 |
| 06 | 数学 | 18 | 22 | 22 | 66–77 | 12 |
| 07 | 计算几何 | 18 | 18 | 18 | 77–130 | 54 |
| 08 | 多项式 | 11 | 18 | 18 | 130–178 | 49 |
| 09 | 字符串 | 13 | 13 | 13 | 178–186 | 9 |
| 10 | 动态规划 | 17 | 20 | 20 | 186–195 | 10 |
| 合计 | 十章 | 174 | 196 | 149 | 正文 3–195 | 193 个不同正文页 |

上述范围包含章节在过渡页所占的部分；相邻章共用过渡页，涉及页数不能直接相加。封面为物理第 1 页、目录为第 2 页；封面不编号，目录页脚为 1，正文页脚等于物理页减 1。PDF 共 **195 页**。

章节顺序保持 1→10。第 4 章原稿缺少 4.21 / 4.22，继续保留缺号；笛卡尔树、六种主席树及章末 CDQ 仍无编号。7.7 / 7.8 原稿重复实现均保留。6.18 原标题提及 Cipolla，但原稿实际只有 Tonelli-Shanks，没有虚构另一份实现。图论原稿与成品编号差异单列于“必须修复”。

### C++ 与测试

- 重新完整执行 `python test/run_all.py`，退出码 **0**，日志以 `PASS ALL 10 CHAPTERS` 结束；十章均通过，没有跳过失败章节。
- 共 196 份头文件通过 C++17 独立编译。工具章原运行器使用 GNU++17，另补做了其全部 10 份头文件的显式 `-std=c++17` 编译。其余章节运行器含 `-std=c++17` 检查；图论 / 树论还有 GNU++17 个人基础模板模式。
- 各章重复包含与多翻译单元链接检查通过；第 4～10 章还检查个人基础模板的宏兼容。使用 GCC 15.1.0、`-O2`、断言、标准库断言和运行测试的 `-ftrapv`。`bits/stdc++.h`、`__int128` 属于文档已声明的 GNU 工具链依赖；高精度另依赖 Boost.Multiprecision 头文件。
- 149 个运行测试单元覆盖基础、边界、重复初始化及适用的随机 / 穷举参考。每章结果见上表。
- 196 份最小调用示例均通过编译，其中图论 37、树论 9、新七章 140，共 **186 份实际运行**；工具章 10 份原最小示例只做语法编译，工具行为由 `tools_test.cpp` 的提交 / LOCAL 两种模式运行覆盖，不能把这 10 份示例称为已单独运行。
- 本次补回的 9 段 C++ 指导片段在 5 个独立上下文中编译并运行断言，包括普通 / 懒标记主席树的两种初始化、值域前缀构建与查询、树上路径第 k 小、PAM 逐字符维护。CDQ 三个布尔参数的八种配置由既有随机对拍覆盖。
- 另从最终 PDF 的实际提取文本取出 LCT、势能费用流、floor_sum、CDQ、复合 / 复合逆、快速插值六个完整源码块，补上对应最小示例后，C++17 编译和运行均通过。不是只重新包含仓库头文件。

### 复杂算法的实测证据

| 章节 / 范围 | 已有参考与边界 | 证据的适用范围 |
| --- | --- | --- |
| 图论流、割与匹配 | 最大流对穷举割；费用流对小容量边流枚举；Gomory-Hu 对所有点对穷举割；匹配 / 赋值及上下界流对小图枚举 | 小图正确性及重复调用；费用流随机图以无负环的 DAG 为主，不能证明任意负环输入可用 |
| LCT、HLD、LCA、虚树、点分治 | LCT 65 组小森林各 2400 次随机操作对 BFS 路径；4000 点链边界；其他树模板对朴素遍历 | 不等于已验证比赛最大规模递归栈与时间 |
| 持久化结构、FHQ、莫队 | 历史版本与朴素容器、区间暴力对照；重复初始化；合并线段树删除至空 | 通用回调只验证测试中的维护逻辑，不覆盖任何题目的自定义状态 |
| 高斯 / 数论 | 实数消元 300 组已知解的对角占优系统；模消元小域穷举；BSGS / exBSGS 在模数 1..100 穷举；Rho 验证质因子与乘积，包括 uint64 上界附近 | 实数病态矩阵和随机算法最坏耗时仍有限制 |
| 半平面交 / MEC / 旋转卡壳 | HPI 1000 组矩形，另 1000 组随机半平面与独立逐边裁剪对照；MEC 500 组小点集枚举候选圆；最近 / 最远点对、矩形对暴力候选 | 浮点退化 / 极端尺度未穷尽；部分圆构造原语由测试与实现共用 |
| NTT / FPS / 求值插值 | 朴素卷积对照，随机正逆变换，实际运行恰好 2^23 长度；FPS 约 150 组长度 <=50 的系数递推与恒等式；独立插值约 200 组长度 <80 对 Horner | 最大 NTT 容量通过不代表所有 FPS 功能的最大规模都完成压力测试 |
| 字符串 | KMP / Z 对朴素匹配，SA 对后缀排序，SAM / PAM 对全子串统计，AC 对模式计数，重复 calc_occ 检查 | 字符集 / 空串约定按各节；哈希碰撞没有被测试消除 |
| CDQ / 插头 / 优化 DP | CDQ 八种取等各 150 组 n<50 对 O(n²)；插头 400 组至多 3×4 网格对 Hamilton 回路枚举；背包 / 铺砖对暴力；WQS / 分治对普通 DP；Knuth 对小规模合并枚举 | 不能代替单调性、凸性证明，也不能证明大规模时间 / 内存上界可承受 |

### PDF 与排版

- Typst **0.15.1** 重新编译完整十章，保留既有 A4、页边距、双栏、栏距、封面、目录、字号和代码背景。`main.typ` 本轮未调整全局样式。
- PDF 含 **187 个目录 / 书签条目**：10 个章节、177 个算法标题 / 原稿补项。196 是文件数量，两者因一节多变体而不同。标题可通过目录、书签和 PDF 文本检索定位；编号差异见下文。
- 固定的 Noto Serif CJK SC Regular / Bold 与 LXGW WenKai 三个字体文件均通过现有脚本 SHA-256 校验；本次关闭系统字体搜索，字体版本、Typst 版本与工作流配置相同。代码保持 5.7pt，正文 7.4pt，未缩字塞页。
- 对全部页面生成渲染总览，查看封面、完整目录、各章起始、长代码、补回调用处、跨栏表及末页；并用 Poppler 渲染末页区域复核。未发现正文空白页、乱码方块或代码越栏。封面与迁移前基准像素一致；前三章完整正文提取文本与基准一致。
- 对全部 **496 个块状源码 / 现场记忆**逐一匹配 PDF 提取文本；没有缺失块。检查全部 5.7pt 字体的文本坐标（含代码中文回退字体），未检测到越过栏界或右页边界；提取文本没有 U+FFFD 或 NUL。
- 代码中没有仓库本地 `#include "..."`：依赖通过 `read()` 展开。标准库 / Boost 的系统 include 保留；公共依赖组合复制时只保留一次，同名独立版本按题选择。
- 有正文末页自然留白，最终页只占左栏上部；没有额外空白页或算法强制分页。恢复遗漏说明后由 194 页增为 195 页，此处没有靠删内容、缩字或强制分页压回原页数。
- 当前 PDF SHA-256：`74afbb9e344bf2321cdf80259110507be183ee8fa8eb274e443ec5c1e2355032`。

## 需要人工复核

以下不是“编译失败”，也未作为“所有场景算法正确”宣布通过。

| 模板 / 范围 | 仍须复核什么 |
| --- | --- |
| `mo*.hpp`、双指针回调、`digit_dp.hpp` | 实际题目的状态充分性、add / del / undo 对称性、前导零与计数范围；莫队许多测试维护区间和，不能证明所有复杂回调 |
| `divide_conquer_dp.hpp`、`knuth_merge.hpp`、`wqs.hpp` | 决策单调、非负合并条件、离散凸性、惩罚界覆盖最优转折。当前随机测试仅覆盖具有这些性质的实例 |
| `gauss_double.hpp`、`adaptive_simpson.hpp` | 病态系统、绝对 EPS 与尺度；积分奇点 / 强振荡，达到递归上限不保证误差已达标 |
| 浮点几何，尤其 `circle_tangents.hpp`、`geometry_3d.hpp` | 近相切 / 近平行 / 近共线、极端尺度；随机切线测试主要检查返回线合法，未独立穷举证明所有输入的切线数量完整；三维主要验证旋转不变量及少数交点样例 |
| `fps.hpp`、`composition.hpp` 及三个求值 / 插值实现 | 全部大规模 FPS 分支、时间 / 内存压力；互逆恒等式不是独立数学证明。按模数、常数项和截断长度前提使用 |
| 递归图 / 树 / FHQ 模板 | 目标编译器的实际栈限、深链和最大规模内存；不能由小样例和 4000 点链推出 2e5 深链安全 |
| 大整数运算、费用 / 路径和、几何整数叉积、DP 计数 | 按题确认全部中间量与哨兵范围；`__int128` 仍有限。断言、`-ftrapv` 和标准库检查不是完整 ASan / UBSan 验证，本轮未运行全库 sanitizer |
| Hash2、随机 Treap / Rho / MEC | 碰撞概率、随机最坏耗时；Duval / 最小表示法的跨平台字符顺序按文档限定 ASCII |
| 6.18 模平方根、7.7 / 7.8 重复项 | 6.18 的 Cipolla 仅在原标题出现，实际使用 Tonelli-Shanks；两份点包含模板是原稿重复，不应误以为不同算法 |
| 比赛查阅与打印 | 标题 / 接口 / 调用结构具备，但未做真实队员“30 秒理解”计时。建议队员按 LCT、FPS、插头 DP、莫队各做一次定时查阅和 100% 比例试印 |
| 成品厚度与重复依赖 | 几何涉及 54 页、多项式涉及 49 页；独立可复制块重复展开基础依赖，有明显体积成本。为保留结构和可复制性，本轮未跨章节抽库或删模板；实际比赛允许携带页数需由队伍确认 |
| Ubuntu / GitHub Actions | 本轮实际执行环境为 Windows GCC / Typst；没有触发远程工作流或做 Ubuntu 实测。相同固定字体与编译器版本是配置检查结论，不是已完成跨平台 PDF 字节一致验证 |

接口速查、最小调用和现场记忆已有统一位置；复杂莫队部分仍使用一段通用回调说明，初次读者需要结合函数签名。模板多为轻量 struct 或普通函数，没有发现必须大规模工程化重构的理由。少量重复标准 include / using 与多份同名模板属于可独立复制的维护结果，本轮保留。

## 必须修复

### 尚未关闭：图论编号与原稿不一致

内容顺序保留，但图论在迁移时将一节拆成多个二级标题，自动编号发生偏移：

- 原 2.1～2.8 → PDF 同号。
- 原 2.9 Floyd / 传递闭包 → PDF 2.9 与 2.10。
- 原 2.10～2.15 → PDF 2.11～2.16。
- 原 2.16 带权 DSU → PDF 2.17 / 2.18 / 2.19。
- 原 2.17～2.34 → PDF 2.20～2.37，例如原 2.34 最大权闭合子图现在为 2.37。

因此“全部原稿算法编号保留”这一验收项 **不通过**。各节已有“来源：原文 2.x”，可追溯，但不能把来源注记等同于标题编号一致。此差异在前两章既有文件中已存在，不是本轮新增。依据维护规则保护现有编号，本轮只记录，没有自行重编号。最终定稿前须确定统一口径并处理；这是本报告暂缓建议提交最终版本的具体原因。

### 本轮已修复并复验

1. **floor_sum 算术边界**：`n=LLONG_MAX,m=1,a=3,b=0` 的正确答案能放入 i128，原 `(n-1)*n*(a/m)/2` 却在除法前溢出；独立复现返回错误结果。改为先计算三角数再乘商，数学思路不变。在 `test/math/floor_sum_test.cpp` 增加正负两组边界断言，相关测试和全部项目测试通过。原文不变，其他可能超过 i128 的输入仍受文档范围约束。
2. **几何复杂度标题**：7.7 / 7.8 原标题 O(log n) 与每次线性面积判退化不符，标题改为 O(n)、含退化检查；来源字段保留原文，编号及算法代码不变。
3. **辅助调用片段遗漏**：补回两种主席树的已有数组 / 单位元初始化，值域主席树离散化、前缀版本和 <=x 查询，树上第 k 小的 DFS 版本和路径查询，PAM 逐字符维护；补齐莫队快照撤销说明及 CDQ 取等 / 最长链指导。未复制一份新的算法实现，源码继续统一从 `code/` 读取。

本轮实际修改：`code/math/floor_sum.hpp`、`test/math/floor_sum_test.cpp`、`sections/04_data_structure.typ`、`sections/07_geometry.typ`、`sections/09_string.typ`、`sections/10_dp.typ`；新增本报告。其余工作区改动是本轮开始前已存在的迁移成果。未修改 raw、前三章源码 / 章节 / 测试、维护样式、Actions 或发布机制。

## 完整算法清单

表中保留原稿标题及顺序；每个文件均位于对应 `code/<topic>/`。这是算法及维护文件清单，不把示例变量、内部 helper 或重复依赖另算算法。图论额外列当前 PDF 编号，其余按前述原稿编号及无编号补项规则展示。

### 01 基础工具：10 个原条目，10 份模板

原稿：`raw/01_tools.md`；实现目录：`code/basic/`。

| 原稿条目 | 模板文件 |
| --- | --- |
| 1.1 GCD | `gcd.hpp` |
| 1.2 位运算枚举 | `bit_enumeration.hpp` |
| 1.3 快读 | `fast_io.hpp` |
| 1.4 取模类 Mint | `mint.hpp` |
| 1.5 STL 二分 | `binary_search.hpp` |
| 1.6 对拍 | `stress.hpp` |
| 1.7 __int128 输入输出 | `int128_io.hpp` |
| 1.8 随机数 RNG | `rng.hpp` |
| 1.9 Custom Hash / 防 Hack | `custom_hash.hpp` |
| 1.10 Debug 宏 | `debug.hpp` |

### 02 图论：34 个原条目，37 份模板

原稿：`raw/02_graph.md`；实现目录：`code/graph/`。

| 原稿条目 | 模板文件 / 当前 PDF 编号 |
| --- | --- |
| 2.1 强连通分量 SCC - Tarjan + 缩点 | `scc.hpp`（PDF 2.1：强连通分量 SCC - Tarjan + 缩点） |
| 2.2 割点 / 桥 | `cut_bridge.hpp`（PDF 2.2：割点 / 桥） |
| 2.3 边双连通分量 EBCC | `ebcc.hpp`（PDF 2.3：边双连通分量 EBCC） |
| 2.4 点双连通分量 VBCC / 圆方树 | `vbcc.hpp`（PDF 2.4：点双连通分量 VBCC / 圆方树） |
| 2.5 2-SAT | `two_sat.hpp`（PDF 2.5：2-SAT） |
| 2.6 链式前向星 Vector 版 | `forward_star.hpp`（PDF 2.6：链式前向星 Vector 版） |
| 2.7 Dijkstra 最短路 | `dijkstra.hpp`（PDF 2.7：Dijkstra 最短路） |
| 2.8 SPFA 判负环 | `negative_cycle.hpp`（PDF 2.8：SPFA 判负环） |
| 2.9 Floyd：全源最短路 / 传递闭包 | `floyd.hpp`（PDF 2.9：Floyd 全源最短路）、`transitive_closure.hpp`（PDF 2.10：bitset 传递闭包） |
| 2.10 差分约束 | `difference_constraints.hpp`（PDF 2.11：差分约束） |
| 2.11 Kruskal 最小生成树 | `kruskal.hpp`（PDF 2.12：Kruskal 最小生成树） |
| 2.12 Prim 最小生成树 | `prim.hpp`（PDF 2.13：Prim 最小生成树） |
| 2.13 并查集 DSU | `dsu.hpp`（PDF 2.14：并查集 DSU） |
| 2.14 可撤销并查集 Rollback DSU | `rollback_dsu.hpp`（PDF 2.15：可撤销并查集 Rollback DSU） |
| 2.15 可持久化并查集 Persistent DSU | `persistent_dsu.hpp`（PDF 2.16：可持久化并查集 Persistent DSU） |
| 2.16 带权并查集 | `weighted_dsu.hpp`（PDF 2.17：加法差分带权并查集）、`xor_dsu.hpp`（PDF 2.18：XOR / 奇偶带权并查集）、`mod_dsu.hpp`（PDF 2.19：模 k 带权并查集） |
| 2.17 拓展域并查集 | `extended_domain_dsu.hpp`（PDF 2.20：拓展域并查集） |
| 2.18 并查集虚点 / 参考点 | `dsu_reference.hpp`（PDF 2.21：并查集虚点 / 参考点） |
| 2.19 拓扑排序 | `topological_sort.hpp`（PDF 2.22：拓扑排序） |
| 2.20 欧拉路径 / 欧拉回路：有向图 | `euler_directed.hpp`（PDF 2.23：欧拉路径 / 欧拉回路：有向图） |
| 2.21 欧拉路径 / 欧拉回路：无向图 | `euler_undirected.hpp`（PDF 2.24：欧拉路径 / 欧拉回路：无向图） |
| 2.22 线段树优化建图 | `segment_graph.hpp`（PDF 2.25：线段树优化建图） |
| 2.23 倍增优化树上路径建图 | `jump_graph.hpp`（PDF 2.26：倍增优化树上路径建图） |
| 2.24 最大流 - Dinic | `dinic.hpp`（PDF 2.27：最大流 - Dinic） |
| 2.25 最小费用最大流 - SPFA | `min_cost_flow_spfa.hpp`（PDF 2.28：最小费用最大流 - SPFA） |
| 2.26 最小费用最大流 - 势能 Dijkstra / Johnson | `min_cost_flow_potential.hpp`（PDF 2.29：最小费用最大流 - 势能 Dijkstra / Johnson） |
| 2.27 最小割树 - Gomory-Hu | `gomory_hu.hpp`（PDF 2.30：最小割树 - Gomory-Hu） |
| 2.28 全局最小割 - Stoer-Wagner | `stoer_wagner.hpp`（PDF 2.31：全局最小割 - Stoer-Wagner） |
| 2.29 0-1 BFS | `zero_one_bfs.hpp`（PDF 2.32：0-1 BFS） |
| 2.30 二分图最大匹配 Hopcroft-Karp | `hopcroft_karp.hpp`（PDF 2.33：二分图最大匹配 Hopcroft-Karp） |
| 2.31 KM / Hungarian Assignment | `hungarian.hpp`（PDF 2.34：KM / Hungarian Assignment） |
| 2.32 上下界可行流 | `lower_bound_flow.hpp`（PDF 2.35：上下界可行流） |
| 2.33 上下界最大流 | `lower_bound_max_flow.hpp`（PDF 2.36：上下界最大流） |
| 2.34 最大权闭合子图 | `max_weight_closure.hpp`（PDF 2.37：最大权闭合子图） |

### 03 树论：9 个原条目，9 份模板

原稿：`raw/03_tree.md`；实现目录：`code/tree/`。

| 原稿条目 | 模板文件 |
| --- | --- |
| 3.1 倍增 LCA | `lca.hpp` |
| 3.2 树剖 LCA | `lca_hld.hpp` |
| 3.3 Tarjan 离线 LCA | `lca_tarjan.hpp` |
| 3.4 重链剖分 HLD | `hld.hpp` |
| 3.5 虚树 | `virtual_tree.hpp` |
| 3.6 启发式合并 / DSU on Tree：子树不同颜色数 | `dsu_on_tree.hpp` |
| 3.7 点分治：统计距离不超过 k 的点对数 | `centroid.hpp` |
| 3.8 Kruskal 重构树 | `kruskal_tree.hpp` |
| 3.9 LCT：动态森林 / 路径加 / 路径和 | `link_cut_tree.hpp` |

### 04 数据结构：39 个原条目，40 份模板

原稿：`raw/04_data_structure.md`；实现目录：`code/data_structure/`。

| 原稿条目 | 模板文件 |
| --- | --- |
| 4.1 单调队列：滑动窗口最小值 / 最大值 | `sliding_window.hpp` |
| 4.2 单调栈：左右最近严格更小元素 | `monotonic_stack.hpp` |
| 笛卡尔树 - 单调栈建树 | `cartesian_tree.hpp` |
| 4.3 ST 表：区间最大值 | `sparse_table.hpp` |
| 4.4 Fenwick 树：动态集合第 k 小 | `fenwick_kth.hpp` |
| 4.5 线性基 | `linear_basis.hpp` |
| 4.6 普通莫队 | `mo.hpp` |
| 4.7 带修莫队 | `mo_updates.hpp` |
| 4.8 只增回滚莫队 | `mo_rollback_add.hpp`、`mo_max_span.hpp` |
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

### 05 区间贪心 / 双指针：5 个原条目，9 份模板

原稿：`raw/05_interval_greedy.md`；实现目录：`code/interval_greedy/`。

| 原稿条目 | 模板文件 |
| --- | --- |
| 5.1 区间集合合法二元组计数 | `count_pairs_small.hpp`、`count_pairs_large.hpp` |
| 5.2 区间最小点覆盖 / 最大不相交区间数 | `min_points.hpp`、`max_disjoint.hpp` |
| 5.3 区间分组 | `interval_group.hpp` |
| 5.4 区间覆盖目标区间 | `min_cover.hpp` |
| 5.5 双指针模板 | `window_longest.hpp`、`window_shortest.hpp`、`two_ends.hpp` |

### 06 数学：18 个原条目，22 份模板

原稿：`raw/06_math.md`；实现目录：`code/math/`。

| 原稿条目 | 模板文件 |
| --- | --- |
| 6.1 ModInt / 快速幂 / 逆元 / 扩展欧几里得 | `modint.hpp`、`qpow.hpp`、`extended_gcd.hpp`、`inverse_mod.hpp` |
| 6.2 组合数预处理：阶乘 / 逆阶乘 | `combination.hpp` |
| 6.3 Lucas 定理 | `lucas.hpp` |
| 6.4 整除分块 | `division_blocks.hpp` |
| 6.5 线性筛 | `linear_sieve.hpp` |
| 6.6 欧拉函数 / 莫比乌斯函数：线性筛 | `multi_sieve.hpp` |
| 6.7 莫比乌斯反演：互质对数 | `mobius_pairs.hpp` |
| 6.8 矩阵快速幂 | `matrix_power.hpp` |
| 6.9 高斯消元：实数线性方程组 | `gauss_double.hpp` |
| 6.10 高斯消元：模素数线性方程组 | `gauss_mod.hpp` |
| 6.11 容斥原理：子集枚举 | `subset_ie.hpp`、`div_any.hpp` |
| 6.12 自适应辛普森积分 | `adaptive_simpson.hpp` |
| 6.13 CRT 中国剩余定理 | `crt.hpp` |
| 6.14 Miller-Rabin 素性测试 | `miller_rabin.hpp` |
| 6.15 Pollard-Rho 大整数分解 | `pollard_rho.hpp` |
| 6.16 BSGS / exBSGS 离散对数 | `bsgs.hpp` |
| 6.17 floor_sum | `floor_sum.hpp` |
| 6.18 Cipolla / Tonelli-Shanks 模平方根 | `mod_sqrt.hpp` |

### 07 计算几何：18 个原条目，18 份模板

原稿：`raw/07_geometry.md`；实现目录：`code/geometry/`。

| 原稿条目 | 模板文件 |
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

### 08 多项式：11 个原条目，18 份模板

原稿：`raw/08_Poly.md`；实现目录：`code/poly/`。

| 原稿条目 | 模板文件 |
| --- | --- |
| 8.1 多项式核心 - NTT / Poly | `poly_core.hpp` |
| 8.2 形式幂级数 FPS - 逆 / ln / exp / 幂 / sqrt / 三角函数 | `fps.hpp` |
| 8.3 多项式除法 / 多点求值 / 插值 | `division_evaluation.hpp` |
| 8.4 多项式平移 / Chirp-Z / 连续点值平移 | `taylor_shift.hpp`、`chirp_z.hpp`、`consecutive_shift.hpp` |
| 8.5 位运算卷积 - FWT XOR / OR / AND | `fwt_xor.hpp`、`fwt_or.hpp`、`fwt_and.hpp` |
| 8.6 线性递推 - Bostan-Mori / Berlekamp-Massey | `bostan_mori.hpp`、`linear_recurrence.hpp`、`berlekamp_massey.hpp` |
| 8.7 多项式复合 / 拉格朗日反演 | `composition.hpp` |
| 8.8 生成函数 OGF / EGF | `ogf.hpp`、`egf.hpp` |
| 8.9 多项式多点求值 | `multipoint.hpp` |
| 8.10 多项式快速插值 | `fast_interpolation.hpp` |
| 8.11 拉格朗日插值 | `lagrange.hpp` |

FPS 头文件包含导数、积分、逆、log、exp、非负整数幂、sqrt（含前导零与无解）、sin / cos / tan / asin / atan；核心包含 NTT / 卷积、阶乘与批量求逆、模平方根及 Poly 基础运算。8.3 成员版、8.9 Multipoint 和 8.10 ProductTree 独立版均保留。8.7 同时包含慢速复合、复合逆及两种拉格朗日反演接口。

### 09 字符串：13 个原条目，13 份模板

原稿：`raw/09_string.md`；实现目录：`code/string/`。

| 原稿条目 | 模板文件 |
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

### 10 动态规划：17 个原条目，20 份模板

原稿：`raw/10_dp.md`；实现目录：`code/dp/`。

| 原稿条目 | 模板文件 |
| --- | --- |
| 10.1 0/1、完全、多重、分组背包 | `knapsack01.hpp`、`knapsack_complete.hpp`、`knapsack_bounded.hpp`、`knapsack_group.hpp` |
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

背包保留 0/1、完全、多重（二进制拆分）、分组以及恰好装满初始化；SOS 保留子集 / 超集及其反演。Hamilton 路径节保留 TSP 回起点记忆公式，CDQ 保留三维八种严格 / 非严格配置。

## Git 与工作区检查

- 十份 `raw/` 文件逐一与 Git HEAD 的原始字节计算 SHA-256，全部一致；另与迁移前保存的 85 个受保护文件哈希核对，全部未变。
- `git diff -- raw` 为空，`git diff --cached` 为空；没有已暂存项、删除项或新增已跟踪 PDF。
- `git diff --check` 通过；另检查所有未跟踪文本文件的行尾空白，因为常规 diff 不包含这些文件。
- PDF、字体缓存、验收截图 / 日志 / 可执行文件位于已忽略路径；没有出现在待提交列表。未跟踪文件列表没有 exe、o、pyc、PDF 等构建产物。原有初始化 `.gitkeep` 保留。
- 本轮使用的测试日志和渲染证据保存在被忽略的 `.cache/notebook-fonts/acceptance_qa/`，不建议提交。

最终 `git status --short --branch`：

```text
## main...origin/main
 M .gitignore
 M AGENTS.md
 M README.md
 M main.typ
?? acceptance_report.md
?? code/data_structure/
?? code/dp/
?? code/geometry/
?? code/interval_greedy/
?? code/math/
?? code/poly/
?? code/string/
?? migration_report.md
?? sections/04_data_structure.typ
?? sections/05_interval_greedy.typ
?? sections/06_math.typ
?? sections/07_geometry.typ
?? sections/08_poly.typ
?? sections/09_string.typ
?? sections/10_dp.typ
?? test/check_examples.py
?? test/check_headers.py
?? test/check_manifest.py
?? test/data_structure/
?? test/dp/
?? test/geometry/
?? test/interval_greedy/
?? test/math/
?? test/migration_manifest.json
?? test/poly/
?? test/run_all.py
?? test/run_chapter.py
?? test/string/
```

按文件展开：4 个已跟踪文件修改、302 个未跟踪文件；无暂存或删除。完整迁移成果仍待维护者统一审查提交。
