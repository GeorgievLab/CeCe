# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #

cmake_minimum_required(VERSION 2.8.8)

# ######################################################################### #

# Create project
project(parser CXX)

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
    Parser.hpp
    Parser.cpp
)

# ######################################################################### #
