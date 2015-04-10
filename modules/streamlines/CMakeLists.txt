# ######################################################################### #

project(module-streamlines)

# Create library
add_library(module-streamlines
    Module.hpp
    Module.cpp
)

# Setup dependencies
add_dependencies(module-streamlines
    core
    simulator
)

# ######################################################################### #
