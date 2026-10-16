# 自动算法编号实现与验证

## 实现

- 保留 `main.typ` 原生 `heading(numbering: "1.1")` 和目录，不增加自定义编号计数器、标题绘制宏或构建预处理。
- 全部十章统一用一级章节标题、二级算法标题；移除 113 处手动标题 counter 更新。原来无编号的笛卡尔树、六种主席树与 CDQ 共 8 个独立算法改为原生二级标题，按当前位置连续编号。
- 为 10 个章节和 177 个算法标题添加全局唯一的语义标签；标签是显式稳定标识，不含显示序号，不由文件路径实时计算。移动算法后保持标签，目录、书签和 ref 自动使用新编号。
- 三级 / 四级分类、接口、现场记忆和组内变体保持无编号、不入目录；196 份 hpp 不等于 196 个独立标题，同一算法组内的变体仍共用所属算法编号。
- 已有文内数字引用改为原生标签引用，正文共 25 处 `ref`，包含树论指向图论 Kruskal 的跨章节引用。来源信息明确标注原稿，历史编号不参与当前显示编号。
- 文件名继续作为维护地址，例如树论文件仍叫 `03_tree.typ`；调整章节顺序只移动入口 include，不改文件名或源码。
- `test/check_manifest.py` 改为检查章节完整且唯一导入，允许任意导入顺序；示例测试不再按文件名前两位数字选择章节。

