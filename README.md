# ICPC Notebook

用于整理 ICPC 竞赛算法模板、说明和验证用例，并使用 Typst 编译为便于查阅的 PDF Notebook。

当前仅完成项目结构初始化：尚未迁移算法，也未生成算法代码。`raw/` 永久保存原始 Markdown 备份，不修改其中任何文件。

## 目录结构

```text
ICPC-Notebook/
├── AGENTS.md       # 项目维护规则
├── README.md       # 项目说明
├── main.typ        # Typst 总入口
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
├── sections/       # 后续 Typst 章节
├── code/           # 后续 C++17 模板
└── test/           # 后续测试和对拍工具
```

三个新目录各包含一个空的 `.gitkeep`，用于让 Git 保留目录结构。

## 编译 Typst

先安装 [Typst CLI](https://github.com/typst/typst#installation)，并确保 `typst` 在 `PATH` 中。然后在仓库根目录执行：

```sh
typst --version
typst compile main.typ notebook.pdf
```

开发时可以监听文件变更并自动重新编译：

```sh
typst watch main.typ notebook.pdf
```

`main.typ` 当前仅包含十个章节的注释导入占位，不展示正文。先创建相应章节文件，再取消对应 `#include` 行前的 `//`。章节路径和导入规则见 [Typst 官方文档](https://typst.app/docs/reference/scripting/#modules)。

`notebook.pdf` 是本地生成产物，默认无需提交。包含中文正文后，请确保编译环境具备所选中文字体。

## 添加新算法模板

以下步骤用于后续内容维护，当前初始化阶段不执行：

1. 阅读 [AGENTS.md](AGENTS.md)，确定算法分类、接口、复杂度和适用条件。参考 `raw/` 时保持原文件不变。
2. 在 `code/<topic>/` 下添加小写下划线命名的 `.hpp` 或 `.cpp` 文件，保证兼容 C++17。
3. 在 `test/<topic>/` 下添加对应的 `<algorithm>_test.cpp`，覆盖常见输入和边界条件，并按需要对拍。
4. 使用支持 C++17 的编译器验证模板，例如编译相关测试时使用 `g++ -std=c++17 -O2 -Wall -Wextra`，随后运行测试。
5. 在 `sections/NN_topic.typ` 中添加算法说明和源码引用。代码引用路径相对于章节文件，指向 `../code/<topic>/<algorithm>.hpp` 或 `.cpp`，使用 `read` 配合 `raw` 展示源码。
6. 若章节文件是新建的，在 `main.typ` 中启用对应导入；新增分类时按编号顺序补充导入。
7. 编译 Notebook，检查 PDF 排版及 `git diff`、`git status`，确认 `raw/` 没有变化后再按需提交。
