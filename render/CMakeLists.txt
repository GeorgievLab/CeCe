# ######################################################################### #

cmake_minimum_required(VERSION 2.8.8)

# Project name
set(PROJECT_NAME render)

# Create project
project(${PROJECT_NAME} CXX)

# OpenGL library is required
find_package(OpenGL REQUIRED)

# Include OpenGL path
include_directories(
    ${OPENGL_INCLUDE_DIR}
)

# Current version is unable to add object library into shared library.
if (UNIX)
    add_compile_options(-fPIC)
endif ()

# Create library
add_library(${PROJECT_NAME} OBJECT
    Position.hpp
    Color.hpp
    errors.hpp
    errors.cpp
    Context.hpp
    Context.cpp
    Camera.hpp
    Camera.cpp
    Buffer.hpp
    Buffer.cpp
    Grid.hpp
    Grid.cpp
    GridVector.hpp
    GridVector.cpp
    Shader.hpp
    Shader.cpp
    Program.hpp
    Program.cpp
)

# ######################################################################### #
