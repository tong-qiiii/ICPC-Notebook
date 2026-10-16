# ICPC Notebook 最终验收复核报告

验收日期：2026-10-16。以用户已确认的 Typst 原生自动编号规则为准。本报告替代旧 `acceptance_report.md` 中因“新编号不等于原稿编号”而暂缓提交的结论。

**结论：本次要求的本地构建、编号、文件完整性检查全部通过，未发现必须修复的剩余阻塞，建议将源码、章节、测试、配置和维护文档提交 GitHub。人工复核项继续保留，不代表所有算法在任意赛题或最大规模上已获证明。**

本轮没有新增算法、修改算法逻辑、重命名模板或调整排版。没有 commit、push、Tag、Release 或远程权限操作。

## 已通过

### 自动编号与完整性

- 10 章按入口顺序导入且无重复；逐章核对原稿全部二级条目、当前算法块和主模板归属。174 个原稿条目对应 177 个算法组、196 份头文件，未发现遗漏、意外重复或调换。每份头文件恰好归属一个主条目；PDF 中重复展开依赖不另计算法。
- 章节和算法标题均使用唯一稳定标签。编号由 `heading(numbering: "1.1")` 和原生 counter 产生，未使用硬编码显示编号、手动更新 counter 或重新编号脚本。
- 三级 / 四级分类、接口、调用、注意事项和组内变体均不编号、不入目录；本次未发现辅助标题错误占用编号。
- 实测 10 个编号场景：基线、章首插入、章中插入、交换、删除、跨章移动、章节重排、章节文件改名、辅助标题、删除被引用算法。前 9 项通过完整 PDF 检查；最后一项按预期因悬空标签报错。临时操作只发生在副本，原工作区哈希不变。
- 本轮补强 `test/typst/numbering_test.py`：除 PDF 书签及正文外，还核对纸面目录标题的顺序、编号和印刷页码。补强后重跑全部场景。根目录最终 PDF 的 187 个目录 / 书签 / 正文标题及目标页码也单独核对通过。

#### 所有编号变化的原因

| 原稿范围 | 当前显示编号 | 原因及结论 |
| --- | --- | --- |
| 2.1–2.8 | 同号 | 未发生变化 |
| 2.9 Floyd / 传递闭包 | 2.9、2.10 | 原稿已有两种实现分为独立条目，正常 |
| 2.10–2.15 | 2.11–2.16 | 前述拆分使后续 +1，正常 |
| 2.16 带权 DSU | 2.17、2.18、2.19 | 保留加法、XOR、模 k 三种实现，分为三个独立条目，正常 |
| 2.17–2.34 | 2.20–2.37 | 两处拆分累计 +3，正常；最大权闭合子图仍是最后一项 |
| 4.1–4.2 | 同号 | 未发生变化 |
| 无编号笛卡尔树 | 4.3 | 原稿已有独立算法参与自动编号，正常 |
| 4.3–4.20 | 4.4–4.21 | 笛卡尔树占用其实际位置，后续 +1，正常 |
| 六个无编号主席树条目 | 4.22–4.27 | 六种独立模板按原位置编号，正常 |
| 4.23–4.34 | 4.28–4.39 | 原稿 4.21/4.22 缺号不保留为空位，加上前述附项，正常 |
| 无编号 CDQ 优化三维偏序 DP | 10.17 | 原稿已有章末独立算法参与编号，正常 |
| 其余条目 | 同原编号 | 内容及相对顺序未变；同组多份实现不额外占号 |

以上都是既有实现的拆分或原稿无编号算法进入连续编号，不是本轮增加新算法。后面的完整算法表列出逐项来源与当前编号，可追溯每个变化。

### C++17 和现有自动化测试

- 新鲜运行 `python test/run_all.py`，全部十章通过，退出码 0，结束标记 `PASS ALL 10 CHAPTERS`。没有跳过或把失败项当作通过。
- 196 份头文件通过独立 C++17 编译。工具章原运行器使用 GNU++17，本轮另对其全部 10 份头文件显式执行 `-std=c++17 -Wall -Wextra -Werror -fsyntax-only`，均通过；其余章节运行器包含 `-std=c++17` 检查。
- 149 个运行测试单元全部通过；适用的基础、边界、多次初始化及随机 / 穷举对拍均由已有测试执行。图论、树论及新七章包含个人基础模板宏兼容和多翻译单元链接检查。
- 196 份 PDF 最小调用示例通过编译，其中 186 份实际运行；工具章 10 份示例仅语法编译，行为由工具综合测试在提交 / LOCAL 两种模式运行覆盖。
- `test/check_manifest.py` 与 `test/check_examples.py` 均通过：新七章 121 个原稿条目、140 个模板及其测试对应完整，原稿哈希不变；140 个展开源码调用示例实际运行。
- 环境为 Windows、GCC 15.1.0、Python 3.14。使用断言、标准库断言及相应运行测试的 `-ftrapv`；未做全库 sanitizer 或 Ubuntu 实测。Boost.Multiprecision 使用已有本地头文件，无需链接 Boost 库。

| 章 | 原稿条目 | 自动编号算法组 | 头文件 | 运行测试单元 | 本轮结果 | PDF 物理页范围（含共享页） |
| --- | ---: | ---: | ---: | ---: | --- | --- |
| 1 基础工具 | 10 | 10 | 10 | 1 | 通过 | 3–6 |
| 2 图论 | 34 | 37 | 37 | 4 | 通过 | 6–28 |
| 3 树论 | 9 | 9 | 9 | 4 | 通过 | 28–35 |
| 4 数据结构 | 39 | 39 | 40 | 40 | 通过 | 35–64 |
| 5 区间贪心 / 双指针 | 5 | 5 | 9 | 9 | 通过 | 64–66 |
| 6 数学 | 18 | 18 | 22 | 22 | 通过 | 66–77 |
| 7 计算几何 | 18 | 18 | 18 | 18 | 通过 | 77–130 |
| 8 多项式 | 11 | 11 | 18 | 18 | 通过 | 130–178 |
| 9 字符串 | 13 | 13 | 13 | 13 | 通过 | 178–186 |
| 10 动态规划 | 17 | 17 | 20 | 20 | 通过 | 186–195 |
| 合计 | 174 | 177 | 196 | 149 | 全部通过 | 193 个不同正文页 |

### Typst PDF

- Typst 0.15.1 已重新编译 `main.typ`：**195 页**，含封面、完整目录和十章。固定字体校验通过，禁用系统字体搜索，保留现有 A4 双栏及全部字号、边距、栏距、标题和代码样式。
- 187 个纸面目录 / PDF 书签条目（10 章 + 177 算法组）与正文及目标页一致。封面无页码，目录为印刷第 1 页，正文印刷页号 = 物理页号 − 1。章节可能共用过渡页，表中范围不能简单相加。
- 全部 496 个块状源码 / 现场记忆与编号改动前基准一致，逐块可在最终 PDF 提取文本中找到。没有源码块缺失、仓库本地头文件 include、U+FFFD 或 NUL。
- 对全部 195 页生成九组渲染总览并查看，抽查完整目录、各章开头、长代码、表格和末页；另用 Poppler 渲染物理第 193 页，实际检查 CDQ 跨栏宽表。没有发现代码越栏、乱码或异常空白页。5.7pt 文本坐标检查（含中文回退字体）越界数为 0。
- 封面与既有基准像素一致；末页正常自然留白。没有为压页数删内容、缩字或强制算法分页。
- 最终 PDF SHA-256：`f5edea6eb2c14ff82be4c3db89b59319669062b2c6bb9aea6e2eacd47c64aa87`。本地 `.sha256` 已同步；PDF 和校验文件均被忽略，不提交 Git。

