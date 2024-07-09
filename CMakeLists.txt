cmake_minimum_required(VERSION 3.10)

# Project Name
project(project)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SQLite3 library
find_package(SQLite3 REQUIRED)

# Add executable targets for each part of the project
add_executable(register register.cpp)
add_executable(login login.cpp)
add_executable(bmi_calculator bmi_calculator.cpp)
add_executable(planning planning.cpp)

# Link SQLite3 library to the executables
target_link_libraries(register SQLite::SQLite3)
target_link_libraries(login SQLite::SQLite3)
target_link_libraries(bmi_calculator SQLite::SQLite3)
target_link_libraries(planning SQLite::SQLite3)

