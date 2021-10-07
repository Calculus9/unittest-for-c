import cffi
"""
1.通过cffi跑通C语言
"""

ffi = cffi.FFI()  # 生成cffi实例

code_def = open(("test.h")).read()
code_source = open(("test.cpp"),encoding="utf-8").read()
# 函数声明
ffi.cdef(code_def)
# 函数定义
ffi.set_source("_export", code_source)
ffi.compile()