### 未跟踪文件和 Git

用户所引“4 个已跟踪修改 + 302 个未跟踪”是更早验收的快照。本轮开始实测为 **8 个已跟踪修改 + 304 个未跟踪**：此前自动编号工作新增 `numbering_report.md` 和 `test/typst/numbering_test.py`。本轮再新增本报告，最终为 **8 个已跟踪修改 + 305 个未跟踪文件**。

| 类别 | 旧 302 | 本轮开始 304 | 最终 305 | 是否应提交 |
| --- | ---: | ---: | ---: | --- |
| 第 4–10 章算法头文件 | 140 | 140 | 140 | 是，`code/` |
| 第 4–10 章 Typst 章节 | 7 | 7 | 7 | 是，`sections/` |
| C++ 运行测试 | 140 | 140 | 140 | 是，`test/` |
| Python 测试 / 维护工具 | 12 | 13 | 13 | 是，含自动编号回归测试 |
| 原稿 / 模板对应清单 JSON | 1 | 1 | 1 | 是，`test/migration_manifest.json` |
| 验收 / 迁移 / 编号文档 | 2 | 3 | 4 | 是，包含本报告；旧报告已标明历史状态 |
| 新增配置、缓存、PDF 或二进制产物 | 0 | 0 | 0 | 缓存和产物不应提交，当前均已忽略 |
| 总计 | 302 | 304 | 305 | 所有可见未跟踪文件都是应保留项目文件 |

项目配置没有新增未跟踪文件；`.gitignore` 等已跟踪文件的修改也应一并提交。最终状态明细见文末。

#### `.cache/` 处理结论

- 旧规则只忽略 `/.cache/notebook-fonts/`，本轮改为忽略整个根目录 `/.cache/`。另忽略 `*.exe`、`*.o`、`*.obj`；保持 PDF / 校验文件及 Python 缓存忽略规则。没有按 `.cpp`、`.hpp`、`.typ`、`.json` 或 `.out` 笼统忽略。
- 扫描全部可见未跟踪文件并读取内容：都是 UTF-8 项目源码 / 文档 / 测试，无 PDF、可执行文件、临时生成代码或缓存混入。对全部 304 个原待提交路径执行 `git check-ignore --no-index`，无误忽略；另外验证 `test/.../sample.out` 等预期输出及各类源码仍可见，虚拟缓存 / 二进制路径会被正确忽略。
- `.cache/` 没有已跟踪文件。内容包括下载字体、PyMuPDF 缓存依赖、一次性迁移 / 验收 Python 脚本、临时 C++ / Typst 文件、PDF / PNG / 日志 / JSON 及可执行文件。它们不属于待提交项目源码，本次只完善忽略规则，没有删除原文件。
- `batch_qa/`、`tree_qa/` 中的迁移生成脚本是历史一次性工具，含机器路径和迁移时点假设；不纳入长期维护，也不重新运行以免覆盖当前修复。`acceptance_qa/`、`numbering_qa/`、`final_qa/` 是本地验收证据和一次性检查脚本。
- 可重复使用的正式功能已在 `test/` 和 `scripts/` 中：十章运行器、头文件 / 示例 / manifest 检查、编号回归测试、固定字体下载。正式源码、入口和 CI 未引用缓存中的迁移 / 验收脚本。字体缓存是 `scripts/setup_fonts.py` 可重建的数据，工作流会自行准备。

本地缓存中的 Python 脚本（排除下载的第三方依赖）逐项归类为一次性工具，不提交：

```text
.cache/notebook-fonts/acceptance_qa/audit.py
.cache/notebook-fonts/acceptance_qa/check_restored.py
.cache/notebook-fonts/acceptance_qa/copy_check.py
.cache/notebook-fonts/acceptance_qa/pdf_audit.py
.cache/notebook-fonts/acceptance_qa/write_report.py
.cache/notebook-fonts/batch_qa/chapter10.py
.cache/notebook-fonts/batch_qa/chapter5.py
.cache/notebook-fonts/batch_qa/chapter6.py
.cache/notebook-fonts/batch_qa/chapter7.py
.cache/notebook-fonts/batch_qa/chapter8.py
.cache/notebook-fonts/batch_qa/chapter9.py
.cache/notebook-fonts/batch_qa/common.py
.cache/notebook-fonts/batch_qa/docs.py
.cache/notebook-fonts/batch_qa/migrate.py
.cache/notebook-fonts/batch_qa/qa_pdf.py
.cache/notebook-fonts/batch_qa/report_tables.py
.cache/notebook-fonts/batch_qa/tests4.py
.cache/notebook-fonts/final_qa/audit.py
.cache/notebook-fonts/final_qa/basic_cxx17.py
.cache/notebook-fonts/final_qa/pdf_check.py
.cache/notebook-fonts/final_qa/root_numbering.py
.cache/notebook-fonts/final_qa/write_report.py
.cache/notebook-fonts/numbering_qa/pdf_check.py
.cache/notebook-fonts/numbering_qa/write_report.py
.cache/notebook-fonts/tree_qa/migrate_tree.py
.cache/notebook-fonts/tree_qa/tree_docs.py
```

## 需要人工复核

保留前次报告的全部实质风险，以下均未因本轮通过而被提升为“所有情况已证明正确”：

| 范围 | 仍需人工判断 |
| --- | --- |
| 莫队 / 双指针 / 数位 DP | 实际回调状态、add/del/undo 对称性、前导零与计数范围；骨架测试不替代题目维护逻辑 |
| 分治 / Knuth / WQS 优化 | 决策单调性、合并条件、离散凸性、惩罚搜索界；测试只覆盖满足前提的实例 |
| 实数 Gauss / Simpson | 病态矩阵、绝对 EPS 与尺度、积分奇点 / 强振荡；递归上限不保证误差达标 |
| 浮点几何 / 公切线 / 三维 | 近平行、近相切、近共线和极端尺度；切线测试未独立证明全部输入的切线数量完整，三维主要是旋转不变量和少数交点 |
| FPS / 复合及插值 | 150 组小长度恒等式和独立递推不足以证明全部最大规模分支；NTT 的 2^23 长度测试不等于所有 FPS 都已做同规模压力测试 |
| 递归图 / 树 / FHQ | 比赛环境的深链栈限及最大规模内存 / 时间；4000 点链测试不能推出 2e5 深链安全 |
| 整数、路径 / 费用 / 几何 / DP 计数 | 按题确认 ll、i128、哨兵与中间乘加范围；本轮没有完成全库 ASan/UBSan |
| Hash / 随机算法 | 哈希碰撞和 Treap / Rho / MEC 随机最坏耗时；Duval / 最小表示法按 ASCII 约定使用 |
| 原稿重复和标题 | 7.7 / 7.8 原稿本来重复，两份均保留，不是迁移意外重复；6.18 标题含 Cipolla，但原稿只有 Tonelli-Shanks，没有凭空补实现 |
| 比赛体验 | 尚无真实队员 30 秒接口理解计时、100% 试印或携带页数规则确认；195 页成品的实际可用性需队伍判断 |
| GitHub Ubuntu | 本轮没有 push、触发远程 CI 或 Ubuntu 实测；已核对相同固定 Typst / 字体配置，不能声称已实证跨平台字节一致 |

