cmake_minimum_required(VERSION 3.10)

project(asm-analyze)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(${PROJECT_NAME} main.cpp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET v PROPERTY CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

##########
set(ARGS "-fpic -fprofile-generate -funroll-loops -fomit-frame-pointer -finline-functions -pedantic -Wall -Wextra -Wunused -Wdeprecated -O3 -march=native -mtune=native")
set(CARGS "-fpeel-loops -fdce -fipa-cp -fipa-icf ${ARGS}")
##########

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_RELEASE "-malign-data -fprefetch-loops ${CARGS}")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_RELEASE "${CARGS}")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${ARGS}")
endif()

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

set(CMAKE_BUILD_TYPE Release)
