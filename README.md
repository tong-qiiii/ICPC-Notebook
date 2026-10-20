# ICPC Notebook

用于整理 ICPC 竞赛算法模板、说明和验证用例，并使用 Typst 编译为便于查阅的 PDF Notebook。

已迁移第 1～10 章，并新增第 11 章“概率与期望”、第 12 章“组合计数与反演”，全部导入 `main.typ`。第 4～10 章的 121 个原始条目整理为 140 份模板，重要变体分别保留；完整文件清单、修复及验证范围见 [migration_report.md](migration_report.md)。`raw/` 永久保存原始 Markdown 备份，不修改其中任何文件。

## 目录结构

```text
ICPC-Notebook/
├── .github/workflows/build-notebook.yml  # 自动构建及标签发布
├── .gitignore      # 忽略生成的 PDF 和字体缓存
├── AGENTS.md       # 项目维护规则
├── TYPST_STYLE.md  # 唯一 Typst 视觉基准与维护要求
├── README.md       # 项目说明
├── main.typ        # Typst 总入口
├── scripts/        # 本地与 CI 共用的字体准备脚本
├── raw/            # 原始 Markdown 永久备份
│   ├── 01_tools.md
│   ├── 02_graph.md
│   ├── 03_tree.md
│   ├── 04_data_structure.md
│   ├── 05_interval_greedy.md
│   ├── 06_math.md
│   ├── 07_geometry.md
│   ├── 08_Poly.md
│   ├── 09_string.md
│   └── 10_dp.md
├── migration_report.md # 第 4～10 章迁移、修复与验证清单
├── sections/       # 01_tools.typ ～ 12_counting.typ
├── code/           # basic/、graph/、tree/、data_structure/、interval_greedy/
│                   # math/、geometry/、poly/、string/、dp/、probability/、counting/
└── test/           # 各章测试、随机对拍、独立编译及 PDF 调用验证
```

三个目录保留初始化时的 `.gitkeep`。

正式源码、章节、可重复运行的测试和维护脚本分别保存在 `code/`、`sections/`、`test/`、`scripts/`，应提交到 Git。根目录 `.cache/` 只存放下载字体、一次性迁移脚本及本地验收产物，整体忽略；正式构建与测试不得依赖其中的一次性脚本。生成的 PDF、校验文件、可执行文件和 Python 缓存不提交；测试用的预期输出文件不按扩展名笼统忽略。

最新验收结论、编号变化原因、完整算法及待提交文件清单见 [final_acceptance_report.md](final_acceptance_report.md)；报告中的人工复核项仍需按实际赛题检查。

## 编译 Typst

