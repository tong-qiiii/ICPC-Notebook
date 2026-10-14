// ICPC Notebook 总入口。仅启用已经迁移完成的章节。
// 创建其余 sections/*.typ 文件后，再取消对应导入行前的注释。

#set page(paper: "a4", margin: 16mm, numbering: "1")
#set text(size: 10pt)
#set par(leading: 0.5em)
#set heading(numbering: "1.1")
#show raw: set text(size: 8pt)
#show raw.where(block: true): it => block(
  breakable: it.text.split("\n").len() > 24,
  it,
)

// 01 工具
#include "sections/01_tools.typ"

// 02 图论
// #include "sections/02_graph.typ"

// 03 树
// #include "sections/03_tree.typ"

// 04 数据结构
// #include "sections/04_data_structure.typ"

// 05 区间与贪心
// #include "sections/05_interval_greedy.typ"

// 06 数学
// #include "sections/06_math.typ"

// 07 计算几何
// #include "sections/07_geometry.typ"

// 08 多项式
// #include "sections/08_poly.typ"

// 09 字符串
// #include "sections/09_string.typ"

// 10 动态规划
// #include "sections/10_dp.typ"
