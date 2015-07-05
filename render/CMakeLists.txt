# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

cmake_minimum_required(VERSION 2.8.8)

# ######################################################################### #

if (!ENABLE_RENDER)
    message(FATAL_ERROR "Cannot build 'render' without ENABLE_RENDER")
endif ()

# ######################################################################### #

# Create project
project(render CXX)

# ######################################################################### #

# OpenGL library is required
find_package(OpenGL REQUIRED)

# Include OpenGL path
include_directories(
    ${OPENGL_INCLUDE_DIR}
)

# ######################################################################### #

# Current version is unable to add object library into shared library.
if (UNIX)
    add_compile_options(-fPIC)
endif ()

if (WIN32)
    add_definitions(-DSIMULATOR_DLL_EXPORT=1)
endif ()

# ######################################################################### #

# Create library
add_library(${PROJECT_NAME} OBJECT
    datatype.hpp
    Color.hpp
    Color.cpp
    errors.hpp
    errors.cpp
    Context.hpp
    Context.cpp
    Object.hpp
    Object.cpp
    Camera.hpp
    Camera.cpp
    Buffer.hpp
    Buffer.cpp
    GridBase.hpp
    Grid.hpp
    Grid.cpp
    GridVector.hpp
    GridVector.cpp
    GridColor.hpp
    GridColor.cpp
    GridColorSmooth.hpp
    GridColorSmooth.cpp
    Shader.hpp
    Shader.cpp
    Program.hpp
    Program.cpp
    VertexElement.hpp
    VertexFormat.hpp
    Texture.hpp
    Texture.cpp
    Circle.hpp
    Circle.cpp
    Rectangle.hpp
    Rectangle.cpp
    ImageData.hpp
    glext.h
)

# ######################################################################### #