## 必须修复

**本次复核未发现尚未关闭的确定性构建、编号、内容完整性或 Git 产物混入问题。** 旧报告的图论编号阻塞按用户确认的自动编号规则关闭；没有为恢复原稿数字而硬写编号。

本轮实际变更：完善 `.gitignore`；README 记录正式文件与缓存职责并链接当前报告；旧验收报告标明历史结论已被替代；补强 `test/typst/numbering_test.py` 的纸面目录检查；新增本报告。其余工作区修改来自此前迁移 / 编号任务，仍保留供用户统一检查。`raw/` 全部十文件与 Git HEAD 字节哈希相同；所有 `code/`、`sections/` 和 `main.typ` 与本轮开始快照相同。

## 完整算法及当前编号清单

| 当前编号 | 算法 | 原稿来源 | 主模板（同组变体均列出） |
| --- | --- | --- | --- |
| 1.1 | GCD | 1.1 GCD | `code/basic/gcd.hpp` |
| 1.2 | 位运算枚举 | 1.2 位运算枚举 | `code/basic/bit_enumeration.hpp` |
| 1.3 | 快读 | 1.3 快读 | `code/basic/fast_io.hpp` |
| 1.4 | 取模类 Mint | 1.4 取模类 Mint | `code/basic/mint.hpp` |
| 1.5 | STL 二分 | 1.5 STL 二分 | `code/basic/binary_search.hpp` |
| 1.6 | 对拍 | 1.6 对拍 | `code/basic/stress.hpp` |
| 1.7 | 128 位整数输入输出 | 1.7 __int128 输入输出 | `code/basic/int128_io.hpp` |
| 1.8 | 随机数 RNG | 1.8 随机数 RNG | `code/basic/rng.hpp` |
| 1.9 | Custom Hash / 防 Hack | 1.9 Custom Hash / 防 Hack | `code/basic/custom_hash.hpp` |
| 1.10 | Debug 宏 | 1.10 Debug 宏 | `code/basic/debug.hpp` |
| 2.1 | 强连通分量 SCC - Tarjan + 缩点 | 2.1 强连通分量 SCC - Tarjan + 缩点 | `code/graph/scc.hpp` |
| 2.2 | 割点 / 桥 | 2.2 割点 / 桥 | `code/graph/cut_bridge.hpp` |
| 2.3 | 边双连通分量 EBCC | 2.3 边双连通分量 EBCC | `code/graph/ebcc.hpp` |
| 2.4 | 点双连通分量 VBCC / 圆方树 | 2.4 点双连通分量 VBCC / 圆方树 | `code/graph/vbcc.hpp` |
| 2.5 | 2-SAT | 2.5 2-SAT | `code/graph/two_sat.hpp` |
| 2.6 | 链式前向星 Vector 版 | 2.6 链式前向星 Vector 版 | `code/graph/forward_star.hpp` |
| 2.7 | Dijkstra 最短路 | 2.7 Dijkstra 最短路 | `code/graph/dijkstra.hpp` |
| 2.8 | SPFA 判负环 | 2.8 SPFA 判负环 | `code/graph/negative_cycle.hpp` |
| 2.9 | Floyd 全源最短路 | 2.9 Floyd：全源最短路 / 传递闭包 | `code/graph/floyd.hpp` |
| 2.10 | bitset 传递闭包 | 2.9 Floyd：全源最短路 / 传递闭包 | `code/graph/transitive_closure.hpp` |
| 2.11 | 差分约束 | 2.10 差分约束 | `code/graph/difference_constraints.hpp` |
| 2.12 | Kruskal 最小生成树 | 2.11 Kruskal 最小生成树 | `code/graph/kruskal.hpp` |
| 2.13 | Prim 最小生成树 | 2.12 Prim 最小生成树 | `code/graph/prim.hpp` |
| 2.14 | 并查集 DSU | 2.13 并查集 DSU | `code/graph/dsu.hpp` |
| 2.15 | 可撤销并查集 Rollback DSU | 2.14 可撤销并查集 Rollback DSU | `code/graph/rollback_dsu.hpp` |
| 2.16 | 可持久化并查集 Persistent DSU | 2.15 可持久化并查集 Persistent DSU | `code/graph/persistent_dsu.hpp` |
| 2.17 | 加法差分带权并查集 | 2.16 带权并查集 | `code/graph/weighted_dsu.hpp` |
| 2.18 | XOR / 奇偶带权并查集 | 2.16 带权并查集 | `code/graph/xor_dsu.hpp` |
| 2.19 | 模 k 带权并查集 | 2.16 带权并查集 | `code/graph/mod_dsu.hpp` |
| 2.20 | 拓展域并查集 | 2.17 拓展域并查集 | `code/graph/extended_domain_dsu.hpp` |
| 2.21 | 并查集虚点 / 参考点 | 2.18 并查集虚点 / 参考点 | `code/graph/dsu_reference.hpp` |
| 2.22 | 拓扑排序 | 2.19 拓扑排序 | `code/graph/topological_sort.hpp` |
| 2.23 | 欧拉路径 / 欧拉回路：有向图 | 2.20 欧拉路径 / 欧拉回路：有向图 | `code/graph/euler_directed.hpp` |
| 2.24 | 欧拉路径 / 欧拉回路：无向图 | 2.21 欧拉路径 / 欧拉回路：无向图 | `code/graph/euler_undirected.hpp` |
| 2.25 | 线段树优化建图 | 2.22 线段树优化建图 | `code/graph/segment_graph.hpp` |
| 2.26 | 倍增优化树上路径建图 | 2.23 倍增优化树上路径建图 | `code/graph/jump_graph.hpp` |
| 2.27 | 最大流 - Dinic | 2.24 最大流 - Dinic | `code/graph/dinic.hpp` |
| 2.28 | 最小费用最大流 - SPFA | 2.25 最小费用最大流 - SPFA | `code/graph/min_cost_flow_spfa.hpp` |
| 2.29 | 最小费用最大流 - 势能 Dijkstra / Johnson | 2.26 最小费用最大流 - 势能 Dijkstra / Johnson | `code/graph/min_cost_flow_potential.hpp` |
| 2.30 | 最小割树 - Gomory-Hu | 2.27 最小割树 - Gomory-Hu | `code/graph/gomory_hu.hpp` |
| 2.31 | 全局最小割 - Stoer-Wagner | 2.28 全局最小割 - Stoer-Wagner | `code/graph/stoer_wagner.hpp` |
| 2.32 | 0-1 BFS | 2.29 0-1 BFS | `code/graph/zero_one_bfs.hpp` |
| 2.33 | 二分图最大匹配 Hopcroft-Karp | 2.30 二分图最大匹配 Hopcroft-Karp | `code/graph/hopcroft_karp.hpp` |
| 2.34 | KM / Hungarian Assignment | 2.31 KM / Hungarian Assignment | `code/graph/hungarian.hpp` |
| 2.35 | 上下界可行流 | 2.32 上下界可行流 | `code/graph/lower_bound_flow.hpp` |
| 2.36 | 上下界最大流 | 2.33 上下界最大流 | `code/graph/lower_bound_max_flow.hpp` |
| 2.37 | 最大权闭合子图 | 2.34 最大权闭合子图 | `code/graph/max_weight_closure.hpp` |
| 3.1 | 倍增 LCA | 3.1 倍增 LCA | `code/tree/lca.hpp` |
| 3.2 | 树剖 LCA | 3.2 树剖 LCA | `code/tree/lca_hld.hpp` |
| 3.3 | Tarjan 离线 LCA | 3.3 Tarjan 离线 LCA | `code/tree/lca_tarjan.hpp` |
| 3.4 | 重链剖分 HLD | 3.4 重链剖分 HLD | `code/tree/hld.hpp` |
| 3.5 | 虚树 | 3.5 虚树 | `code/tree/virtual_tree.hpp` |
| 3.6 | 启发式合并 / DSU on Tree：子树不同颜色数 | 3.6 启发式合并 / DSU on Tree：子树不同颜色数 | `code/tree/dsu_on_tree.hpp` |
| 3.7 | 点分治：统计距离不超过 k 的点对数 | 3.7 点分治：统计距离不超过 k 的点对数 | `code/tree/centroid.hpp` |
| 3.8 | Kruskal 重构树 | 3.8 Kruskal 重构树 | `code/tree/kruskal_tree.hpp` |
| 3.9 | LCT：动态森林 / 路径加 / 路径和 | 3.9 LCT：动态森林 / 路径加 / 路径和 | `code/tree/link_cut_tree.hpp` |
| 4.1 | 单调队列：滑动窗口最小值 / 最大值 | 4.1 单调队列：滑动窗口最小值 / 最大值 | `code/data_structure/sliding_window.hpp` |
| 4.2 | 单调栈：左右最近严格更小元素 | 4.2 单调栈：左右最近严格更小元素 | `code/data_structure/monotonic_stack.hpp` |
| 4.3 | 笛卡尔树 - 单调栈建树 | 笛卡尔树 - 单调栈建树 | `code/data_structure/cartesian_tree.hpp` |
| 4.4 | ST 表：区间最大值 | 4.3 ST 表：区间最大值 | `code/data_structure/sparse_table.hpp` |
| 4.5 | Fenwick 树：动态集合第 k 小 | 4.4 Fenwick 树：动态集合第 k 小 | `code/data_structure/fenwick_kth.hpp` |
| 4.6 | 线性基 | 4.5 线性基 | `code/data_structure/linear_basis.hpp` |
| 4.7 | 普通莫队 | 4.6 普通莫队 | `code/data_structure/mo.hpp` |
| 4.8 | 带修莫队 | 4.7 带修莫队 | `code/data_structure/mo_updates.hpp` |
| 4.9 | 只增回滚莫队 | 4.8 只增回滚莫队 | `code/data_structure/mo_rollback_add.hpp`、`code/data_structure/mo_max_span.hpp` |
| 4.10 | 只删回滚莫队 | 4.9 只删回滚莫队 | `code/data_structure/mo_rollback_delete.hpp` |
| 4.11 | 树上莫队 | 4.10 树上莫队 | `code/data_structure/mo_tree.hpp` |
| 4.12 | 二次离线莫队：二元组贡献 | 4.11 二次离线莫队：二元组贡献 | `code/data_structure/mo_second_offline.hpp` |
| 4.13 | 值域分块：频次 / 第 k 小 / 前驱后继 | 4.12 值域分块：频次 / 第 k 小 / 前驱后继 | `code/data_structure/value_block.hpp` |
| 4.14 | 序列分块：区间加 / 区间和 | 4.13 序列分块：区间加 / 区间和 | `code/data_structure/sequence_block.hpp` |
| 4.15 | InfoTag 线段树 | 4.14 InfoTag 线段树 | `code/data_structure/segment_tree_lazy.hpp` |
| 4.16 | Info 线段树：单点修改 | 4.15 Info 线段树：单点修改 | `code/data_structure/segment_tree_point.hpp` |
| 4.17 | 李超线段树：整数最大值 | 4.16 李超线段树：整数最大值 | `code/data_structure/li_chao_integer.hpp` |
| 4.18 | 李超线段树：浮点系数最大值（整数横坐标） | 4.17 李超线段树：浮点系数最大值（整数横坐标） | `code/data_structure/li_chao_double.hpp` |
| 4.19 | 栈维护凸包最值 | 4.18 栈维护凸包最值 | `code/data_structure/line_hull.hpp` |
| 4.20 | 动态开点线段树：区间赋值 + 剩余数量 | 4.19 动态开点线段树：区间赋值 + 剩余数量 | `code/data_structure/dynamic_segment_tree.hpp` |
| 4.21 | 线段树合并：点计数 + 区间最值 | 4.20 线段树合并：点计数 + 区间最值 | `code/data_structure/segment_tree_merge.hpp` |
| 4.22 | 主席树：Info 单点修改 | 主席树：Info 单点修改 | `code/data_structure/persistent_segment_tree_point.hpp` |
| 4.23 | 主席树：InfoTag 区间修改 | 主席树：InfoTag 区间修改 | `code/data_structure/persistent_segment_tree_lazy.hpp` |
| 4.24 | 主席树：值域计数 | 主席树：值域计数 | `code/data_structure/persistent_segment_tree_count.hpp` |
| 4.25 | 主席树：区间不同颜色数量 | 主席树：区间不同颜色数量 | `code/data_structure/persistent_distinct.hpp` |
| 4.26 | 主席树：区间 mex | 主席树：区间 mex | `code/data_structure/persistent_mex.hpp` |
| 4.27 | 主席树：树上第 k 小 | 主席树：树上第 k 小 | `code/data_structure/persistent_tree_kth.hpp` |
| 4.28 | 可持久化 01 Trie | 4.23 可持久化 01 Trie | `code/data_structure/persistent_trie.hpp` |
| 4.29 | 前缀线性基：区间异或最大值 | 4.24 前缀线性基：区间异或最大值 | `code/data_structure/prefix_linear_basis.hpp` |
| 4.30 | FHQ Treap：按值平衡树 | 4.25 FHQ Treap：按值平衡树 | `code/data_structure/fhq_set.hpp` |
| 4.31 | FHQ Treap：隐式序列 | 4.26 FHQ Treap：隐式序列 | `code/data_structure/fhq_sequence.hpp` |
| 4.32 | 可持久化 FHQ Treap：按值平衡树 | 4.27 可持久化 FHQ Treap：按值平衡树 | `code/data_structure/persistent_fhq_set.hpp` |
| 4.33 | 可持久化 FHQ Treap：隐式序列 | 4.28 可持久化 FHQ Treap：隐式序列 | `code/data_structure/persistent_fhq_sequence.hpp` |
| 4.34 | 可持久化隐式 FHQ：区间和扩展 | 4.29 可持久化隐式 FHQ：区间和扩展 | `code/data_structure/persistent_fhq_sum.hpp` |
| 4.35 | 树状数组：动态第 k 小 / 前 k 小之和 | 4.30 树状数组：动态第 k 小 / 前 k 小之和 | `code/data_structure/fenwick_k_sum.hpp` |
| 4.36 | 双 multiset 维护前 k 小之和 | 4.31 双 multiset 维护前 k 小之和 | `code/data_structure/multiset_k_sum.hpp` |
| 4.37 | set 维护不相交区间集合 | 4.32 set 维护不相交区间集合 | `code/data_structure/interval_union.hpp` |
| 4.38 | set 维护不相交区间集合 | 4.33 set 维护不相交区间集合 | `code/data_structure/interval_ids.hpp` |
| 4.39 | 高精度整数 cpp_int | 4.34 高精度整数 `cpp_int` | `code/data_structure/big_integer.hpp` |
| 5.1 | 区间集合合法二元组计数 | 5.1 区间集合合法二元组计数 | `code/interval_greedy/count_pairs_small.hpp`、`code/interval_greedy/count_pairs_large.hpp` |
| 5.2 | 区间最小点覆盖 / 最大不相交区间数 | 5.2 区间最小点覆盖 / 最大不相交区间数 | `code/interval_greedy/min_points.hpp`、`code/interval_greedy/max_disjoint.hpp` |
| 5.3 | 区间分组 | 5.3 区间分组 | `code/interval_greedy/interval_group.hpp` |
| 5.4 | 区间覆盖目标区间 | 5.4 区间覆盖目标区间 | `code/interval_greedy/min_cover.hpp` |
| 5.5 | 双指针模板 | 5.5 双指针模板 | `code/interval_greedy/window_longest.hpp`、`code/interval_greedy/window_shortest.hpp`、`code/interval_greedy/two_ends.hpp` |
| 6.1 | ModInt / 快速幂 / 逆元 / 扩展欧几里得 | 6.1 ModInt / 快速幂 / 逆元 / 扩展欧几里得 | `code/math/modint.hpp`、`code/math/qpow.hpp`、`code/math/extended_gcd.hpp`、`code/math/inverse_mod.hpp` |
| 6.2 | 组合数预处理：阶乘 / 逆阶乘 | 6.2 组合数预处理：阶乘 / 逆阶乘 | `code/math/combination.hpp` |
| 6.3 | Lucas 定理 | 6.3 Lucas 定理 | `code/math/lucas.hpp` |
| 6.4 | 整除分块 | 6.4 整除分块 | `code/math/division_blocks.hpp` |
| 6.5 | 线性筛 | 6.5 线性筛 | `code/math/linear_sieve.hpp` |
| 6.6 | 欧拉函数 / 莫比乌斯函数：线性筛 | 6.6 欧拉函数 / 莫比乌斯函数：线性筛 | `code/math/multi_sieve.hpp` |
| 6.7 | 莫比乌斯反演：互质对数 | 6.7 莫比乌斯反演：互质对数 | `code/math/mobius_pairs.hpp` |
| 6.8 | 矩阵快速幂 | 6.8 矩阵快速幂 | `code/math/matrix_power.hpp` |
| 6.9 | 高斯消元：实数线性方程组 | 6.9 高斯消元：实数线性方程组 | `code/math/gauss_double.hpp` |
| 6.10 | 高斯消元：模素数线性方程组 | 6.10 高斯消元：模素数线性方程组 | `code/math/gauss_mod.hpp` |
| 6.11 | 容斥原理：子集枚举 | 6.11 容斥原理：子集枚举 | `code/math/subset_ie.hpp`、`code/math/div_any.hpp` |
| 6.12 | 自适应辛普森积分 | 6.12 自适应辛普森积分 | `code/math/adaptive_simpson.hpp` |
| 6.13 | CRT 中国剩余定理 | 6.13 CRT 中国剩余定理 | `code/math/crt.hpp` |
| 6.14 | Miller-Rabin 素性测试 | 6.14 Miller-Rabin 素性测试 | `code/math/miller_rabin.hpp` |
| 6.15 | Pollard-Rho 大整数分解 | 6.15 Pollard-Rho 大整数分解 | `code/math/pollard_rho.hpp` |
| 6.16 | BSGS / exBSGS 离散对数 | 6.16 BSGS / exBSGS 离散对数 | `code/math/bsgs.hpp` |
| 6.17 | floor_sum | 6.17 floor_sum | `code/math/floor_sum.hpp` |
| 6.18 | Cipolla / Tonelli-Shanks 模平方根 | 6.18 Cipolla / Tonelli-Shanks 模平方根 | `code/math/mod_sqrt.hpp` |
| 7.1 | 二维几何基础 | 7.1 二维几何基础 | `code/geometry/geometry_base.hpp` |
| 7.2 | 直线与线段 | 7.2 直线与线段 | `code/geometry/line_segment.hpp` |
| 7.3 | 三角形常用工具 | 7.3 三角形常用工具 | `code/geometry/triangle.hpp` |
| 7.4 | 多边形基础 | 7.4 多边形基础 | `code/geometry/polygon.hpp` |
| 7.5 | 格点多边形：Pick 定理 | 7.5 格点多边形：Pick 定理 | `code/geometry/pick.hpp` |
| 7.6 | 二维凸包 Andrew | 7.6 二维凸包 Andrew | `code/geometry/convex_hull.hpp` |
| 7.7 | 凸多边形点包含 O(n)，含退化检查 | 7.7 凸多边形点包含 O(log n) | `code/geometry/convex_contains.hpp` |
| 7.8 | 凸多边形点包含 O(n)，含退化检查 | 7.8 凸多边形点包含 O(log n) | `code/geometry/convex_contains_variant.hpp` |
| 7.9 | 旋转卡壳：凸包直径 | 7.9 旋转卡壳：凸包直径 | `code/geometry/convex_diameter.hpp` |
| 7.10 | 旋转卡壳：最小面积外接矩形 | 7.10 旋转卡壳：最小面积外接矩形 | `code/geometry/bounding_rectangle.hpp` |
| 7.11 | 最近点对 | 7.11 最近点对 | `code/geometry/closest_pair.hpp` |
| 7.12 | 半平面交 | 7.12 半平面交 | `code/geometry/halfplane_intersection.hpp` |
| 7.13 | 圆基础 | 7.13 圆基础 | `code/geometry/circle.hpp` |
| 7.14 | 圆的切线 | 7.14 圆的切线 | `code/geometry/circle_tangents.hpp` |
| 7.15 | 最小圆覆盖 | 7.15 最小圆覆盖 | `code/geometry/minimum_enclosing_circle.hpp` |
| 7.16 | Minkowski Sum | 7.16 Minkowski Sum | `code/geometry/minkowski_sum.hpp` |
| 7.17 | Convex Hull Trick：单调直线凸包 | 7.17 Convex Hull Trick：单调直线凸包 | `code/geometry/monotone_cht.hpp` |
| 7.18 | 三维几何基础 | 7.18 三维几何基础 | `code/geometry/geometry_3d.hpp` |
| 8.1 | 多项式核心 - NTT / Poly | 8.1 多项式核心 - NTT / Poly | `code/poly/poly_core.hpp` |
| 8.2 | 形式幂级数 FPS - 逆 / ln / exp / 幂 / sqrt / 三角函数 | 8.2 形式幂级数 FPS - 逆 / ln / exp / 幂 / sqrt / 三角函数 | `code/poly/fps.hpp` |
| 8.3 | 多项式除法 / 多点求值 / 插值 | 8.3 多项式除法 / 多点求值 / 插值 | `code/poly/division_evaluation.hpp` |
| 8.4 | 多项式平移 / Chirp-Z / 连续点值平移 | 8.4 多项式平移 / Chirp-Z / 连续点值平移 | `code/poly/taylor_shift.hpp`、`code/poly/chirp_z.hpp`、`code/poly/consecutive_shift.hpp` |
| 8.5 | 位运算卷积 - FWT XOR / OR / AND | 8.5 位运算卷积 - FWT XOR / OR / AND | `code/poly/fwt_xor.hpp`、`code/poly/fwt_or.hpp`、`code/poly/fwt_and.hpp` |
| 8.6 | 线性递推 - Bostan-Mori / Berlekamp-Massey | 8.6 线性递推 - Bostan-Mori / Berlekamp-Massey | `code/poly/bostan_mori.hpp`、`code/poly/linear_recurrence.hpp`、`code/poly/berlekamp_massey.hpp` |
| 8.7 | 多项式复合 / 拉格朗日反演 | 8.7 多项式复合 / 拉格朗日反演 | `code/poly/composition.hpp` |
| 8.8 | 生成函数 OGF / EGF | 8.8 生成函数 OGF / EGF | `code/poly/ogf.hpp`、`code/poly/egf.hpp` |
| 8.9 | 多项式多点求值 | 8.9 多项式多点求值 | `code/poly/multipoint.hpp` |
| 8.10 | 多项式快速插值 | 8.10 多项式快速插值 | `code/poly/fast_interpolation.hpp` |
| 8.11 | 拉格朗日插值 | 8.11 拉格朗日插值 | `code/poly/lagrange.hpp` |
| 9.1 | KMP / Prefix Function | 9.1 KMP / Prefix Function | `code/string/kmp.hpp` |
| 9.2 | Z 函数 | 9.2 Z 函数 | `code/string/z_function.hpp` |
| 9.3 | Manacher：回文半径 | 9.3 Manacher：回文半径 | `code/string/manacher.hpp` |
| 9.4 | 字符串哈希：双模数 | 9.4 字符串哈希：双模数 | `code/string/hash_double.hpp` |
| 9.5 | Trie：字符串插入 / 前缀计数 | 9.5 Trie：字符串插入 / 前缀计数 | `code/string/trie.hpp` |
| 9.6 | 01 Trie：最大异或 / 最小异或 | 9.6 01 Trie：最大异或 / 最小异或 | `code/string/trie01.hpp` |
| 9.7 | AC 自动机：多模式匹配 | 9.7 AC 自动机：多模式匹配 | `code/string/aho_corasick.hpp` |
| 9.8 | 回文自动机 PAM | 9.8 回文自动机 PAM | `code/string/palindromic_tree.hpp` |
| 9.9 | 后缀自动机 SAM | 9.9 后缀自动机 SAM | `code/string/suffix_automaton.hpp` |
| 9.10 | 后缀数组 SA：O(n log n) | 9.10 后缀数组 SA：O(n log n) | `code/string/suffix_array.hpp` |
| 9.11 | LCP / Height + RMQ | 9.11 LCP / Height + RMQ | `code/string/lcp_rmq.hpp` |
| 9.12 | Duval：Lyndon 分解 | 9.12 Duval：Lyndon 分解 | `code/string/duval.hpp` |
| 9.13 | 字符串最小表示法 | 9.13 字符串最小表示法 | `code/string/minimum_rotation.hpp` |
| 10.1 | 0/1、完全、多重、分组背包 | 10.1 0/1、完全、多重、分组背包 | `code/dp/knapsack01.hpp`、`code/dp/knapsack_complete.hpp`、`code/dp/knapsack_bounded.hpp`、`code/dp/knapsack_group.hpp` |
| 10.2 | 区间 DP（石子合并骨架） | 10.2 区间 DP（石子合并骨架） | `code/dp/interval_merge.hpp` |
| 10.3 | 树形 DP（最大权独立集骨架） | 10.3 树形 DP（最大权独立集骨架） | `code/dp/tree_independent_set.hpp` |
| 10.4 | 换根 DP（全点距离和） | 10.4 换根 DP（全点距离和） | `code/dp/reroot_distance.hpp` |
| 10.5 | 数位 DP（记忆化通用骨架） | 10.5 数位 DP（记忆化通用骨架） | `code/dp/digit_dp.hpp` |
| 10.6 | 状压 DP（Hamilton 路径 / TSP 骨架） | 10.6 状压 DP（Hamilton 路径 / TSP 骨架） | `code/dp/hamilton_path.hpp` |
| 10.7 | 轮廓线 DP（多米诺骨牌铺满） | 10.7 轮廓线 DP（多米诺骨牌铺满） | `code/dp/domino_tilings.hpp` |
| 10.8 | 插头 DP（网格 Hamilton 单回路） | 10.8 插头 DP（网格 Hamilton 单回路） | `code/dp/plug_hamilton_cycle.hpp` |
| 10.9 | SOS DP（子集 / 超集和） | 10.9 SOS DP（子集 / 超集和） | `code/dp/sos_dp.hpp` |
| 10.10 | 单调队列优化 DP | 10.10 单调队列优化 DP | `code/dp/monotone_queue_dp.hpp` |
| 10.11 | 斜率优化 CHT | 10.11 斜率优化 CHT | `code/dp/cht_min.hpp` |
| 10.12 | 分治优化 DP | 10.12 分治优化 DP | `code/dp/divide_conquer_dp.hpp` |
| 10.13 | Knuth 优化（区间 DP） | 10.13 Knuth 优化（区间 DP） | `code/dp/knuth_merge.hpp` |
| 10.14 | Li Chao Tree 优化 DP | 10.14 Li Chao Tree 优化 DP | `code/dp/li_chao_min.hpp` |
| 10.15 | Bitset 优化 DP（0/1 子集和） | 10.15 Bitset 优化 DP（0/1 子集和） | `code/dp/bitset_subset_sum.hpp` |
| 10.16 | WQS 二分 / Aliens Trick | 10.16 WQS 二分 / Aliens Trick | `code/dp/wqs.hpp` |
| 10.17 | CDQ 优化三维偏序 DP | CDQ 优化三维偏序 DP | `code/dp/cdq_dominance.hpp` |

