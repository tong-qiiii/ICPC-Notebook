# ICPC Notebook Typst 排版规范

本项目唯一视觉基准是用户提供的 `icpc_template_dynamic_10_twocol.typ`（消息中称 `icpc_template_dynamic_10_twocol(1).typ`）。实际读取文件 SHA-256：`c5347a45e610d6b1acf375eb603138795f48731d543e93f8cff459d63d91e189`。已扫描完整文件的 23148 行，以下参数来自其实际排版指令，不从算法内容推测样式。

`main.typ` 是可执行的统一样式入口。本规范和该入口共同固定参考文件的视觉风格；以后所有新增章节、算法、表格及源码引用必须继承，不得由 Agent 自行重新设计。只有用户明确要求调整排版时才能变更这些规则。

## 纸张、封面、目录和页码

| 项目 | 固定规则 |
| --- | --- |
| 纸张 | 所有页面均为 A4，210 × 297 mm |
| 封面 | 独立单栏页；上下边距 16 mm，左右边距 18 mm；无页码 |
| 封面整体 | `align(center + horizon)`，整组水平、垂直居中 |
| 顶部英文 | `ICPC ALGORITHM TEMPLATE`；New Computer Modern，7.5 pt，字距 0.18 em，灰度 105 |
| 上方横线 | 上方留白 25 pt；宽 22%，线宽 0.45 pt，灰度 188；下方留白 30 pt |
| 中文主句 | `你说再见面的话 是落叶还是飘雪`；LXGW WenKai，22 pt，regular，字距 0.045 em |
| 副标题 | 主句后留白 15 pt；`Leave or Flurry`；New Computer Modern，18 pt，italic、regular，字距 0.06 em |
| 下方横线 | 副标题后留白 31 pt；与上方横线相同 |
| 目录和正文 | 上边距 8 mm、下边距 10 mm、左右边距 8 mm；真正的页面双栏，栏间距 5 mm，每栏宽 94.5 mm |
| 页码 | 封面后重置为 1，从目录开始按阿拉伯数字连续编号；页底居中 |
| 目录 | 标题为“目录”，`outline(depth: 3)`；沿用 Typst 原生目录、缩进、点引线与页码链接，不额外重画 |
| 换页 | 仅封面后和目录后强制换页；章节、算法连续排版，由 Typst 自然分栏分页 |

章节和算法继续使用 `1`、`1.1`、`2.1` 等原有视觉形式，显示数字由 Typst 原生 heading counter 根据实际位置生成，不在标题中手写。章节按 main.typ 导入顺序编号，二级算法在所属章节内连续编号。接口、调用、复杂度、注意事项和现场记忆等三级 / 四级标题保持无编号、不入目录；目录深度仍为 3，辅助标题通过 outlined:false 排除。

## 自动编号与稳定引用

不引入另一套计数器或标题绘制宏；沿用 main.typ 中 `heading(numbering: "1.1")` 与原生目录。所有章节统一采用以下结构：

```typst
= 树论 <chapter-tree>

== 倍增 LCA <algo-tree-lca>

=== 接口说明
// 用途、复杂度、下标、调用和代码引用照常编写。

==== 同一算法的变体
// 无编号辅助标题，不增加算法序号。
```

- 标题只写名称；章节与二级算法必须有唯一标签，建议使用语义化的 `chapter-...`、`algo-...`。标签中的主题只作命名空间，不是当前位置；算法换章后也保留原标签。
- 插入、删除或移动完整算法块时，无需修改其余标题。原稿无编号的笛卡尔树、主席树和 CDQ 也是独立算法，使用二级标题参与编号；原稿缺号不进入显示编号。
- 文内使用 `@algo-tree-lca` 或 `#ref(<algo-tree-lca>, supplement: none)` 引用；后者仅显示链接编号。引用章节同理。不得写死“算法 3.1”；原稿来源中的历史编号可以保留，但必须明确写成原稿来源，不能用作当前跳转。
- 标签不由文件路径、文件名前缀、中文标题或显示编号动态计算；标题、章节位置和源码路径调整后，保留既有标签。删除被引用算法时，应删除引用或改指仍存在的算法；Typst 对悬空标签报错，不自动重定向到下一个算法。
- 文件名是稳定的维护地址，当前 NN 前缀仅保留历史命名。更换章节顺序只移动 main.typ 的 include，不能手动设置标题 counter，也不需要运行重新编号脚本。
- 分类说明只用三级 / 四级标题；若某个变体成为独立算法，再使用带稳定标签的二级标题。算法组内多份源码仍可共用一个二级编号，不把每个 hpp 文件自动当作一个算法标题。
- 回归测试运行 `python test/typst/numbering_test.py`（Typst 在 PATH，或设置 TYPST 为可执行文件路径），在临时副本中验证插入、交换、删除、跨章移动、章节重排、辅助标题和标签引用。加 `--pdf-check` 时额外需要 PyMuPDF，逐场景验证 PDF 的目录、正文与引用。测试不改写工作区，不参与日常构建。

