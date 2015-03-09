# ######################################################################### #

project(render)

# Create library
add_library(render
    Position.hpp
    Color.hpp
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
