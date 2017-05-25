# 指定可执行文件的生成目录，也可以通过添加子目录里设置
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)

# 生成可执行文件
add_executable(print print.cc)

# 链接库文件
target_link_libraries(print parsing)
