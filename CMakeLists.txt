cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
project(asm-analyze)

set(ICON_PATH "images/icon.ico")

add_executable(${PROJECT_NAME} WIN32
  main.cpp
  ${ICON_PATH}
)