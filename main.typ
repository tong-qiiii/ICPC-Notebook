// Layout source: icpc_template_dynamic_10_twocol.typ (see TYPST_STYLE.md).
// Keep all shared typography here; sections contain content and source references.

// ============================================================
// ICPC / XCPC Algorithm Template
// Dynamic chapters · A4 two-column edition
// ============================================================

// ---------------- Body typography ----------------
#set text(
  font: (
    "Noto Serif CJK SC",
    "New Computer Modern",
  ),
  size: 7.4pt,
  lang: "zh",
)

#set par(
  leading: 0.24em,
  spacing: 0.18em,
  justify: true,
)

#set raw(tab-size: 4)

#show raw.where(block: true): block.with(
  fill: luma(248),
  inset: 2.6pt,
  radius: 1.4pt,
  stroke: luma(230),
)
#show raw.where(block: true): set text(size: 5.7pt)

#show raw.where(block: false): box.with(
  fill: luma(247),
  inset: (x: 1.1pt, y: 0pt),
  outset: (y: 0.4pt),
  radius: 1pt,
)

#show table: set text(size: 6.35pt)
#show quote: set text(size: 6.45pt)
#show quote: set pad(x: 0.35em)

#set heading(numbering: "1.1")
#show heading.where(level: 3): set heading(numbering: none, outlined: false)
#show heading.where(level: 4): set heading(numbering: none, outlined: false)

#show heading.where(level: 1): set text(size: 12pt, weight: "bold")
#show heading.where(level: 2): set text(size: 8.8pt, weight: "bold")
#show heading.where(level: 3): set text(size: 7.5pt, weight: "bold")
#show heading.where(level: 4): set text(size: 6.9pt, weight: "bold")

// Wide table figures may need to break if they become tall.
#show figure.where(kind: table): set block(breakable: true)

// ============================================================
// Cover — independent A4 page, no page number
// ============================================================
#set page(
  paper: "a4",
  margin: (
    top: 16mm,
    bottom: 16mm,
    left: 18mm,
    right: 18mm,
  ),
  columns: 1,
  numbering: none,
)

#align(center + horizon)[
  #text(
    font: "New Computer Modern",
    size: 7.5pt,
    tracking: 0.18em,
    fill: luma(105),
  )[ICPC ALGORITHM TEMPLATE]

  #v(25pt)
  #line(length: 22%, stroke: 0.45pt + luma(188))
  #v(30pt)

  // Prefer one complete calligraphic CJK family for the whole sentence.
  // Pin the reference's first choice locally and in CI; do not substitute it.
  #text(
    font: (
      "LXGW WenKai",
    ),
    size: 22pt,
    weight: "regular",
    tracking: 0.045em,
  )[你说再见面的话 是落叶还是飘雪]

  #v(15pt)

  #text(
    font: "New Computer Modern",
    size: 18pt,
    style: "italic",
    weight: "regular",
    tracking: 0.06em,
  )[Leave or Flurry]

  #v(31pt)
  #line(length: 22%, stroke: 0.45pt + luma(188))
]

#pagebreak()

// ============================================================
// TOC + body — true A4 two-column layout
// ============================================================
#set page(
  paper: "a4",
  margin: (
    top: 8mm,
    bottom: 10mm,
    left: 8mm,
    right: 8mm,
  ),
  columns: 2,
  numbering: "1",
  number-align: center + bottom,
)

#set columns(gutter: 5mm)
#counter(page).update(1)

#outline(
  title: [目录],
  depth: 3,
)

#pagebreak()


// 01 工具
#include "sections/01_tools.typ"

// 02 图论
#include "sections/02_graph.typ"

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
