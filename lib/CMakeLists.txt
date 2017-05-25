# 查找目录下的所有源文件，路径为相对本文件的路径
aux_source_directory(. LIB_SRCS)

# 指定库文件的生成目录，也可以通过添加子目录里设置
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# 生成库文件
add_library(parsing SHARED ${LIB_SRCS})