## 应提交的未跟踪文件逐项清单

下面列出本轮开始的 304 个路径及本轮新增的报告，共 305 个；每项均读取核查，没有缓存或编译产物。

### 算法源码（140）

```text
code/data_structure/big_integer.hpp
code/data_structure/cartesian_tree.hpp
code/data_structure/dynamic_segment_tree.hpp
code/data_structure/fenwick_k_sum.hpp
code/data_structure/fenwick_kth.hpp
code/data_structure/fhq_sequence.hpp
code/data_structure/fhq_set.hpp
code/data_structure/interval_ids.hpp
code/data_structure/interval_union.hpp
code/data_structure/li_chao_double.hpp
code/data_structure/li_chao_integer.hpp
code/data_structure/line_hull.hpp
code/data_structure/linear_basis.hpp
code/data_structure/mo.hpp
code/data_structure/mo_max_span.hpp
code/data_structure/mo_rollback_add.hpp
code/data_structure/mo_rollback_delete.hpp
code/data_structure/mo_second_offline.hpp
code/data_structure/mo_tree.hpp
code/data_structure/mo_updates.hpp
code/data_structure/monotonic_stack.hpp
code/data_structure/multiset_k_sum.hpp
code/data_structure/persistent_distinct.hpp
code/data_structure/persistent_fhq_sequence.hpp
code/data_structure/persistent_fhq_set.hpp
code/data_structure/persistent_fhq_sum.hpp
code/data_structure/persistent_mex.hpp
code/data_structure/persistent_segment_tree_count.hpp
code/data_structure/persistent_segment_tree_lazy.hpp
code/data_structure/persistent_segment_tree_point.hpp
code/data_structure/persistent_tree_kth.hpp
code/data_structure/persistent_trie.hpp
code/data_structure/prefix_linear_basis.hpp
code/data_structure/segment_tree_lazy.hpp
code/data_structure/segment_tree_merge.hpp
code/data_structure/segment_tree_point.hpp
code/data_structure/sequence_block.hpp
code/data_structure/sliding_window.hpp
code/data_structure/sparse_table.hpp
code/data_structure/value_block.hpp
code/dp/bitset_subset_sum.hpp
code/dp/cdq_dominance.hpp
code/dp/cht_min.hpp
code/dp/digit_dp.hpp
code/dp/divide_conquer_dp.hpp
code/dp/domino_tilings.hpp
code/dp/hamilton_path.hpp
code/dp/interval_merge.hpp
code/dp/knapsack01.hpp
code/dp/knapsack_bounded.hpp
code/dp/knapsack_complete.hpp
code/dp/knapsack_group.hpp
code/dp/knuth_merge.hpp
code/dp/li_chao_min.hpp
code/dp/monotone_queue_dp.hpp
code/dp/plug_hamilton_cycle.hpp
code/dp/reroot_distance.hpp
code/dp/sos_dp.hpp
code/dp/tree_independent_set.hpp
code/dp/wqs.hpp
code/geometry/bounding_rectangle.hpp
code/geometry/circle.hpp
code/geometry/circle_tangents.hpp
code/geometry/closest_pair.hpp
code/geometry/convex_contains.hpp
code/geometry/convex_contains_variant.hpp
code/geometry/convex_diameter.hpp
code/geometry/convex_hull.hpp
code/geometry/geometry_3d.hpp
code/geometry/geometry_base.hpp
code/geometry/halfplane_intersection.hpp
code/geometry/line_segment.hpp
code/geometry/minimum_enclosing_circle.hpp
code/geometry/minkowski_sum.hpp
code/geometry/monotone_cht.hpp
code/geometry/pick.hpp
code/geometry/polygon.hpp
code/geometry/triangle.hpp
code/interval_greedy/count_pairs_large.hpp
code/interval_greedy/count_pairs_small.hpp
code/interval_greedy/interval_group.hpp
code/interval_greedy/max_disjoint.hpp
code/interval_greedy/min_cover.hpp
code/interval_greedy/min_points.hpp
code/interval_greedy/two_ends.hpp
code/interval_greedy/window_longest.hpp
code/interval_greedy/window_shortest.hpp
code/math/adaptive_simpson.hpp
code/math/bsgs.hpp
code/math/combination.hpp
code/math/crt.hpp
code/math/div_any.hpp
code/math/division_blocks.hpp
code/math/extended_gcd.hpp
code/math/floor_sum.hpp
code/math/gauss_double.hpp
code/math/gauss_mod.hpp
code/math/inverse_mod.hpp
code/math/linear_sieve.hpp
code/math/lucas.hpp
code/math/matrix_power.hpp
code/math/miller_rabin.hpp
code/math/mobius_pairs.hpp
code/math/mod_sqrt.hpp
code/math/modint.hpp
code/math/multi_sieve.hpp
code/math/pollard_rho.hpp
code/math/qpow.hpp
code/math/subset_ie.hpp
code/poly/berlekamp_massey.hpp
code/poly/bostan_mori.hpp
code/poly/chirp_z.hpp
code/poly/composition.hpp
code/poly/consecutive_shift.hpp
code/poly/division_evaluation.hpp
code/poly/egf.hpp
code/poly/fast_interpolation.hpp
code/poly/fps.hpp
code/poly/fwt_and.hpp
code/poly/fwt_or.hpp
code/poly/fwt_xor.hpp
code/poly/lagrange.hpp
code/poly/linear_recurrence.hpp
code/poly/multipoint.hpp
code/poly/ogf.hpp
code/poly/poly_core.hpp
code/poly/taylor_shift.hpp
code/string/aho_corasick.hpp
code/string/duval.hpp
code/string/hash_double.hpp
code/string/kmp.hpp
code/string/lcp_rmq.hpp
code/string/manacher.hpp
code/string/minimum_rotation.hpp
code/string/palindromic_tree.hpp
code/string/suffix_array.hpp
code/string/suffix_automaton.hpp
code/string/trie.hpp
code/string/trie01.hpp
code/string/z_function.hpp
```

