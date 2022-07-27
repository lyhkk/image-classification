cmake_minimum_required(VERSION 3.20.0)
project(STATION VERSION 0.1.0)

include(CTest)
enable_testing()

# 设置C/C++标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-std=c99)

add_executable(STATION main.c)

set(CMAKE_C_COMPILER "gcc")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
