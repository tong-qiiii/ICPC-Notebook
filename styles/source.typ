// Each visible block owns one source file. Shared code is printed at chapter start.
#let source_code(files, shared: none) = {
  assert(files.len() == 1, message: "源码块只展示自身文件，依赖请链接到公共骨架或对应算法。")
  let file = files.first()
  if shared != none {
    link(shared)[源码见章首公共骨架，本节不重复展示。]
  } else {
    metadata((kind: "notebook-source", file: file))
    let source = read("../code/" + file)
      .replace(regex("(?m)^#pragma once\r?\n"), "")
      .replace(regex("(?m)^#include \".*\"\r?\n"), "")
    raw(source, lang: "cpp", block: true)
  }
}
