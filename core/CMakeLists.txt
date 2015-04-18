# ######################################################################### #

cmake_minimum_required(VERSION 2.8.8)

# Project name
set(PROJECT_NAME core)

# Create project
project(${PROJECT_NAME} CXX)

# Current version is unable to add object library into shared library.
if (UNIX)
    add_compile_options(-fPIC)
endif ()

# Create library
add_library(${PROJECT_NAME} OBJECT
    constants.hpp
    Matrix.hpp
    Units.hpp
    Vector.hpp
    Vector.cpp
    AlignedAllocator.hpp
    AlignedAllocator.cpp
    Log.hpp
    Log.cpp
)

# ######################################################################### #
