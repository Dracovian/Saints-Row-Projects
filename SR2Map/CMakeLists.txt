cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0054 NEW)
project(SR2Map LANGUAGES C)
set(CMAKE_C_STANDARD 90)

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)

include_directories(inc)
file(GLOB_RECURSE SRC src/*.c)
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME})