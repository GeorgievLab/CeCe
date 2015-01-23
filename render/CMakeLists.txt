# ######################################################################### #

project(render)

# Create library
add_library(render
    Context.h
    Context.cpp
    Camera.hpp
    Camera.cpp
)

# Setup dependencies
add_dependencies(render
    core
    simulator
)

# ######################################################################### #