依据 Typst 官方原生机制：[Heading](https://typst.app/docs/reference/model/heading/)、[Counter](https://typst.app/docs/reference/introspection/counter/)、[Reference](https://typst.app/docs/reference/model/ref/)。本项目不使用正则替换显示数字，也无需任何重新编号脚本。

## 实际验证

运行 `python test/typst/numbering_test.py --pdf-check`，使用 Typst 0.15.1 与项目固定字体。九个正常场景均对完整项目副本实际排版并生成 PDF，逐一核对全部标题的 counter 值、目录 / 书签编号、正文标题以及三个跨章 ref 的实际提取文本；第十个场景验证悬空引用应当失败。最终退出码 0。

| 场景 | 核对结果 |
| --- | --- |
| 基准 | 通过：10 章、177 算法，LCA 3.1，目录 / 正文 / 引用一致 |
| 章首插入临时算法 | 通过：LCA 从 3.1 自动顺延为 3.2 |
| 章中插入临时算法 | 通过：虚树从 3.5 自动顺延为 3.6 |
| 交换两个完整算法块 | 通过：树剖 LCA 显示 3.5，虚树显示 3.2；标签不变 |
| 删除未被引用的算法 | 通过：删除 HLD，虚树前移为 3.4，后续连续 |
| 跨章移动算法 | 通过：区间 DP 从 10.2 移至数学章成为 6.19，引用自动跟随，标签不变 |
| 交换章节顺序 | 通过：LCA 变为 6.1，数学 Mint 变为 3.1，跨章引用同时更新 |
| 临时改章节文件名 | 通过：数学章改为 math_reference.typ，显示编号、内容与引用不受文件名前缀影响 |
| 插入分类与接口辅助标题 | 通过：三级 / 四级辅助标题无编号、不入目录，不占用后续算法编号 |
| 删除被引用的算法 | 通过：删除 LCA 后，Typst 对 algo-tree-lca 悬空引用报错；测试确认该失败，不会将引用误指向下一项 |

所有临时插入、交换、删除、跨章移动、文件改名及探针均只存在于临时副本；测试 finally 中核对工作区入口、章节、raw 和 C++ 哈希，原工作区无测试残留。测试脚本仅验证编号，不生成或修改项目编号，日常编译不调用它。

最终各章算法标题数量（非 hpp 数）：

| 当前章节 | 算法标题数 | 当前编号范围 |
| --- | ---: | --- |
| 1 | 10 | 1.1–1.10 |
| 2 | 37 | 2.1–2.37 |
| 3 | 9 | 3.1–3.9 |
| 4 | 39 | 4.1–4.39 |
| 5 | 5 | 5.1–5.5 |
| 6 | 18 | 6.1–6.18 |
| 7 | 18 | 7.1–7.18 |
| 8 | 11 | 8.1–8.11 |
| 9 | 13 | 9.1–9.13 |
| 10 | 17 | 10.1–10.17 |

最终完整 PDF 编译成功，共 **195 页**、187 个目录 / 书签条目；无编号测试探针。A4、双栏、字体字号、代码背景、页边距、栏距、页码和封面布局均保留。原全局排版指令逐行比较一致；封面像素一致，物理 3～27 页（未改正文的工具 / 图论内容）像素一致。

已渲染全部页面总览并查看目录、章节过渡、新参与编号的算法、长代码、跨栏表和末页。未检测到代码越栏、乱码、空白正文页或丢失代码；末页继续自然留白。原稿来源文字和引用长度变化引起少量自然换行 / 分页位置移动，没有重新设计排版。

全部 **496 个源码 / 现场记忆块**与修改前逐项一致，并完整匹配 PDF 提取文本；196 份最小 C++ 调用片段也与修改前一致。全部 raw / code 文件（共 207 个，含 code/.gitkeep）SHA-256 不变，C++ 文件没有新增、删除、重命名或逻辑修改。本轮没有重跑整套 C++ 运行测试，验证范围为编号、PDF、清单及源码不变性。

另执行 `python test/check_manifest.py`、`git diff --check`，均通过。生成 PDF、测试 PDF、截图、日志及字体缓存均位于现有忽略路径，不提交到 Git。没有修改 GitHub Actions，正常 `typst compile main.typ ICPC-Notebook.pdf` 即使用自动编号；没有 commit、push 或 Release。

最终 PDF SHA-256：`a9fbdb9d0dd5cb4842e2f3708aca30ba4a33a2af9cf13c1dd57658bbf1d694a9`。

## 后续维护

1. 在章节的期望位置插入 `== 算法名称 <algo-unique-id>`，选择新的全局唯一语义标签；保留已有的用途、接口、示例和 `read()` 源码引用习惯。
2. 说明标题使用 `===` / `====`；同组变体不额外占算法编号，若要作为独立算法则用带稳定标签的 `==`。
3. 引用使用 `@algo-unique-id`，只显示编号使用 `#ref(<algo-unique-id>, supplement: none)`；不要写死当前数字。
4. 移动或修改标题时保留标签。章节换位只调整 main.typ 的 include 顺序，文件名和 C++ 源码无需因编号变化而修改。
5. 普通 Typst 编译自动更新所有数字、目录与引用。删除被引用目标时应删除或修正其引用，编译会检查悬空标签。

## 文件变更

- `main.typ`：记录原生编号分层约定，章节注释去掉位置数字；全局样式参数未改变。
- `sections/*.typ` 十份：统一标题、稳定标签、原稿来源和标签引用；源码块与调用代码不变。
- `AGENTS.md`、`TYPST_STYLE.md`、`README.md`：用内容 / 标签稳定、显示编号随位置变化的新规则替换旧编号保护规则，并给出维护示例及测试命令。
- `test/typst/numbering_test.py`：新增自动编号回归测试。
- `test/check_manifest.py`、`test/check_examples.py`：解除导入排序与数字文件名前缀对测试选择的限制。
- `acceptance_report.md`、`migration_report.md`：增加历史快照说明，指向当前编号机制，保留旧记录。
- `numbering_report.md`：本实现与验证报告。

当前 Git 状态包含本轮开始前已经存在的迁移成果，全部保留。最终 `git status --short --branch`：

```text
## main...origin/main
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

按文件展开：8 个已跟踪文件修改、304 个未跟踪文件，无暂存 / 删除 / 待提交 PDF。