### Typst 章节（7）

```text
sections/04_data_structure.typ
sections/05_interval_greedy.typ
sections/06_math.typ
sections/07_geometry.typ
sections/08_poly.typ
sections/09_string.typ
sections/10_dp.typ
```

### 测试、维护脚本和对应清单（154）

```text
test/check_examples.py
test/check_headers.py
test/check_manifest.py
test/data_structure/big_integer_test.cpp
test/data_structure/cartesian_tree_test.cpp
test/data_structure/dynamic_segment_tree_test.cpp
test/data_structure/fenwick_k_sum_test.cpp
test/data_structure/fenwick_kth_test.cpp
test/data_structure/fhq_sequence_test.cpp
test/data_structure/fhq_set_test.cpp
test/data_structure/interval_ids_test.cpp
test/data_structure/interval_union_test.cpp
test/data_structure/li_chao_double_test.cpp
test/data_structure/li_chao_integer_test.cpp
test/data_structure/line_hull_test.cpp
test/data_structure/linear_basis_test.cpp
test/data_structure/mo_max_span_test.cpp
test/data_structure/mo_rollback_add_test.cpp
test/data_structure/mo_rollback_delete_test.cpp
test/data_structure/mo_second_offline_test.cpp
test/data_structure/mo_test.cpp
test/data_structure/mo_tree_test.cpp
test/data_structure/mo_updates_test.cpp
test/data_structure/monotonic_stack_test.cpp
test/data_structure/multiset_k_sum_test.cpp
test/data_structure/persistent_distinct_test.cpp
test/data_structure/persistent_fhq_sequence_test.cpp
test/data_structure/persistent_fhq_set_test.cpp
test/data_structure/persistent_fhq_sum_test.cpp
test/data_structure/persistent_mex_test.cpp
test/data_structure/persistent_segment_tree_count_test.cpp
test/data_structure/persistent_segment_tree_lazy_test.cpp
test/data_structure/persistent_segment_tree_point_test.cpp
test/data_structure/persistent_tree_kth_test.cpp
test/data_structure/persistent_trie_test.cpp
test/data_structure/prefix_linear_basis_test.cpp
test/data_structure/run_tests.py
test/data_structure/segment_tree_lazy_test.cpp
test/data_structure/segment_tree_merge_test.cpp
test/data_structure/segment_tree_point_test.cpp
test/data_structure/sequence_block_test.cpp
test/data_structure/sliding_window_test.cpp
test/data_structure/sparse_table_test.cpp
test/data_structure/value_block_test.cpp
test/dp/bitset_subset_sum_test.cpp
test/dp/cdq_dominance_test.cpp
test/dp/cht_min_test.cpp
test/dp/digit_dp_test.cpp
test/dp/divide_conquer_dp_test.cpp
test/dp/domino_tilings_test.cpp
test/dp/hamilton_path_test.cpp
test/dp/interval_merge_test.cpp
test/dp/knapsack01_test.cpp
test/dp/knapsack_bounded_test.cpp
test/dp/knapsack_complete_test.cpp
test/dp/knapsack_group_test.cpp
test/dp/knuth_merge_test.cpp
test/dp/li_chao_min_test.cpp
test/dp/monotone_queue_dp_test.cpp
test/dp/plug_hamilton_cycle_test.cpp
test/dp/reroot_distance_test.cpp
test/dp/run_tests.py
test/dp/sos_dp_test.cpp
test/dp/tree_independent_set_test.cpp
test/dp/wqs_test.cpp
test/geometry/bounding_rectangle_test.cpp
test/geometry/circle_tangents_test.cpp
test/geometry/circle_test.cpp
test/geometry/closest_pair_test.cpp
test/geometry/convex_contains_test.cpp
test/geometry/convex_contains_variant_test.cpp
test/geometry/convex_diameter_test.cpp
test/geometry/convex_hull_test.cpp
test/geometry/geometry_3d_test.cpp
test/geometry/geometry_base_test.cpp
test/geometry/halfplane_intersection_test.cpp
test/geometry/line_segment_test.cpp
test/geometry/minimum_enclosing_circle_test.cpp
test/geometry/minkowski_sum_test.cpp
test/geometry/monotone_cht_test.cpp
test/geometry/pick_test.cpp
test/geometry/polygon_test.cpp
test/geometry/run_tests.py
test/geometry/triangle_test.cpp
test/interval_greedy/count_pairs_large_test.cpp
test/interval_greedy/count_pairs_small_test.cpp
test/interval_greedy/interval_group_test.cpp
test/interval_greedy/max_disjoint_test.cpp
test/interval_greedy/min_cover_test.cpp
test/interval_greedy/min_points_test.cpp
test/interval_greedy/run_tests.py
test/interval_greedy/two_ends_test.cpp
test/interval_greedy/window_longest_test.cpp
test/interval_greedy/window_shortest_test.cpp
test/math/adaptive_simpson_test.cpp
test/math/bsgs_test.cpp
test/math/combination_test.cpp
test/math/crt_test.cpp
test/math/div_any_test.cpp
test/math/division_blocks_test.cpp
test/math/extended_gcd_test.cpp
test/math/floor_sum_test.cpp
test/math/gauss_double_test.cpp
test/math/gauss_mod_test.cpp
test/math/inverse_mod_test.cpp
test/math/linear_sieve_test.cpp
test/math/lucas_test.cpp
test/math/matrix_power_test.cpp
test/math/miller_rabin_test.cpp
test/math/mobius_pairs_test.cpp
test/math/mod_sqrt_test.cpp
test/math/modint_test.cpp
test/math/multi_sieve_test.cpp
test/math/pollard_rho_test.cpp
test/math/qpow_test.cpp
test/math/run_tests.py
test/math/subset_ie_test.cpp
test/migration_manifest.json
test/poly/berlekamp_massey_test.cpp
test/poly/bostan_mori_test.cpp
test/poly/chirp_z_test.cpp
test/poly/composition_test.cpp
test/poly/consecutive_shift_test.cpp
test/poly/division_evaluation_test.cpp
test/poly/egf_test.cpp
test/poly/fast_interpolation_test.cpp
test/poly/fps_test.cpp
test/poly/fwt_and_test.cpp
test/poly/fwt_or_test.cpp
test/poly/fwt_xor_test.cpp
test/poly/lagrange_test.cpp
test/poly/linear_recurrence_test.cpp
test/poly/multipoint_test.cpp
test/poly/ogf_test.cpp
test/poly/poly_core_test.cpp
test/poly/run_tests.py
test/poly/taylor_shift_test.cpp
test/run_all.py
test/run_chapter.py
test/string/aho_corasick_test.cpp
test/string/duval_test.cpp
test/string/hash_double_test.cpp
test/string/kmp_test.cpp
test/string/lcp_rmq_test.cpp
test/string/manacher_test.cpp
test/string/minimum_rotation_test.cpp
test/string/palindromic_tree_test.cpp
test/string/run_tests.py
test/string/suffix_array_test.cpp
test/string/suffix_automaton_test.cpp
test/string/trie01_test.cpp
test/string/trie_test.cpp
test/string/z_function_test.cpp
test/typst/numbering_test.py
```

### 维护文档（4）

```text
acceptance_report.md
final_acceptance_report.md
migration_report.md
numbering_report.md
```

## 最终 Git 状态

`git diff --check` 通过；未跟踪文本的尾随空白检查也通过。无暂存、删除或 raw 修改。

```text
 M .gitignore
 M AGENTS.md
 M README.md
 M TYPST_STYLE.md
 M main.typ
 M sections/01_tools.typ
 M sections/02_graph.typ
 M sections/03_tree.typ
?? acceptance_report.md
?? code/data_structure/
?? code/dp/
?? code/geometry/
?? code/interval_greedy/
?? code/math/
?? code/poly/
?? code/string/
?? final_acceptance_report.md
?? migration_report.md
?? numbering_report.md
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
?? test/typst/
```

按文件展开为 8 个已跟踪修改、305 个未跟踪文件；目录折叠显示不改变这个数量。建议用户检查后提交这批项目文件，PDF 由 GitHub Actions 构建；保留上述人工风险，不等同于宣布全部算法对任意输入均正确。
