cmake_minimum_required(VERSION 3.30.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(asm-analyze)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(${PROJECT_NAME} main.cpp)

# optimization, -fdce just for fun
set(CMAKE_CXX_FLAGS_RELEASE "-finline-functions -fdce -fipa-cp -fipa-icf -Wall -Wdeprecated -O3 -march=native -mtune=native")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

set(CMAKE_BUILD_TYPE Release)
