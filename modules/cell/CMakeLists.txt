# ######################################################################### #

project(module-cell)

# Create library
add_library(module-cell
    Module.hpp
    Module.cpp
    Cell.hpp
    Cell.cpp
    Yeast.hpp
    Yeast.cpp
    Generator.hpp
    Generator.cpp
)

# Setup dependencies
add_dependencies(module-cell
    core
    simulator
)

# ######################################################################### #
