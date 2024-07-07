cmake_minimum_required(VERSION 3.10)

project(loginproject)

set(CMAKE_CXX_STANDARD 11)

find_package(SQLite3 REQUIRED)
include_directories(${SQLite3_INCLUDE_DIRS})

add_executable(loginproject ../main.cpp)
target_link_libraries(loginproject ${SQLite3_LIBRARIES})