原生机制依据：[Heading](https://typst.app/docs/reference/model/heading/)、[Counter](https://typst.app/docs/reference/introspection/counter/)、[Reference](https://typst.app/docs/reference/model/ref/)。视觉参数与下文规则保持不变。

编辑器检查也必须以 `main.typ` 为入口。仓库的 `.vscode/settings.json` 使用 `tinymist.typstExtraArgs: ["main.typ"]` 固定入口；章节单独编译会丢失标题编号设置及跨章节标签。遇到此类诊断先检查编辑器入口，不要在章节内重复设置编号、复制其他章节标签或删除正确的交叉引用。字体配置仍使用统一缓存目录，不自动导出完整 PDF。

## 正文、标题和代码

| 项目 | 固定规则 |
| --- | --- |
| 中文及正文西文 | Noto Serif CJK SC 优先，New Computer Modern 回退；7.4 pt，语言 `zh` |
| 段落 | 行间附加距离 `leading: 0.24em`，段间距 `spacing: 0.18em`，两端对齐 |
| 一级标题 | 12 pt，bold |
| 二级标题 | 8.8 pt，bold |
| 三级标题 | 7.5 pt，bold |
| 四级标题 | 6.9 pt，bold |
| 标题间距 | 使用同版本 Typst 的原生标题间距和标题与后文黏连规则；原版没有额外设置标题上下间距 |
| 块代码 | 5.7 pt；保留 Typst 原生 raw 等宽字体 DejaVu Sans Mono；Tab 宽度为 4；语法高亮保持原生 C++ 样式 |
| 代码行距 | 原版未另设代码专用行距；保留上述段落设置及固定版本 Typst 的 raw 原生排版行为，不额外压缩 |
| 代码容器 | 背景 `luma(248)`，内边距 2.6 pt，圆角 1.4 pt，边框 `luma(230)`；允许跨栏、跨页自然拆分 |
| 行内代码 | 背景 `luma(247)`，横向 inset 1.1 pt、纵向 0；纵向 outset 0.4 pt；圆角 1 pt |
| 表格 | 6.35 pt |
| 引用说明 | 6.45 pt，横向 pad 0.35 em |

代码字号来自用户指定的参考文件，不允许为了减少页数继续缩小。禁止用 `scale`、裁切或固定高度容器掩盖溢出，也不得按算法或代码行数强制换页。长行的显示换行不得改动 `code/` 中源码；所有内容必须完整显示且可复制。

## 公共骨架与源码去重

PDF 中每份 C++ 源码只完整展示一次。DSU / Dinic、Mint、二维几何基础和 Poly / NTT 等公共骨架放在所属章节最前面的三级无编号小节，使用 `shared-...` 稳定标签；原算法标题、顺序、编号、接口、调用示例与现场记忆均保留。FPS、直线线段、凸包等既有算法被复用时，在自身条目展示一次，后续直接链接其位置。跨章复用 Mint、高斯消元等同理，不重新展示。

统一导入 `styles/source.typ`。普通代码块只列自身文件，不把依赖文件前置拼入；依赖用 `#link` / `#ref` 明确列齐。原骨架算法条目使用 `shared` 链接到章首代码，保留自身的最小调用示例：

```typst
#import "../styles/source.typ": source_code

=== 公共骨架 <shared-example-core>
#source_code(("topic/core.hpp",))

// 原基础算法条目中的源码位置：只跳转，不重复打印。
#source_code(("topic/core.hpp",), shared: <shared-example-core>)

// 其他算法的源码位置：说明需要先复制章首骨架。
依赖：#link(<shared-example-core>)[章首公共骨架]。
#source_code(("topic/algorithm.hpp",))
```

辅助标题不占用算法编号，也不进入目录。源码展示只去掉 `#pragma once` 与仓库本地 include，不改算法逻辑；读者按链接复制完整依赖后再复制当前算法，无需比赛现场访问仓库。测试通过实际 include 关系重建复制顺序，必须检查每份依赖确实已在前文展示、链接指向正确、没有遗漏或重复定义。

`python test/notebook_sources.py` 检查展示与依赖覆盖；`python test/check_examples.py` 编译运行拼接后的最小示例，支持按章节主题选择。新骨架必须增加稳定标签和相应引用，不允许通过全局隐藏代码、裁切、缩小字号或删算法来减少 PDF 页数。

## 表格与宽表

普通接口表沿用原文件的居中 `figure` + 双列表格，`inset: 6pt`，列对齐 `auto`。只有真正需要跨栏的宽表使用以下结构；参考文件的权值维护速查表即采用此方式，不切换整个文档为单栏：

```typst
#figure(
  placement: top,
  scope: "parent",
  align(center)[#table(
    columns: (0.55fr, 1.05fr, 1fr, 1.25fr, 1.25fr),
    align: (col, row) => (auto, auto, auto, auto, auto).at(col),
    inset: 3pt,
    [类型], [定义], [路径压缩], [合并], [查询],
    // 根据实际接口填写数据，不复制不适用的算法约定。
  )],
)
```

表格类 figure 允许 block 拆分，避免长表整块撑出页面。页顶浮动可能自然调整出现页码，应检查表格与前后说明的对应关系，不通过大量空行或额外分页强行定位。

工具与图论章节使用列表记录接口，不为模仿参考文件而重写这些算法说明。树论章节的接口速查继承上述普通双列表格。后续新增表格及真正需要跨栏的宽表应使用上述方式。

## 字体与 GitHub Actions

- Typst 固定为 **0.15.1**；内置 DejaVu Sans Mono、New Computer Modern 和数学字体，不依赖 Ubuntu 的系统字体包。
- 正文安装 **Noto Serif CJK SC 2.003 Regular / Bold**，封面安装 **LXGW WenKai 1.522 Regular**，均从官方固定版本下载并校验 SHA-256。
- 本地和 CI 共用 `python scripts/setup_fonts.py`，字体路径设为 `.cache/notebook-fonts`，并设置 `TYPST_IGNORE_SYSTEM_FONTS=true`。详见 README 的平台命令。
- 参考文件还列出 Source Han Serif SC、SimSun、Songti SC，以及 KaiTi、Kaiti SC、FandolKai 等回退字体。本项目固定安装首选完整字库，因此不依赖或要求安装这些未使用的回退字体，避免本地与 Ubuntu 选择不同字体。
- 不得因 CI 缺字体擅自替换字体、改变字号或栏宽；字体准备失败应让构建失败。

## 内容与验证要求

1. 章节放在 `sections/`，保留既有文件地址、算法内容和稳定标签；显示编号服从当前导入 / 排列顺序，允许自动变化。章节不设置全局页边距、字体、字号或栏数。
2. 算法实现仍以 `read()` 引用 `code/` 中源码，通过 `styles/source.typ` 展示自身实现；公共骨架只在章首完整展示一次，其他依赖链接到已展示的算法，不重复展开或维护第二份实现。保留用途、复杂度、下标、接口、调用、限制及现场记忆。
3. 不修改 `raw/`，不为改善排版修改 C++ 算法逻辑。
4. 排版变更后编译 `main.typ` 并实际渲染 PDF，检查封面、目录、中文字体、公式、长代码、表格、跨栏宽表和分页。
5. 对照本规范及参考样式检查正文和代码大小、栏宽、页边距、封面位置及页码；确认没有溢出、裁切、缺字或内容丢失。
6. 检查 `git diff`、`git status`，确认原始备份和算法源码未变。生成的 PDF、字体与验证中间文件不提交到 Git。
