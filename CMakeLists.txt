cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
project(asm-analyze)

set(ICON_PATH "images/icon.ico")

add_executable(${PROJECT_NAME}
  main.cpp
)

if(WIN32)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    RESOURCE_FILE ${ICON_PATH}
  )
endif()