# ######################################################################### #

project(module-physics)

# Create library
add_library(module-physics
    Module.hpp
    Module.cpp
)

# Setup dependencies
add_dependencies(module-physics
    core
    simulator
)

# ######################################################################### #
