cmake_minimum_required(VERSION 3.30.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(asm-analyze)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(${PROJECT_NAME} main.cpp)

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    # Optimization flags for AppleClang (-fdce -fipa-cp -fipa-icf removed)
    set(CMAKE_CXX_FLAGS_RELEASE "-finline-functions -Wall -Wdeprecated -O3 -march=native -mtune=native")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-finline-functions -fdce -fipa-cp -fipa-icf -Wall -Wdeprecated -O3 -march=native -mtune=native")
endif()

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

set(CMAKE_BUILD_TYPE Release)
