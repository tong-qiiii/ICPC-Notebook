# ICPC Notebook

用于整理 ICPC 竞赛算法模板、说明和验证用例，并使用 Typst 编译为便于查阅的 PDF Notebook。

已迁移基础工具、图论和树论三章，对应 `sections/01_tools.typ`、`sections/02_graph.typ`、`sections/03_tree.typ` 及 `code/basic/`、`code/graph/`、`code/tree/`；其余章节仍保留导入占位。`raw/` 永久保存原始 Markdown 备份，不修改其中任何文件。

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
├── sections/       # 已迁移工具、图论、树论三章
├── code/           # GNU++17 模板：basic/、graph/、tree/
└── test/           # 对应 basic/、graph/、tree/ 的测试和对拍工具
```

三个目录保留初始化时的 `.gitkeep`。

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

`main.typ` 已启用基础工具、图论和树论，其他七章仍是注释导入占位。先创建相应章节文件，再取消对应 `#include` 行前的 `//`。章节路径和导入规则见 [Typst 官方文档](https://typst.app/docs/reference/scripting/#modules)。

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
5. 在 `sections/NN_topic.typ` 中添加算法说明和源码引用。代码引用路径相对于章节文件，指向 `../code/<topic>/<algorithm>.hpp` 或 `.cpp`，使用 `read` 配合 `raw` 展示源码。
6. 若章节文件是新建的，在 `main.typ` 中启用对应导入；新增分类时按编号顺序补充导入。
7. 编译 Notebook，检查 PDF 排版及 `git diff`、`git status`，确认 `raw/` 没有变化后再按需提交。

## 基础工具验证

安装 Python 3 和支持 GNU++17 的 GCC 后，在仓库根目录执行：

```sh
python test/basic/run_tests.py
```

脚本会检查头文件独立编译、章节调用示例及多个翻译单元链接，并分别运行提交模式和 `-DLOCAL` 模式的边界与随机测试。可通过 `CXX` 环境变量指定编译器。可执行文件和对拍输入输出仅写入临时目录，不污染仓库。

## 图论验证与比赛使用

图论章节遵循 [STYLE.md](STYLE.md) 的个人基础模板和码风。原文 34 个小节拆为 37 个独立模板：Floyd 与传递闭包分别维护，带权并查集分为加法、XOR、模 k 三版。完整拆分、接口及限制见 `sections/02_graph.typ`。

```sh
python test/graph/run_tests.py
```

脚本验证每份头文件独立编译、个人基础模板宏兼容、样例与边界、多次调用、随机或穷举对拍，并编译运行章节中展开依赖后的全部调用示例。检查包含 `-std=c++17` 与 GNU++17 两种模式，生成产物只写入临时目录。

比赛时从 PDF 复制个人基础模板及所需算法实现，无需引用仓库头文件。DSU、Dinic 等依赖会在相应模板处完整展示；组合多份算法时，共用实现只复制一次。两版费用流分别使用 `MCMF` 与 `MCMFPotential`，可以同时包含。

## 树论验证与比赛使用

树论完整对应 `raw/03_tree.md` 的 3.1-3.9，保留 9 个独立轻量 struct：倍增 LCA、树剖 LCA、Tarjan 离线 LCA、HLD、虚树、DSU on Tree、点分治、Kruskal 重构树和 LCT。源码、接口、复杂度、最小调用示例、适用边界及现场记忆分别见 `code/tree/` 和 `sections/03_tree.typ`，顺序与原稿一致。

```sh
python test/tree/run_tests.py
```

脚本在 C++17 独立头文件模式和 GNU++17 个人基础模板模式下，检查全部模板、边界、多次初始化、换根和随机对拍；另编译运行 9 份 PDF 调用示例，并检查多翻译单元链接。使用断言、标准库边界检查及 `-ftrapv`，中间文件仅写入系统临时目录。

本章保留递归 DFS，深链应评估栈空间；LCT 仍为迭代 Splay。点分治内部使用 GCC `__int128` 累加距离，避免 `ll` 边权累加溢出；其余权值运算的 `ll` 范围约束见对应模板说明。PDF 展示的模板已包含标准头文件及 `ll` 兜底宏，移除的只有维护用 `#pragma once`，无需引用仓库头文件。
