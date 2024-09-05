cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(asm-analyze)

add_executable(${PROJECT_NAME} main.cpp)

# optimization
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -mtune=native")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

set(CMAKE_BUILD_TYPE Release)