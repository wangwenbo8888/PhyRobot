cmake_minimum_required(VERSION 3.10)

# 项目信息
project(XJC_SDK_Lib_USE)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

# 包含头文件路径
include_directories(${CMAKE_SOURCE_DIR}/include) # 包含文件（如 .h 文件）在 include/ 目录中

# 动态库路径
link_directories(${CMAKE_SOURCE_DIR}/lib) # 动态库名为 libmylibrary.so，存放在项目的 lib/ 目录中

# 可执行文件源文件
add_executable(XJC_SDK_Lib_USE src/XJC_SDK_Lib_USE.cpp)

# 链接动态库
target_link_libraries(XJC_SDK_Lib_USE XJC_SDK_Lib)

# 设置 RUNTIME 路径以确保运行时能找到动态库
set_target_properties(XJC_SDK_Lib_USE PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_target_properties(XJC_SDK_Lib_USE PROPERTIES INSTALL_RPATH ${CMAKE_SOURCE_DIR}/lib)

# 调试信息（可选）
message(STATUS "Include directories: ${CMAKE_SOURCE_DIR}/include")
message(STATUS "Library directories: ${CMAKE_SOURCE_DIR}/lib")