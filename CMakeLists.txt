cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(asm-analyze)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(${PROJECT_NAME} main.cpp)

set(ARGS "-fomit-frame-pointer -finline-functions -pedantic -Wall -Wextra -Wunused -Wdeprecated -O3 -march=native -mtune=native")
if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(CMAKE_CXX_FLAGS_RELEASE "${ARGS}")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-fdce -fipa-cp -fipa-icf ${ARGS}")
endif()

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

set(CMAKE_BUILD_TYPE Release)
