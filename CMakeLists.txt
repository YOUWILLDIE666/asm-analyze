cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
project(asm-analyze)

set(ICON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/images/icon.ico")

add_executable(${PROJECT_NAME}
  main.cpp
  ${ICON_PATH}
)