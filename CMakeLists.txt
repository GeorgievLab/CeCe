# ######################################################################### #

cmake_minimum_required(VERSION 2.8)

# Workspace name
project(grow_simulator)

# Default include directories
include_directories(
    ${CMAKE_SOURCE_DIR}
)

# Add flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors -pedantic -std=c++11 -Wall")

# Projects
add_subdirectory(core)
add_subdirectory(render)
add_subdirectory(physics)
add_subdirectory(simulator)
add_subdirectory(javascript)
add_subdirectory(gui)

# ######################################################################### #
