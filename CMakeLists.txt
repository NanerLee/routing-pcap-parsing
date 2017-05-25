# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.5)

# 项目名称
project(Routing-pcap-parsingp CXX)

# 版本号
set(Demo_VERSION_MAJOR 1)
set(Demo_VERSION_MINOR 0)

# 编译选项
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall -g -O0 -D DEBUG -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -Wall -O3 -std=c++11")

# 添加头文件的目录，路径是相对当前CMakeLists.txt文件的路径的
include_directories("./lib")

# 添加库的链接目录，路径是相对执行make的路径的
link_directories("./build/lib")

# 添加子目录
add_subdirectory(lib)
add_subdirectory(src)
