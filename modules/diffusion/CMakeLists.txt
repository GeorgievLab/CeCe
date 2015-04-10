# ######################################################################### #

project(module-diffusion)

# Create library
add_library(module-diffusion
    Module.hpp
    Module.cpp
)

# Setup dependencies
add_dependencies(module-diffusion
    core
    simulator
)

# ######################################################################### #