先安装 [Typst CLI 0.15.1](https://github.com/typst/typst/releases/tag/v0.15.1) 和 Python 3，并确保 `typst` 在 `PATH` 中。CI 固定使用相同 Typst 版本，避免升级编译器时意外改变分页。

排版严格继承用户提供的 `icpc_template_dynamic_10_twocol.typ`：原文案封面、双栏目录、紧凑 A4 双栏正文及浅灰代码块；详细参数见 [TYPST_STYLE.md](TYPST_STYLE.md)。正文优先使用 **Noto Serif CJK SC 2.003**（Regular、Bold），封面中文使用 **LXGW WenKai 1.522**（Regular）；封面英文和西文回退使用 Typst 内置的 New Computer Modern，代码使用内置的 DejaVu Sans Mono，公式使用内置数学字体。

字体来自 [Noto 官方仓库](https://github.com/notofonts/noto-cjk/tree/Serif2.003/Serif) 和 [LXGW WenKai 官方发布](https://github.com/lxgw/LxgwWenKai/releases/tag/v1.522)，分别遵循 [Noto 字体许可](https://github.com/notofonts/noto-cjk/blob/Serif2.003/Serif/LICENSE) 和 [LXGW 字体许可](https://github.com/lxgw/LxgwWenKai/blob/v1.522/OFL.txt)。脚本固定版本并校验 SHA-256，下载到被 Git 忽略的 `.cache/notebook-fonts/`，无需安装到系统。Ubuntu runner 不预设具备这些中文字体，工作流会在编译前运行同一脚本。

在仓库根目录执行以下命令。PowerShell：

```powershell
python scripts/setup_fonts.py
$env:TYPST_FONT_PATHS = (Join-Path $PWD '.cache/notebook-fonts')
$env:TYPST_IGNORE_SYSTEM_FONTS = 'true'
typst --version
typst fonts
typst compile main.typ ICPC-Notebook.pdf
```

Linux / macOS：

```sh
python3 scripts/setup_fonts.py
export TYPST_FONT_PATHS="$PWD/.cache/notebook-fonts"
export TYPST_IGNORE_SYSTEM_FONTS=true
typst --version
typst fonts
typst compile main.typ ICPC-Notebook.pdf
```

`typst fonts` 应包含 `Noto Serif CJK SC`、`LXGW WenKai`、`DejaVu Sans Mono`、`New Computer Modern` 和 `New Computer Modern Math`。本地与 CI 均禁用系统字体搜索，只使用固定中文字体与同版本 Typst 内置字体，减少 Windows / Ubuntu 字体回退造成的乱码和换行差异。编辑器预览也应使用这些字体路径及相同版本的编译器；比赛以构建生成的 PDF 为准。不得因 CI 缺字体而替换原版字体或改变栏宽、字号。

完成上述字体配置后，开发时可以监听文件变更并自动重新编译：

```sh
typst watch main.typ ICPC-Notebook.pdf
```

`main.typ` 已启用十二章，显示章节号按 include 的实际顺序自动生成。章节内的二级算法标题按位置连续编号，原稿编号只保存在来源信息中。现有文件名前缀是稳定的维护地址，不绑定显示编号。章节路径和导入规则见 [Typst 官方文档](https://typst.app/docs/reference/scripting/#modules)。

使用 VS Code 时，以仓库根目录打开工作区。`.vscode/settings.json` 已按 [Tinymist 的多文件配置](https://myriad-dreamin.github.io/tinymist/feature/compiler-settings.html) 将检查入口固定为 `main.typ`，并使用相同的缓存字体；默认不自动导出 PDF。`sections/*.typ` 是章节片段，不能单独编译：它们依赖总入口中的标题编号、全局样式和其他章节标签，单独检查会出现“无法引用未编号标题”或“标签不存在”。已有编辑器会话若仍显示这些错误，执行 `Developer: Reload Window` 重新载入工作区配置。

`ICPC-Notebook.pdf`、校验文件和字体缓存均不提交到 Git。CI 直接编译 `main.typ`，章节及所有排版以此入口为准；尚未启用的章节不会自动加入 PDF。

## PDF 自动构建与下载

工作流：[Build Notebook PDF](https://github.com/tong-qiiii/ICPC-Notebook/actions/workflows/build-notebook.yml)。将本次配置提交并推送到 `main` 后生效。

| 场景 | 触发方式 | 产物位置 |
| --- | --- | --- |
| 日常构建 | 每次 push 到 `main` | 对应 Actions 运行页面的 Artifacts，保留 30 天 |
| 手动构建 | Actions → Build Notebook PDF → Run workflow，选择分支或标签 | 对应运行的 Artifacts；不会创建 Release |
| 正式比赛版本 | 推送 `notebook-v*` 标签，例如 `notebook-v1.0.0` | 对应运行的 Artifacts，以及 GitHub Releases |

日常下载：打开对应提交的成功运行，在 **Artifacts** 下载 `ICPC-Notebook-<run_id>-<run_attempt>`，解压得到 `ICPC-Notebook.pdf` 和 `ICPC-Notebook.pdf.sha256`。运行摘要也提供本次产物链接、源提交 SHA、Typst 版本及 PDF 校验值。Actions 产物下载需要登录 GitHub，过期后可对所需版本手动重新构建，或下载正式版本。

正式下载：[Releases 页面](https://github.com/tong-qiiii/ICPC-Notebook/releases)，或 [最新正式 PDF](https://github.com/tong-qiiii/ICPC-Notebook/releases/latest/download/ICPC-Notebook.pdf)。首次成功发布后该直链才可用。日常 push 和手动构建不会新增 Release。

## 发布正式比赛版本

先确认所选 `main` 提交已经构建成功并检查过 PDF，再由维护者手动创建并推送一个新标签（以下只是操作示例，自动工作流不会执行 Git 提交或推送）：

```sh
git switch main
git pull --ff-only
git tag -a notebook-v1.0.0 -m "ICPC Notebook 1.0.0"
git push origin notebook-v1.0.0
```

请为每个正式版本使用新的 `notebook-v*` 标签，并让该标签指向包含工作流、字体脚本和所有章节源码的提交。标签构建编译标签所指源码；构建及校验成功后，发布作业只下载**本次运行**的产物，先上传到草稿 Release，再公开发布并标记为 Latest。上传中断时可重新运行失败作业，继续完成草稿；已公开的同名版本不会被覆盖，需要发布新标签。不要提前在网页创建或公开同标签的 Release。

默认构建仅使用 `contents: read`。只有标签发布作业申请 `GITHUB_TOKEN` 的 `contents: write`，用于创建 Release 和上传附件（[GitHub 权限说明](https://docs.github.com/en/actions/reference/workflows-and-actions/workflow-syntax#permissions)），不需要额外 PAT，也不修改仓库权限设置。若组织策略禁止此权限，发布作业会明确失败；日常 PDF 构建仍可使用。

确认成功时，检查 Actions 运行显示绿色 **Success**，`Build PDF` 作业中的字体准备、编译和上传步骤全部成功；正式发布还应检查 `Publish contest PDF` 成功且 Release 的 Assets 中存在 PDF。构建前删除旧输出，编译失败会直接令工作流失败，上传步骤不会运行；缺失产物也会报错，发布作业不会使用其他运行的 PDF。

下载后可校验附件：Linux 使用 `sha256sum --check ICPC-Notebook.pdf.sha256`；PowerShell 使用 `Get-FileHash ICPC-Notebook.pdf -Algorithm SHA256`，与 `.sha256` 文件中的值对照。最终仍应打开 PDF 检查中文、代码换行及分页后再用于比赛。

## 添加新算法模板

每次按章节或算法逐步维护：

1. 阅读 [AGENTS.md](AGENTS.md)、[STYLE.md](STYLE.md) 和 [TYPST_STYLE.md](TYPST_STYLE.md)，确定算法分类、接口、复杂度和适用条件，并继承统一排版。参考 `raw/` 时保持原文件不变。
2. 在 `code/<topic>/` 下添加小写下划线命名的 `.hpp` 或 `.cpp` 文件，保证兼容 C++17。
3. 在 `test/<topic>/` 下添加对应的 `<algorithm>_test.cpp`，覆盖常见输入和边界条件，并按需要对拍。
4. 使用支持 C++17 的编译器验证模板，例如编译相关测试时使用 `g++ -std=c++17 -O2 -Wall -Wextra`，随后运行测试。
5. 在 `sections/NN_topic.typ` 中添加算法说明和源码引用。使用 `styles/source.typ` 的 `source_code` 展示自身文件；共享骨架集中在章首完整展示一次，其余依赖用稳定标签链接到已有算法，禁止每个代码块重贴整条依赖链。
6. 使用 `== 算法名称 <algo-stable-id>` 在章节的期望位置插入算法；新建章节使用 `= 章节名称 <chapter-stable-id>`，在 `main.typ` 的期望位置导入。三级 / 四级标题用于无编号辅助说明，具体规则见下方。
7. 编译 Notebook，检查 PDF 排版及 `git diff`、`git status`，确认 `raw/` 没有变化后再按需提交。

## 自动编号与交叉引用

在所需位置插入带稳定标签的标题，不写数字：

```typst
== 倍增 LCA <algo-tree-lca>

=== 接口说明
// 补充用途、复杂度、下标、调用、限制和现场记忆，并 read() 引用源码。
```

上例标签已存在；新增算法请选一个全局唯一的新标签。以后改标题、移动算法或调整章节顺序，保留该标签。引用使用 `@algo-tree-lca`；仅需编号时写 `#ref(<algo-tree-lca>, supplement: none)`。Typst 编译时同时更新正文、目录和引用，无需运行重新编号命令，也无需修改其他算法或 C++ 文件。

一级标题是章节、二级标题是独立算法；三级 / 四级标题是无编号的接口、分类和组内变体。原稿中的无编号独立算法现也参与连续编号，原稿来源编号仅用于历史追溯。章节顺序由 `main.typ` 的 include 位置决定，文件名保持不变。删除被引用算法后，需要删除或修正对应引用；悬空引用会使构建失败。

编号回归测试：

```sh
python test/typst/numbering_test.py
# 可选的逐场景 PDF 检查需要安装 PyMuPDF：
python test/typst/numbering_test.py --pdf-check
```

测试需要与正文相同的 Typst 和字体配置；可用 `TYPST` 环境变量指定编译器路径。所有插入、交换、删除、换章及文件改名仅在临时副本中执行，不改写当前项目。详细规则见 [TYPST_STYLE.md](TYPST_STYLE.md#自动编号与稳定引用)。

## 基础工具验证

安装 Python 3 和支持 GNU++17 的 GCC 后，在仓库根目录执行：

```sh
python test/basic/run_tests.py
```

脚本会检查头文件独立编译、章节调用示例及多个翻译单元链接，并分别运行提交模式和 `-DLOCAL` 模式的边界与随机测试。可通过 `CXX` 环境变量指定编译器。可执行文件和对拍输入输出仅写入临时目录，不污染仓库。

实数二分、整数三分和实数三分采用函数式接口；三分默认求最小值，传 `greater<>` 求最大值。测试包含整数三分暴力对拍、实数搜索已知答案、端点与最优平台、整数极值、浮点停滞，以及个人基础模板兼容和新增调用示例的复制运行。二分要求判定单调；三分要求两侧严格单调，允许最优值平台，不能直接用于任意阶梯函数。

## 图论验证与比赛使用

图论章节遵循 [STYLE.md](STYLE.md) 的个人基础模板和码风。原文 34 个小节拆为 37 个独立模板：Floyd 与传递闭包分别维护，带权并查集分为加法、XOR、模 k 三版。完整拆分、接口及限制见 `sections/02_graph.typ`。

```sh
python test/graph/run_tests.py
```

脚本验证每份头文件独立编译、个人基础模板宏兼容、样例与边界、多次调用、随机或穷举对拍，并编译运行“公共骨架＋所需基础算法＋当前算法＋调用”的全部复制示例。检查包含 `-std=c++17` 与 GNU++17 两种模式，生成产物只写入临时目录。

比赛时从 PDF 复制个人基础模板及所需算法实现，无需引用仓库头文件。DSU、Dinic 等公共骨架只在章首完整展示一次；各算法通过链接注明依赖，自身代码块不重复打印骨架。组合多份算法时，共用实现只复制一次。两版费用流分别使用 `MCMF` 与 `MCMFPotential`，可以同时包含。

## 树论验证与比赛使用

树论保留 `raw/03_tree.md` 的全部 9 份轻量 struct，并在章首补充 3 份函数式模板：两次 DFS 求直径、一次树形 DP 求直径、求全部树重心，共 12 份。两次 DFS 要求非负边权；DP 允许负边和零边路径；重心按节点数计算。原有算法相对顺序保留，显示编号自动更新。源码、接口、复杂度、最小调用示例、适用边界及现场记忆分别见 `code/tree/` 和 `sections/03_tree.typ`。

```sh
python test/tree/run_tests.py
```

脚本在 C++17 独立头文件模式和 GNU++17 个人基础模板模式下，检查全部模板、边界、多次初始化、换根和随机对拍；另编译运行 12 份 Notebook 调用示例，并检查多翻译单元链接。直径对全源路径枚举，重心对逐点删除后的连通块枚举进行对拍。使用断言、标准库边界检查及 `-ftrapv`，中间文件仅写入系统临时目录。

本章保留递归 DFS，深链应评估栈空间；LCT 仍为迭代 Splay。点分治内部使用 GCC `__int128` 累加距离，避免 `ll` 边权累加溢出；其余权值运算的 `ll` 范围约束见对应模板说明。PDF 展示的模板已包含标准头文件及 `ll` 兜底宏，移除的只有维护用 `#pragma once`，无需引用仓库头文件。

## Linux 本地编译与调试

第一章提供 `code/basic/run` 与 `code/basic/debug` 两个无后缀 Bash 脚本。复制到题目目录，准备好 `a.cpp`、`a.in` 后执行：

```bash
chmod +x run debug
./run a
./debug a
```

答案写入 `a.out`，报错和计时显示在终端。调试版启用 ASan / UBSan 和 STL 调试检查；编译参数、栈限制及使用边界见第一章。脚本使用 LF 换行；也可直接 `bash run a`。只检查这两份脚本时运行 `python test/basic/run_debug_test.py`，不进行性能测试。

## 全项目验证

```sh
python test/run_all.py
```

第 4～10 章也可分别运行 `python test/<topic>/run_tests.py`，topic 为 `data_structure`、`interval_greedy`、`math`、`geometry`、`poly`、`string`、`dp`。新测试检查独立 C++17 编译、个人基础模板宏兼容、多翻译单元链接、基础样例、边界及适用的随机对拍；`test/notebook_sources.py` 检查公共骨架去重与依赖链接，`test/check_examples.py` 按实际依赖顺序拼接 PDF 中分开展示的代码并编译运行最小调用示例（也覆盖图论，可用 `geometry poly` 等主题参数只检查相关章）。`test/check_manifest.py` 核对原稿条目、文件对应关系、原始哈希及章节完整且不重复导入，允许调整导入顺序；原稿以外的新增模板在清单的 `additions` 中单列。

数据结构章补充了 Beats（区间取小 / 取大 / 加法与和、最值查询）、zkw 单点修改版和 zkw 区间加 / 和版。Beats 按节点信息与树框架分层；zkw 使用数值数组，区间版采用永久标记，无 Info/Tag 层。测试覆盖混合操作暴力对拍、极值及次值、非二次幂长度、祖先标记和多测重建。各版同名 `Seg` 独立选用，接口和赛场修改位置见章节说明。

需 GCC（支持 `bits/stdc++.h` 与 `__int128`）、Python 3，以及高精度模板使用的 Boost.Multiprecision 头文件。Boost 不在默认搜索路径时，Linux / macOS 可设置 `CPLUS_INCLUDE_PATH=/path/to/boost`，PowerShell 可设置 `$env:CPLUS_INCLUDE_PATH = 'D:/path/to/boost'`，该目录下应存在 `boost/multiprecision/cpp_int.hpp`。不必链接 Boost 二进制库。可通过 `CXX` 指定 GCC；测试产物放入临时目录，Python 缓存由 Git 忽略。

公共骨架集中在所属章首，算法代码只展示自身部分，并链接到需要先复制的骨架或基础算法；数学与概率等跨章依赖也复用同一处代码。所有说明、示例和原算法标题保留。组合多个模板时，共用实现只复制一次；同名 `Info`、`Tag`、`Seg`、`Node` 等独立版本按题选择，不要求它们能在同一作用域同时包含。莫队、数位 DP 等回调骨架仍需实现题意维护；WQS、分治 / Knuth 优化的数学条件以及浮点几何误差应按实际题目复核，测试通过不替代这些前提。


## 概率与期望专题

第 11 章位于 `sections/11_probability.typ`，包含 15 个主题、13 份 C++17 模板：条件概率、指示变量与方差、常见分布、独立成功次数 DP、固定步数游走、自环与逆拓扑期望、后缀和优化、Sushi 状态压缩、吸收马尔可夫链、一维与树上随机游走、收集齐、随机停止长度、连续段矩以及模意义期望。每个可执行模板均提供最小调用、限制和现场记忆；例题含自拟小模型与 AtCoder 官方题目链接。

```sh
python test/probability/run_tests.py
python test/check_examples.py probability
```

验证包含枚举、尾概率求和、容斥和独立线性方程对照，以及 Coins、Sushi、Sugoroku 3/4、Critical Hit 的官方样例。`test/run_all.py` 已自动覆盖该专题；模板在 `test/migration_manifest.json` 中以新增章节登记，不伪造原始备份。吸收链通过图可达性区分无限期望，但高斯精度仍受已有 `GaussD::EPS` 限制；近奇异、小逃逸概率、无限状态过程及新题建模需另行判断。默认不导出本地完整 PDF，正式版本由 GitHub Actions 构建。


## 组合计数与反演专题

第 12 章位于 `sections/12_counting.typ`，按“对象定义、公式、推导、例子、适用边界”整理 40 个主题，覆盖十二重计数、隔板与有界拆分、容斥与错排、Stirling/Bell/Lah/Eulerian 数、Catalan/Ballot/Fuss–Catalan、整数分拆、生成函数、二项式/子集/约数/倍数反演、φ 加权反演与欧拉变换、GCD/LCM 求和、Burnside/Pólya、非周期串、标号树、矩阵树/LGV、有限域、Hook-length 与染色多项式。

参考 [thedyingkai 的仓库](https://github.com/thedyingkai/XCPC-Template-by-thedyingkai_/tree/04355d0a0ac07913df2629fbfa6f12ef5eb6958e/组合计数)（固定提交 `04355d0`），并在章节中给出 NIST、OEIS 与 MIT 的相关来源。已有 Comb、Lucas、μ/φ 筛、互质计数、FPS 等实现通过稳定引用跳转，不重复展开源码；新增 7 份轻量 C++17 模板，各有独立测试与最小调用。

```sh
python test/counting/run_tests.py
python test/check_examples.py counting
```

公式测试使用精确整数 / 分数，包含小规模枚举、有限差分、容斥、反演、群轨道、路径族、树、有限域矩阵与 Young 图可移角对照。它验证的是明确有限范围的样例，不代替一般定理证明；新题仍须检查对象标号、边界、符号、群作用、LGV 配对条件及模分母。扩展 Lucas 与模素数行列式的可复制实现见数学章，须遵守对应模数和规模限制。默认不自动构建完整 PDF，也不提交、推送或发布。

## 常用模型补充

新增扫描线矩形面积并、线段树分治动态连通、Nim / SG、bitset 异或高斯、模素数行列式、矩阵树调用、区间树状数组、杜教筛、扩展 Lucas、LIS 恢复与树上背包；Hopcroft–Karp 增加最小点覆盖 / 最大独立集恢复。均放入已有章节，沿用自动编号与共享代码单次展示。补充清单见 `test/additions_manifest.json`。

本批补充的集中检查入口为 `python test/check_additions.py`，覆盖独立编译、个人宏、多翻译单元链接、随机 / 暴力对拍和复制调用示例；也已纳入各章 `test/<topic>/run_tests.py`。日常维护只运行与改动相关的编译和正确性测试。扫描线返回面积和杜教筛的 φ 前缀使用 GNU `__int128`；扩展 Lucas 按质数幂开表，不能忽略模数导致的内存成本。
