# Minimum required version of CMake
cmake_minimum_required(VERSION 3.5)

# Project name
project(MyProject)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(my_project_executable
    quine_mcluskey.cpp
    main_functions.cc
    main_functions.hpp
)


